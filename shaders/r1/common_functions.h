////////////////////////////////////////////////////////////////////////////
//	Created		: 22.12.2022
//	Author		: Deathman
//	Basic idea 	: xRay engine 2.0 sm 4.0 
////////////////////////////////////////////////////////////////////////////
#ifndef COMMON_FUNCTIONS_H_INCLUDED
#define COMMON_FUNCTIONS_H_INCLUDED
////////////////////////////////////////////////////////////////////////////
#include "shared\fog.h" // R1/R2 common shader
////////////////////////////////////////////////////////////////////////////
// Shared common
half3 	unpack_normal(half3 v) { return 2 * v - 1; }
half3 	unpack_bx2(half3 v) { return 2 * v - 1; }
half3 	unpack_bx4(half3 v) { return 4 * v - 2; }

half2 	unpack_tc_base(half2 tc, half du, half dv) {
	return (tc.xy + half2(du, dv)) * (32.f / 32768.f);
}

half2 	unpack_tc_lmap(half2 tc) { return tc * (1.f / 32768.f); } // [-1  .. +1 ]

half 	calc_cyclic(half x) {
	half 	phase = 1 / (2 * 3.141592653589f);
	half 	sqrt2 = 1.4142136f;
	half 	sqrt2m2 = 2.8284271f;
	half 	f = sqrt2m2 * frac(x) - sqrt2;	// [-sqrt2 .. +sqrt2]
	return 	f * f - 1.f;				// [-1     .. +1]
}
half2 	calc_xz_wave(half2 dir2D, half frac) {
	// Beizer
	half2  ctrl_A = half2(0.f, 0.f);
	half2 	ctrl_B = half2(dir2D.x, dir2D.y);
	return  lerp(ctrl_A, ctrl_B, frac);
}

half2 	calc_detail 	(half3 w_pos)	{ 
	half  	dtl	= distance(w_pos,eye_position)*dt_params.w;
		dtl	= min(dtl*dtl, 1);
	half  	dt_mul	= 1  - dtl;	// dt*  [1 ..  0 ]
	half  	dt_add	= .5 * dtl;	// dt+	[0 .. 0.5]
	return	half2	(dt_mul,dt_add);
}
half3 	calc_reflection	(half3 pos_w, half3 norm_w)
{
    return reflect(normalize(pos_w-eye_position), norm_w);
}
half4	calc_spot 	(out half4 tc_lmap, out half2 tc_att, half4 w_pos, half3 w_norm)	{
	half4 	s_pos	= mul	(L_dynamic_xform, w_pos);
	tc_lmap		= s_pos.xyww;			// projected in ps/ttf
	tc_att 		= s_pos.z;			// z=distance * (1/range)
	half3 	L_dir_n = normalize	(w_pos - L_dynamic_pos.xyz);
	half 	L_scale	= dot(w_norm,-L_dir_n);
	return 	L_dynamic_color*L_scale*saturate(1-CalcVertexFogness(w_pos));
}
half4	calc_point 	(out half2 tc_att0, out half2 tc_att1, half4 w_pos, half3 w_norm)	{
	half3 	L_dir_n = normalize	(w_pos - L_dynamic_pos.xyz);
	half 	L_scale	= dot		(w_norm,-L_dir_n);
	half3	L_tc 	= (w_pos - L_dynamic_pos.xyz) * L_dynamic_pos.w + .5f;	// tc coords
	tc_att0		= L_tc.xz;
	tc_att1		= L_tc.xy;
	return 	L_dynamic_color*L_scale*saturate(1-CalcVertexFogness(w_pos));
}
half3	calc_sun		(half3 norm_w)	{ return max(dot((norm_w),-L_sun_dir_w),0); 		}
half3 	calc_model_hemi 	(half3 norm_w)	{ return (norm_w.y*0.5+0.5)*L_dynamic_props.w*L_hemi_color; 		}
half3	calc_model_lq_lighting	(half3 norm_w) { return calc_model_hemi(norm_w) + L_ambient + L_dynamic_props.xyz*calc_sun(norm_w); 	}
half3 	_calc_model_hemi 	(half3 norm_w)	{ return max(0,norm_w.y)*.2*L_hemi_color; 				}
half3	_calc_model_lq_lighting	(half3 norm_w) { return calc_model_hemi(norm_w) + L_ambient + .5*calc_sun(norm_w); 	}
half4 	calc_model_lmap 	(half3 pos_w)	{
	half3  pos_wc	= clamp		(pos_w,m_plmap_clamp[0],m_plmap_clamp[1]);		// clamp to BBox
	half4 	pos_w4c	= half4	(pos_wc,1);	
	half4 	plmap 	= mul		(m_plmap_xform,pos_w4c);				// calc plmap tc
	return  plmap.xyww;
}

half3	v_hemi 		(half3 n)		{	return L_hemi_color/* *(.5f + .5f*n.y) */; 		}
half3	v_hemi_wrap	(half3 n, half w)	{	return L_hemi_color/* *(w + (1-w)*n.y) */; 		}
half3 	v_sun 		(half3 n)		{	return L_sun_color*max(0,dot(n,-L_sun_dir_w));		}
half3 	v_sun_wrap	(half3 n, half w)	{	return L_sun_color*(w+(1-w)*dot(n,-L_sun_dir_w));	}
half3	p_hemi		(half2 tc) 	{
	half3	t_lmh 	= tex2D		(s_hemi, tc);
	return  dot	(t_lmh,1.h/3.h);
}

half4	watermove	(half4 P)	{
	half3 	wave1	= half3(0.11f,0.13f,0.07f)*W_POSITION_SHIFT_SPEED	;
	half 	dh	= sin  	(timers.x+dot((half3)P,wave1))			;
		P.y	+= dh * W_POSITION_SHIFT_HEIGHT	;
	return 	P	;
}
half2	watermove_tc	(half2 base, half2 P, half amp)	{
	half2 	wave1	= 	half2	(0.2111f,0.2333f)*amp	;
	half 	angle 	= 	timers.z + dot (P,wave1)	;
	half 	du	= 	sin  	(angle);
	half 	dv	= 	cos  	(angle);
		return	(base + amp*half2(du,dv));
}

half3	waterrefl	(out half amount, half3 P, half3 N)	{
	half3 	v2point	= normalize	(P-eye_position);
	half3	vreflect= reflect	(v2point, N);
	half 	fresnel	= (.5f + .5f*dot(vreflect,v2point));
		amount	= 1 - fresnel*fresnel;			// 0=full env, 1=no env
	return	vreflect;
}

half4 	wmark_shift 	(half3 pos, half3 norm)
{
	half3	P 	= 	pos;
	half3 	N 	= 	norm;
	half3	sd 	= 	eye_position-P;
	half 	d 	= 	length(sd);
	half 	w 	= 	min(d/RANGE,1.f);
	half 	s 	= 	lerp(MIN_SHIFT,MAX_SHIFT,d);
		P	+=	N.xyz*NORMAL_SHIFT;
		P	-=	normalize(eye_direction + normalize(P-eye_position)) * s;
	return	half4	(P,1.f);
}
////////////////////////////////////////////////////////////////////////////
#endif // #ifndef COMMON_FUNCTIONS_H_INCLUDED