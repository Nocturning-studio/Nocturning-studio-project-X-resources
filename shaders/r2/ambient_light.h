////////////////////////////////////////////////////////////////////////////
//	Created		: 26.11.2023
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
#include "common.h"
////////////////////////////////////////////////////////////////////////////
#define LIGHTMAP_BRIGHTNESS 1.0h
////////////////////////////////////////////////////////////////////////////
float3 CalculateAmbient(float LightMapAO, float3 Normal)
{
    float3 WorldSpaceNormal = mul(m_v2w, Normal);
    float3 EnvironmentActual = texCUBE(env_s0, WorldSpaceNormal);
	float3 EnvironmentNext = texCUBE(env_s1, WorldSpaceNormal);
    float3 Environment = lerp(EnvironmentActual, EnvironmentNext, env_color.w);
    Environment = env_color * lerp(Environment, fog_color.rgb, fog_sky_influence);
    return Environment * LightMapAO * LIGHTMAP_BRIGHTNESS + L_ambient.rgb;
}
////////////////////////////////////////////////////////////////////////////