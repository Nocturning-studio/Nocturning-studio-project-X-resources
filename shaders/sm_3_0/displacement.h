////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Created		: 07.08.2023
//	Author		: Deathman
//	Noctuning Studio for NS Project X
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef DISPLACEMENT_INCLUDED
#define DISPLACEMENT_INCLUDED
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constant table
#define PARALLAX_H 0.03f
#define constant_parallax_scale float2(PARALLAX_H, -PARALLAX_H / 2)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Quality
#if BUMP_QUALITY == UNDEFINED_QUALITY
#define STEEP_PARALLAX_START_FADE 5
#define STEEP_PARALLAX_STOP_FADE 10
#define PARALLAX_START_FADE 15
#define PARALLAX_STOP_FADE 20
#define STEEP_PARALLAX_MIN_SAMPLES 3
#define STEEP_PARALLAX_MAX_SAMPLES 5

#define DETAIL_STEEP_PARALLAX_START_FADE 4
#define DETAIL_STEEP_PARALLAX_STOP_FADE 8
#define DETAIL_PARALLAX_START_FADE 10
#define DETAIL_PARALLAX_STOP_FADE 15
#define DETAIL_STEEP_PARALLAX_MIN_SAMPLES 1
#define DETAIL_STEEP_PARALLAX_MAX_SAMPLES 4
#elif BUMP_QUALITY == LOW_QUALITY
#define STEEP_PARALLAX_START_FADE 5
#define STEEP_PARALLAX_STOP_FADE 10
#define PARALLAX_START_FADE 15
#define PARALLAX_STOP_FADE 20
#define STEEP_PARALLAX_MIN_SAMPLES 3
#define STEEP_PARALLAX_MAX_SAMPLES 5

#define DETAIL_STEEP_PARALLAX_START_FADE 4
#define DETAIL_STEEP_PARALLAX_STOP_FADE 8
#define DETAIL_PARALLAX_START_FADE 10
#define DETAIL_PARALLAX_STOP_FADE 15
#define DETAIL_STEEP_PARALLAX_MIN_SAMPLES 1
#define DETAIL_STEEP_PARALLAX_MAX_SAMPLES 4
#elif BUMP_QUALITY == MIDDLE_QUALITY
#define STEEP_PARALLAX_START_FADE 7
#define STEEP_PARALLAX_STOP_FADE 12
#define PARALLAX_START_FADE 17
#define PARALLAX_STOP_FADE 22
#define STEEP_PARALLAX_MIN_SAMPLES 2
#define STEEP_PARALLAX_MAX_SAMPLES 6

#define DETAIL_STEEP_PARALLAX_START_FADE 5
#define DETAIL_STEEP_PARALLAX_STOP_FADE 10
#define DETAIL_PARALLAX_START_FADE 10
#define DETAIL_PARALLAX_STOP_FADE 20
#define DETAIL_STEEP_PARALLAX_MIN_SAMPLES 2
#define DETAIL_STEEP_PARALLAX_MAX_SAMPLES 4
#elif BUMP_QUALITY == HIGHT_QUALITY
#define STEEP_PARALLAX_START_FADE 10
#define STEEP_PARALLAX_STOP_FADE 15
#define PARALLAX_START_FADE 20
#define PARALLAX_STOP_FADE 25
#define STEEP_PARALLAX_MIN_SAMPLES 4
#define STEEP_PARALLAX_MAX_SAMPLES 8

#define DETAIL_STEEP_PARALLAX_START_FADE 7
#define DETAIL_STEEP_PARALLAX_STOP_FADE 12
#define DETAIL_PARALLAX_START_FADE 15
#define DETAIL_PARALLAX_STOP_FADE 20
#define DETAIL_STEEP_PARALLAX_MIN_SAMPLES 2
#define DETAIL_STEEP_PARALLAX_MAX_SAMPLES 4
#elif BUMP_QUALITY == ULTRA_QUALITY
#define STEEP_PARALLAX_START_FADE 15
#define STEEP_PARALLAX_STOP_FADE 20
#define PARALLAX_START_FADE 25
#define PARALLAX_STOP_FADE 30
#define STEEP_PARALLAX_MIN_SAMPLES 7
#define STEEP_PARALLAX_MAX_SAMPLES 12

#define DETAIL_STEEP_PARALLAX_START_FADE 10
#define DETAIL_STEEP_PARALLAX_STOP_FADE 15
#define DETAIL_PARALLAX_START_FADE 15
#define DETAIL_PARALLAX_STOP_FADE 20
#define DETAIL_STEEP_PARALLAX_MIN_SAMPLES 4
#define DETAIL_STEEP_PARALLAX_MAX_SAMPLES 6
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float3 GetEyefloat(float3 Position, float3x3 TBN)
{
    TBN = transpose(TBN);
    return normalize(mul(TBN, -Position));
}

float GetHeight(sampler2D HeightmapSampler, float2 UV)
{
    return tex2Dlod0(HeightmapSampler, UV).a;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float2 CalculateParallaxMapping(sampler2D HeightmapSampler, float3 Position, float3x3 TBN, float2 UV,
                                 float StartFadingDistance, float StopFadingDistance)
{
    if (Position.z < StopFadingDistance)
    {
        float3 Eyefloat = GetEyefloat(Position, TBN);

        float height = GetHeight(HeightmapSampler, UV);
        height *= constant_parallax_scale.x;
        height += constant_parallax_scale.y;

        float fParallaxFade = smoothstep(StopFadingDistance, StartFadingDistance, Position.z);
        UV += height * Eyefloat * fParallaxFade;
    }

    return UV;
}

float2 CalculateSteepParallaxOcclusionMapping(sampler2D HeightmapSampler, float3 Position, float3x3 TBN, float2 UV,
                                               float StartFadingDistance, float StopFadingDistance,
                                               int MinimalSamplesCount, int MaximalSamplesCount)
{
    if (Position.z < StopFadingDistance)
    {
        double3 Eyefloat = GetEyefloat(Position, TBN);

        // Calculate number of steps
        double nNumSteps = lerp(MaximalSamplesCount, MinimalSamplesCount, Eyefloat.z);

        double fStepSize = 1.0h / nNumSteps;
        double2 vDelta = -Eyefloat.xy * constant_parallax_scale.x;
        double2 vTexOffsetPerStep = fStepSize * vDelta;

        // Prepare start data for cycle
        double2 vTexCurrentOffset = UV;
        double fCurrHeight = 0.0h;
        double fCurrentBound = 1.0h;

        for (; fCurrHeight < fCurrentBound; fCurrentBound -= fStepSize)
        {
            vTexCurrentOffset += vTexOffsetPerStep;
            double HeightMap = GetHeight(HeightmapSampler, vTexCurrentOffset.xy);
            fCurrHeight = HeightMap;
        }

        // Reconstruct previouse step's data
        vTexCurrentOffset -= vTexOffsetPerStep;
        double fPrevHeight = GetHeight(HeightmapSampler, vTexCurrentOffset.xy);

        // Smooth tc position between current and previouse step
        double fDelta2 = ((fCurrentBound + fStepSize) - fPrevHeight);
        double fDelta1 = (fCurrentBound - fCurrHeight);
        double fParallaxAmount =
            (fCurrentBound * fDelta2 - (fCurrentBound + fStepSize) * fDelta1) / (fDelta2 - fDelta1);
        double fParallaxFade = smoothstep(StopFadingDistance, StartFadingDistance, Position.z);
        double2 vParallaxOffset = vDelta * ((1.0h - fParallaxAmount) * fParallaxFade);

        UV += vParallaxOffset;
    }

    return UV;
}

float2 GetDisplacement(sampler2D Heightmap, float3 Position, float3x3 TBN, float2 UV)
{
#if defined(USE_STEEP_PARALLAX_MAPPING)
    UV = CalculateSteepParallaxOcclusionMapping(Heightmap, Position, TBN, UV, STEEP_PARALLAX_START_FADE,
                                                STEEP_PARALLAX_STOP_FADE, STEEP_PARALLAX_MIN_SAMPLES,
                                                STEEP_PARALLAX_MAX_SAMPLES);
#elif defined(USE_PARALLAX_MAPPING)
    UV = CalculateParallaxMapping(Heightmap, Position, TBN, UV, PARALLAX_START_FADE, PARALLAX_STOP_FADE);
#endif

    return UV;
}

float2 GetDetailDisplacement(sampler2D Heightmap, float3 Position, float3x3 TBN, float2 UV)
{
#if defined(USE_STEEP_PARALLAX_MAPPING)
    UV = CalculateSteepParallaxOcclusionMapping(Heightmap, Position, TBN, UV, DETAIL_STEEP_PARALLAX_START_FADE,
                                                DETAIL_STEEP_PARALLAX_STOP_FADE, DETAIL_STEEP_PARALLAX_MIN_SAMPLES,
                                                DETAIL_STEEP_PARALLAX_MAX_SAMPLES);
#elif defined(USE_PARALLAX_MAPPING)
    UV = CalculateParallaxMapping(Heightmap, Position, TBN, UV, DETAIL_PARALLAX_START_FADE, DETAIL_PARALLAX_STOP_FADE);
#endif
    return UV;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // DISPLACEMENT_INCLUDED
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////