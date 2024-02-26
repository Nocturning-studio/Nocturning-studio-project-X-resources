////////////////////////////////////////////////////////////////////////////
//	Created		: 22.12.2022
//	Author		: Deathman
//	Basic idea 	: xRay engine 2.0 sm 4.0 
////////////////////////////////////////////////////////////////////////////
#ifndef	COMMON_UNIFORMS_H_INCLUDED
#define	COMMON_UNIFORMS_H_INCLUDED
////////////////////////////////////////////////////////////////////////////
#pragma pack_matrix(row_major)
////////////////////////////////////////////////////////////////////////////
uniform float3x4		m_W;
uniform float3x4		m_V;
uniform float4x4 	m_P;
uniform float3x4		m_WV;
uniform float4x4 	m_VP;
uniform float4x4 	m_WVP;
uniform float4		timers;
uniform float4 		env_color;	// color.w  = lerp factor
uniform float3		L_sun_color;
uniform float3		L_sun_dir_w;
uniform float3		L_sun_dir_e;
uniform float4		L_hemi_color;
uniform float4		L_ambient;		// L_ambient.w = skynbox-lerp-factor
uniform float3 		eye_position;
uniform float3		eye_direction;
uniform float3		eye_normal;
uniform	float4 		dt_params;
uniform float4		fog_params;
uniform float3		fog_color;

uniform float4		L_dynamic_props;	// per object, xyz=sun,w=hemi
uniform float4		L_dynamic_color;	// dynamic light color (rgb1)	- spot/point
uniform float4		L_dynamic_pos;		// dynamic light pos+1/range(w) - spot/point
uniform float4x4 	L_dynamic_xform;

uniform float4x4		m_plmap_xform;
uniform float4 		m_plmap_clamp	[2];	// 0.w = factor
////////////////////////////////////////////////////////////////////////////
#endif	//	COMMON_UNIFORMS_H_INCLUDED