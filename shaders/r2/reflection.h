////////////////////////////////////////////////////////////////////////////
//	Created		: 27.11.2023
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
#include "common.h"
////////////////////////////////////////////////////////////////////////////
#define TRUE_REMAPPING_FOR_REFLECTIONS
////////////////////////////////////////////////////////////////////////////
float3 BlurredSkyReflection(float3 Point, float3 Normal, float Roughness)
{
    float3 WorldSpaceNormal = mul(m_v2w, Normal);
    float3 v2pointL = normalize(Point);
    float3 v2point = mul(m_v2w, v2pointL);
    float3 vreflect = reflect(v2point, WorldSpaceNormal);

#ifdef TRUE_REMAPPING_FOR_REFLECTIONS
    float3 vreflectabs = abs(vreflect);
    float vreflectmax = max(vreflectabs.x, max(vreflectabs.y, vreflectabs.z));
    vreflect /= vreflectmax;
    if (vreflect.y < 0.99f)
#endif
        vreflect.y = vreflect.y * 2.0f - 1.0f;

    float3 Environment0 = texCUBElod(env_s0, float4(vreflect, 8.0f * Roughness));
    float3 Environment1 = texCUBElod(env_s1, float4(vreflect, 8.0f * Roughness));
    float3 Environment = lerp(Environment0, Environment1, env_color.w);

    float3 Sky0 = texCUBElod(sky_s0, float4(vreflect, 8.0f * Roughness));
    float3 Sky1 = texCUBElod(sky_s1, float4(vreflect, 8.0f * Roughness));
    float3 Sky = lerp(Sky0, Sky1, env_color.w);

    Environment = lerp(Sky, Environment, Roughness);

    return lerp(Environment, fog_color.rgb, fog_sky_influence);
}
////////////////////////////////////////////////////////////////////////////