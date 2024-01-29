////////////////////////////////////////////////////////////////////////////
// Created: 20.11.2023
// Author: Deathman
// Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
#ifndef SHADOW_FILTERING_INCLUDED
#define SHADOW_FILTERING_INCLUDED
////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "filters.h"
////////////////////////////////////////////////////////////////////////////
//#define USE_JITTERING
////////////////////////////////////////////////////////////////////////////
#if SHADOW_FILTER == LOW_FILTERING
#define SHADOW_FILTERING_SAMPLES_COUNT 4
#define OFFSET_ARRAY box_coords
#elif SHADOW_FILTER == MIDDLE_FILTERING
#define SHADOW_FILTERING_SAMPLES_COUNT 9
#define OFFSET_ARRAY box_coords
#elif SHADOW_FILTER == HIGHT_FILTERING
#define SHADOW_FILTERING_SAMPLES_COUNT 9
#define OFFSET_ARRAY strided_coords
#elif SHADOW_FILTER == DISABLE_FILTERING
#define SHADOW_FILTERING_SAMPLES_COUNT 1
#define OFFSET_ARRAY strided_coords
#endif
////////////////////////////////////////////////////////////////////////////
vector sample_shadow_map(vector4 TexCoords, vector2 SampleBias)
{
    vector ShadowMapPixelSize = 1.0h / float(SMAP_size);
    vector4 SampleTexCoords = vector4(TexCoords + TexCoords.w * vector4(SampleBias, 0.0h, 0.0h) * ShadowMapPixelSize);

    return tex2Dproj(s_smap, SampleTexCoords).x;
}

vector shadow_map_filter(vector4 TexCoords)
{
    vector ShadowMap = 0.0h;

    for (int i = 0; i < SHADOW_FILTERING_SAMPLES_COUNT; i++)
    {
#ifdef USE_JITTERING
        vector2 JitterCoords = tex2Dlod0(s_blue_noise, (TexCoords + poissonDisk_64[i]) * vector(SMAP_size) / 256.0h);
        ShadowMap += sample_shadow_map(TexCoords, OFFSET_ARRAY[i] + JitterCoords);
#else
        ShadowMap += sample_shadow_map(TexCoords, OFFSET_ARRAY[i]);
#endif
    }

    return ShadowMap / SHADOW_FILTERING_SAMPLES_COUNT;
}

vector get_shadow_map(vector4 TexCoords)
{
    return shadow_map_filter(TexCoords);
}
////////////////////////////////////////////////////////////////////////////
#endif // SHADOW_FILTERING_INCLUDED
////////////////////////////////////////////////////////////////////////////
