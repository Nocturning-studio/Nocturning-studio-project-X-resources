////////////////////////////////////////////////////////////////////////////
//	Created		: 27.07.2022
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
uniform float4 ao_resolution;

uniform float4 L_material;     // 0,0,0,mid
uniform float4 Ldynamic_color; // dynamic light color (rgb1)	- spot/point
uniform float4 Ldynamic_pos;   // dynamic light pos+1/range(w) - spot/point
uniform float4 Ldynamic_dir;   // dynamic light direction 	- sun

uniform float4 J_direct[6];
uniform float4 J_spot[6];
uniform float4 m_lmap[2];
uniform float4x4 m_shadow;

uniform float4 wind_velocity;

uniform float4 dof_params; // x - near, y - focus, z - far, w - sky distance
uniform float3 dof_kernel; // x, y - resolution pre-scaled, z - just kernel size

uniform float2 parallax_height;

uniform float3 sun_dir;
uniform float3 sun_color;
////////////////////////////////////////////////////////////////////////////