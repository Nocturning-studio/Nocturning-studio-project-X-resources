////////////////////////////////////////////////////////////////////////////
//	Created		: 26.11.2023
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
#include "common.h"
////////////////////////////////////////////////////////////////////////////
#define LIGHTMAP_BRIGHTNESS 0.7f
////////////////////////////////////////////////////////////////////////////
float3 CalculateAmbientWithBlurredEnvironment(float LightMapAO, float3 Normal, float Roughness)
{
    LightMapAO += avg3(sun_color) * 0.1f;
    float3 WorldSpaceNormal = mul(m_v2w, Normal);
    float3 EnvironmentActual = texCUBElod(env_s0, float4(WorldSpaceNormal, Roughness * 8.0f));
    float3 EnvironmentNext = texCUBElod(env_s1, float4(WorldSpaceNormal, Roughness * 8.0f));
    float3 Environment = lerp(EnvironmentActual, EnvironmentNext, env_color.w);
    Environment = env_color * lerp(Environment, fog_color.rgb, fog_sky_influence);
    return Environment * LightMapAO * LIGHTMAP_BRIGHTNESS + L_ambient.rgb;
}
////////////////////////////////////////////////////////////////////////////
