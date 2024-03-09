////////////////////////////////////////////////////////////////////////////
//	Created		: 22.12.2022
//	Author		: Deathman
//	Basic idea 	: xRay engine 2.0 sm 4.0 
////////////////////////////////////////////////////////////////////////////
float4 calc_spot (out float4 tc_lmap, out float2 tc_att, float4 w_pos, float3 w_norm)	
{
	float4 s_pos = mul(L_dynamic_xform, w_pos);
	tc_lmap	= s_pos.xyww; // projected in ps/ttf
	tc_att = s_pos.z; // z=distance * (1/range)
	float3 L_dir_n = normalize(w_pos - L_dynamic_pos.xyz);
	float L_scale = dot(w_norm, -L_dir_n);
	return L_dynamic_color * L_scale * saturate(1.0f - CalcVertexFogness(w_pos));
}

float4 calc_point (out float2 tc_att0, out float2 tc_att1, float4 w_pos, float3 w_norm)	
{
	float3 L_dir_n = normalize	(w_pos - L_dynamic_pos.xyz);
	float L_scale = dot	(w_norm,-L_dir_n);
	float3 L_tc = (w_pos - L_dynamic_pos.xyz) * L_dynamic_pos.w + 0.5f;	// tc coords
	tc_att0	= L_tc.xz;
	tc_att1	= L_tc.xy;
	return L_dynamic_color * L_scale * saturate(1.0f - CalcVertexFogness(w_pos));
}

float3 calc_sun	(float3 norm_w)	
{ 
	return max(dot((norm_w), -L_sun_dir_w), 0.0f); 		
}

float3 calc_model_hemi(float3 norm_w)	
{ 
	return (norm_w.y * 0.5f + 0.5f) * L_dynamic_props.w * L_hemi_color; 		
}

float3 calc_model_lq_lighting(float3 norm_w) 
{ 
	return calc_model_hemi(norm_w) + L_ambient + L_dynamic_props.xyz * calc_sun(norm_w); 
}

float4 calc_model_lmap(float3 pos_w)	
{
	float3 pos_wc = clamp(pos_w, m_plmap_clamp[0], m_plmap_clamp[1]);	// clamp to BBox
	float4 pos_w4c = float4	(pos_wc, 1.0f);	
	float4 plmap = mul(m_plmap_xform,pos_w4c); // calc plmap tc
	return plmap.xyww;
}

float3 v_hemi(float3 n)	
{	
	return L_hemi_color/* *(.5f + .5f*n.y) */; 		
}

float3 v_hemi_wrap(float3 n, float w)	
{	
	return L_hemi_color/* *(w + (1-w)*n.y) */; 		
}

float3 v_sun(float3 n)		
{	
	return L_sun_color * max(0.0f, dot(n, -L_sun_dir_w));		
}

float3 v_sun_wrap(float3 n, float w)
{	
	return L_sun_color * (w + (1.0f - w) * dot(n, -L_sun_dir_w));	
}

float3 p_hemi(float2 tc) 	
{
	float3 t_lmh = tex2D(s_hemi, tc);
	return dot(t_lmh, 1.0f / 3.0f);
}
////////////////////////////////////////////////////////////////////////////