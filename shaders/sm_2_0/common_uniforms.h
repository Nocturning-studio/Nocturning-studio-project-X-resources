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
uniform half3x4		m_W;
uniform half3x4		m_V;
uniform half4x4 	m_P;
uniform half3x4		m_WV;
uniform half4x4 	m_VP;
uniform half4x4 	m_WVP;
uniform half4		timers;
uniform half4		fog_plane;
uniform half4		fog_params;		// x=near*(1/(far-near)), ?,?, w = -1/(far-near)
uniform half4		fog_color;
uniform half4 		env_color;	// color.w  = lerp factor
uniform half3		L_sun_color;
uniform half3		L_sun_dir_w;
uniform half3		L_sun_dir_e;
uniform half4		L_hemi_color;
uniform half4		L_ambient;		// L_ambient.w = skynbox-lerp-factor
uniform half3 		eye_position;
uniform half3		eye_direction;
uniform half3		eye_normal;
uniform	half4 		dt_params;

uniform half4		L_dynamic_props;	// per object, xyz=sun,w=hemi
uniform half4		L_dynamic_color;	// dynamic light color (rgb1)	- spot/point
uniform half4		L_dynamic_pos;		// dynamic light pos+1/range(w) - spot/point
uniform half4x4 	L_dynamic_xform;

uniform half4x4		m_plmap_xform;
uniform half4 		m_plmap_clamp	[2];	// 0.w = factor
////////////////////////////////////////////////////////////////////////////
#endif	//	COMMON_UNIFORMS_H_INCLUDED