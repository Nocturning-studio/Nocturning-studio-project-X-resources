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
// Deathman - линейно интерполируем размытое и неразмытое
//            небо по значению глянцевитости поверхности
////////////////////////////////////////////////////////////////////////////
float3 BlurredSkyReflectionApproximated(float3 Point, float3 Normal, float Power)
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

    float3 SkyUblurred0 = texCUBE(sky_s0, vreflect);
    float3 SkyUblurred1 = texCUBE(sky_s1, vreflect);
    float3 SkyUblurred = lerp(SkyUblurred0, SkyUblurred1, env_color.w);

    float3 SkyBlurred0 = texCUBE(env_s0, vreflect);
    float3 SkyBlurred1 = texCUBE(env_s1, vreflect);
    float3 SkyBlurred = lerp(SkyBlurred0, SkyBlurred1, env_color.w);

    float3 Environment = lerp(SkyBlurred, SkyUblurred, Power);

    return Environment;
}
////////////////////////////////////////////////////////////////////////////
#endif // REFLECTION_INCLUDED
////////////////////////////////////////////////////////////////////////////
