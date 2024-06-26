////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Deadiablo, Deathman
// Original code: https://www.shadertoy.com/view/WscyRl
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "material_components.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define AO_SAMPLES 8.0
#define AO_ATTENUATION 5.0
#define AO_RADIUS 0.1
#define AO_POWER 2.0
#define AO_RENDER_DISTANCE 50.0f
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float AOfromHeight(float2 UV, float WorldSpaceDepth
#ifdef USE_FOR_TERRAIN_MATERIAL
    , float4 Mask
#endif
)
{
    float AO = 1.0f;

    if (WorldSpaceDepth < AO_RENDER_DISTANCE)
    {
#ifdef USE_FOR_TERRAIN_MATERIAL
        float height = GetTerrainDetailHeight(Mask, UV);
#else
        float height = GetHeight(UV);
#endif

        float result = 0.0;
        for (float f = 0.0; f < 1.0; f += 1.0 / AO_SAMPLES)
        {
            float randUoffset = random(float2(f * 16524.56, f * 23532.43));
            float randVoffset = random(float2(f * 21384.12, f * 11835.78));
            float2 offset = float2(randUoffset, randVoffset) * 2.0 - 1.0;
            offset *= AO_RADIUS;

#ifdef USE_FOR_TERRAIN_MATERIAL
            result += max(GetTerrainDetailHeight(Mask, UV + offset) - height, 0.0f);
#else
            result += max(GetHeight(UV + offset) - height, 0.0f);
#endif
        }

        result /= AO_SAMPLES;
        result = pow(1.0 - result, AO_POWER);

        // Create smooth render distance border
        float FarFadingFactor = smoothstep(AO_RENDER_DISTANCE * 0.75f, AO_RENDER_DISTANCE, WorldSpaceDepth);
        result = lerp(result, 1.0f, FarFadingFactor);

        AO = result;
    }

    return AO;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////