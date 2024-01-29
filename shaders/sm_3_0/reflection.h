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
vector3 BlurredSkyReflectionApproximated(vector3 Point, vector3 Normal, vector Power)
{
    vector3 WorldSpaceNormal = mul(m_v2w, Normal);
    vector3 v2pointL = normalize(Point);
    vector3 v2point = mul(m_v2w, v2pointL);
    vector3 vreflect = reflect(v2point, WorldSpaceNormal);

#ifdef TRUE_REMAPPING_FOR_REFLECTIONS
    vector3 vreflectabs = abs(vreflect);
    vector vreflectmax = max(vreflectabs.x, max(vreflectabs.y, vreflectabs.z));
    vreflect /= vreflectmax;
    if (vreflect.y < 0.99h)
#endif
        vreflect.y = vreflect.y * 2.0h - 1.0h;

    vector3 SkyUblurred0 = texCUBE(sky_s0, vreflect);
    vector3 SkyUblurred1 = texCUBE(sky_s1, vreflect);
    vector3 SkyUblurred = lerp(SkyUblurred0, SkyUblurred1, env_color.w);

    vector3 SkyBlurred0 = texCUBE(env_s0, vreflect);
    vector3 SkyBlurred1 = texCUBE(env_s1, vreflect);
    vector3 SkyBlurred = lerp(SkyBlurred0, SkyBlurred1, env_color.w);

    vector3 Environment = lerp(SkyBlurred, SkyUblurred, Power);

    return Environment;
}
////////////////////////////////////////////////////////////////////////////
#endif // REFLECTION_INCLUDED
////////////////////////////////////////////////////////////////////////////
