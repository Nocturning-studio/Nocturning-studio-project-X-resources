////////////////////////////////////////////////////////////////////////////
//	Created		: 22.12.2022
//	Author		: Deathman
//	Basic idea 	: xRay engine 2.0 sm 4.0 
////////////////////////////////////////////////////////////////////////////
float 	calc_cyclic(float x) {
	float 	phase = 1 / (2 * 3.141592653589f);
	float 	sqrt2 = 1.4142136f;
	float 	sqrt2m2 = 2.8284271f;
	float 	f = sqrt2m2 * frac(x) - sqrt2;	// [-sqrt2 .. +sqrt2]
	return 	f * f - 1.f;				// [-1     .. +1]
}
float2 	calc_xz_wave(float2 dir2D, float frac) {
	// Beizer
	float2  ctrl_A = float2(0.f, 0.f);
	float2 	ctrl_B = float2(dir2D.x, dir2D.y);
	return  lerp(ctrl_A, ctrl_B, frac);
}

float2 	calc_detail 	(float3 w_pos)	{ 
	float  	dtl	= distance(w_pos,eye_position)*dt_params.w;
		dtl	= min(dtl*dtl, 1);
	float  	dt_mul	= 1  - dtl;	// dt*  [1 ..  0 ]
	float  	dt_add	= .5 * dtl;	// dt+	[0 .. 0.5]
	return	float2	(dt_mul,dt_add);
}
float3 	calc_reflection	(float3 pos_w, float3 norm_w)
{
    return reflect(normalize(pos_w-eye_position), norm_w);
}
float4	calc_spot 	(out float4 tc_lmap, out float2 tc_att, float4 w_pos, float3 w_norm)	{
	float4 	s_pos	= mul	(L_dynamic_xform, w_pos);
	tc_lmap		= s_pos.xyww;			// projected in ps/ttf
	tc_att 		= s_pos.z;			// z=distance * (1/range)
	float3 	L_dir_n = normalize	(w_pos - L_dynamic_pos.xyz);
	float 	L_scale	= dot(w_norm,-L_dir_n);
	return 	L_dynamic_color*L_scale*saturate(1-CalcVertexFogness(w_pos));
}
float4	calc_point 	(out float2 tc_att0, out float2 tc_att1, float4 w_pos, float3 w_norm)	{
	float3 	L_dir_n = normalize	(w_pos - L_dynamic_pos.xyz);
	float 	L_scale	= dot		(w_norm,-L_dir_n);
	float3	L_tc 	= (w_pos - L_dynamic_pos.xyz) * L_dynamic_pos.w + .5f;	// tc coords
	tc_att0		= L_tc.xz;
	tc_att1		= L_tc.xy;
	return 	L_dynamic_color*L_scale*saturate(1-CalcVertexFogness(w_pos));
}
float3	calc_sun		(float3 norm_w)	{ return max(dot((norm_w),-L_sun_dir_w),0); 		}
float3 	calc_model_hemi 	(float3 norm_w)	{ return (norm_w.y*0.5+0.5)*L_dynamic_props.w*L_hemi_color; 		}
float3	calc_model_lq_lighting	(float3 norm_w) { return calc_model_hemi(norm_w) + L_ambient + L_dynamic_props.xyz*calc_sun(norm_w); 	}
float3 	_calc_model_hemi 	(float3 norm_w)	{ return max(0,norm_w.y)*.2*L_hemi_color; 				}
float3	_calc_model_lq_lighting	(float3 norm_w) { return calc_model_hemi(norm_w) + L_ambient + .5*calc_sun(norm_w); 	}
float4 	calc_model_lmap 	(float3 pos_w)	{
	float3  pos_wc	= clamp		(pos_w,m_plmap_clamp[0],m_plmap_clamp[1]);		// clamp to BBox
	float4 	pos_w4c	= float4	(pos_wc,1);	
	float4 	plmap 	= mul		(m_plmap_xform,pos_w4c);				// calc plmap tc
	return  plmap.xyww;
}

float3	v_hemi 		(float3 n)		{	return L_hemi_color/* *(.5f + .5f*n.y) */; 		}
float3	v_hemi_wrap	(float3 n, float w)	{	return L_hemi_color/* *(w + (1-w)*n.y) */; 		}
float3 	v_sun 		(float3 n)		{	return L_sun_color*max(0,dot(n,-L_sun_dir_w));		}
float3 	v_sun_wrap	(float3 n, float w)	{	return L_sun_color*(w+(1-w)*dot(n,-L_sun_dir_w));	}
float3	p_hemi		(float2 tc) 	{
	float3	t_lmh 	= tex2D		(s_hemi, tc);
	return  dot	(t_lmh,1.h/3.h);
}

float4	watermove	(float4 P)	{
	float3 	wave1	= float3(0.11f,0.13f,0.07f)*W_POSITION_SHIFT_SPEED	;
	float 	dh	= sin  	(timers.x+dot((float3)P,wave1))			;
		P.y	+= dh * W_POSITION_SHIFT_HEIGHT	;
	return 	P	;
}
float2	watermove_tc	(float2 base, float2 P, float amp)	{
	float2 	wave1	= 	float2	(0.2111f,0.2333f)*amp	;
	float 	angle 	= 	timers.z + dot (P,wave1)	;
	float 	du	= 	sin  	(angle);
	float 	dv	= 	cos  	(angle);
		return	(base + amp*float2(du,dv));
}

float3	waterrefl	(out float amount, float3 P, float3 N)	{
	float3 	v2point	= normalize	(P-eye_position);
	float3	vreflect= reflect	(v2point, N);
	float 	fresnel	= (.5f + .5f*dot(vreflect,v2point));
		amount	= 1 - fresnel*fresnel;			// 0=full env, 1=no env
	return	vreflect;
}

float4 	wmark_shift 	(float3 pos, float3 norm)
{
	float3	P 	= 	pos;
	float3 	N 	= 	norm;
	float3	sd 	= 	eye_position-P;
	float 	d 	= 	length(sd);
	float 	w 	= 	min(d/RANGE,1.f);
	float 	s 	= 	lerp(MIN_SHIFT,MAX_SHIFT,d);
		P	+=	N.xyz*NORMAL_SHIFT;
		P	-=	normalize(eye_direction + normalize(P-eye_position)) * s;
	return	float4	(P,1.f);
}
////////////////////////////////////////////////////////////////////////////