////////////////////////////////////////////////////////////////////////////
//	Created		: 27.07.2022
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
#ifndef COMMON_UNIFORMS_H_INCLUDED
#define COMMON_UNIFORMS_H_INCLUDED
////////////////////////////////////////////////////////////////////////////
#include "common_math.h"
////////////////////////////////////////////////////////////////////////////
uniform float4 screen_res;
uniform float4 ao_resolution;

uniform float3 L_sun_color;
uniform float3 L_sun_dir_w;
uniform float3 L_sun_dir_e;
uniform float4 L_hemi_color;
uniform float4 L_ambient;      // L_ambient.w = skynbox-lerp-factor
uniform float4 L_material;     // 0,0,0,mid
uniform float4 Ldynamic_color; // dynamic light color (rgb1)	- spot/point
uniform float4 Ldynamic_pos;   // dynamic light pos+1/range(w) - spot/point
uniform float4 Ldynamic_dir;   // dynamic light direction 	- sun

uniform float4 J_direct[6];
uniform float4 J_spot[6];
uniform float4 m_lmap[2];
uniform float4x4 m_shadow;

uniform float3x4 m_W;
uniform float3x4 m_invW;
uniform float3x4 m_V;
uniform float3x4 m_invV;
uniform float4x4 m_P;
uniform float3x4 m_WV;
uniform float4x4 m_VP;
uniform float4x4 m_WVP;
uniform float3x4 m_v2w;
uniform float4x4 m_texgen;
uniform float4x4 m_texgen_J;
uniform float4x4 mVPTexgen;

uniform float4 timers;

uniform float4 fog_plane;
uniform float4 fog_params; // x=near*(1/(far-near)), ?,?, w = -1/(far-near)
uniform float4 fog_color;
uniform float fog_density;
uniform float fog_sky_influence;
uniform float vertical_fog_intensity;
uniform float vertical_fog_density;
uniform float vertical_fog_height;

uniform float far_plane;

uniform float4 env_color;

uniform float4 rain_density;

uniform float4 wind_velocity;

uniform float3 eye_position;
uniform float3 eye_direction;
uniform float3 eye_normal;

uniform float4 dt_params;

// x = -1.0f * tan(FOV * 0.5)
// y = x / aspect
// z = near plane
// w = far plane
uniform float4 pos_decompression_params;

uniform float4 dof_params; // x - near, y - focus, z - far, w - sky distance
uniform float3 dof_kernel; // x, y - resolution pre-scaled, z - just kernel size

uniform float2 parallax_height;

uniform float3 sun_dir;
uniform float3 sun_color;
////////////////////////////////////////////////////////////////////////////
#endif //	COMMON_UNIFORMS_H_INCLUDED
////////////////////////////////////////////////////////////////////////////
