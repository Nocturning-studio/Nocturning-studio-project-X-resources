////////////////////////////////////////////////////////////////////////////
//	Created		: 27.11.2023
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
#ifndef REFLECTION_INCLUDED
#define REFLECTION_INCLUDED
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
    if (vreflect.y < 0.99h)
#endif
        vreflect.y = vreflect.y * 2.0h - 1.0h;

    float3 Environment0 = texCUBElod(sky_s0, float4(vreflect, 4.0h * Roughness));
    float3 Environment1 = texCUBElod(sky_s1, float4(vreflect, 4.0h * Roughness));
    float3 Environment = lerp(Environment0, Environment1, env_color.w);

    return lerp(Environment, fog_color.rgb, fog_sky_influence);
}
////////////////////////////////////////////////////////////////////////////
#endif // REFLECTION_INCLUDED
////////////////////////////////////////////////////////////////////////////
