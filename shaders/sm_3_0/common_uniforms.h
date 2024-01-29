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
uniform vector4 screen_res;
uniform vector4 ao_resolution;

uniform vector3 L_sun_color;
uniform vector3 L_sun_dir_w;
uniform vector3 L_sun_dir_e;
uniform vector4 L_hemi_color;
uniform vector4 L_ambient;      // L_ambient.w = skynbox-lerp-factor
uniform vector4 L_material;     // 0,0,0,mid
uniform vector4 Ldynamic_color; // dynamic light color (rgb1)	- spot/point
uniform vector4 Ldynamic_pos;   // dynamic light pos+1/range(w) - spot/point
uniform vector4 Ldynamic_dir;   // dynamic light direction 	- sun

uniform vector4 J_direct[6];
uniform vector4 J_spot[6];
uniform vector4 m_lmap[2];
uniform matrix4x4 m_shadow;

uniform matrix3x4 m_W;
uniform matrix3x4 m_invW;
uniform matrix3x4 m_V;
uniform matrix3x4 m_invV;
uniform matrix4x4 m_P;
uniform matrix3x4 m_WV;
uniform matrix4x4 m_VP;
uniform matrix4x4 m_WVP;
uniform matrix3x4 m_v2w;
uniform matrix4x4 m_texgen;
uniform matrix4x4 m_texgen_J;
uniform matrix4x4 mVPTexgen;

uniform vector4 timers;

uniform vector4 fog_plane;
uniform vector4 fog_params; // x=near*(1/(far-near)), ?,?, w = -1/(far-near)
uniform vector4 fog_color;
uniform vector fog_density;
uniform vector fog_sky_influence;
uniform vector vertical_fog_intensity;
uniform vector vertical_fog_density;
uniform vector vertical_fog_height;

uniform vector far_plane;

uniform vector4 env_color;

uniform vector4 rain_density;

uniform vector4 wind_velocity;

uniform vector3 eye_position;
uniform vector3 eye_direction;
uniform vector3 eye_normal;

uniform vector4 dt_params;

uniform vector4 pos_decompression_params;
uniform vector4 pos_decompression_params2;

uniform vector4 dof_params; // x - near, y - focus, z - far, w - sky distance
uniform vector3 dof_kernel; // x, y - resolution pre-scaled, z - just kernel size

uniform vector2 parallax_height;
////////////////////////////////////////////////////////////////////////////
#endif //	COMMON_UNIFORMS_H_INCLUDED
////////////////////////////////////////////////////////////////////////////
