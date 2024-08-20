////////////////////////////////////////////////////////////////////////////
//	Created		: 08.03.2024
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
uniform float3x4 m_W;
uniform float3x4 m_invW;
uniform float3x4 m_V;
uniform float4x4 m_P;
uniform float3x4 m_WV;
uniform float4x4 m_VP;
uniform float4x4 m_WVP;
uniform float3x4 m_v2w;

uniform float3x4 m_xform_v;
uniform float3x4 m_xform;
uniform float4 consts;// {1/quant,1/quant,diffusescale,ambient}
uniform float4 wave; // cx,cy,cz,tm
uniform float4 wind; // direction2D
uniform float4 c_bias; // + color
uniform float4 c_scale;	// * color
uniform float2 c_sun; // x=*, y=+

uniform float4x4 m_texgen;
uniform float4x4 mVPTexgen;

uniform float4 fog_params;
uniform float3 fog_color;
uniform float fog_sky_influence;

uniform float vertical_fog_density;
uniform float vertical_fog_height;

uniform float3 water_intensity; 
uniform float3 sun_shafts_intensity;
uniform float4 rain_density;

uniform float vignette_power;
uniform float4 sepia_params;

uniform float far_plane;

// x = -1.0f * tan(FOV * 0.5)
// y = x / aspect
// z = near plane
// w = far plane
uniform float4 pos_decompression_params;
uniform float4 pos_decompression_params_hud;

uniform float fov;

uniform float4 env_color; // color.w  = lerp factor

uniform float4 timers;

uniform float3 eye_position;
uniform float3 eye_direction;
uniform float3 eye_normal;

uniform float3 L_sun_color;
uniform float3 L_sun_dir_w;
uniform float3 L_sun_dir_e;

uniform float4 L_hemi_color;
uniform float4 L_ambient; // L_ambient.w = skynbox-lerp-factor

uniform float4 screen_res;

uniform	float4 dt_params;
////////////////////////////////////////////////////////////////////////////