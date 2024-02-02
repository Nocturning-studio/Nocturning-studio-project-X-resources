////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Created: 09.12.2022
// Author: Deathman
// Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef TERRAIN_FUNCTIONS_INCLUDED
#define TERRAIN_FUNCTIONS_INCLUDED
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "contrast_adaptive_sharpening.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constant table
#define TERRAIN_PARALLAX_H 0.02f
#define constant_terrain_parallax_scale float2(TERRAIN_PARALLAX_H, -TERRAIN_PARALLAX_H * 0.5f)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Quality
#if BUMP_QUALITY == UNDEFINED_QUALITY
#define TERRAIN_STEEP_PARALLAX_START_FADE 8
#define TERRAIN_STEEP_PARALLAX_STOP_FADE 12
#define TERRAIN_PARALLAX_START_FADE 15
#define TERRAIN_PARALLAX_STOP_FADE 20
#define TERRAIN_STEEP_PARALLAX_MIN_SAMPLES 3
#define TERRAIN_STEEP_PARALLAX_MAX_SAMPLES 5
#elif BUMP_QUALITY == LOW_QUALITY
#define TERRAIN_STEEP_PARALLAX_START_FADE 10
#define TERRAIN_STEEP_PARALLAX_STOP_FADE 15
#define TERRAIN_PARALLAX_START_FADE 15
#define TERRAIN_PARALLAX_STOP_FADE 20
#define TERRAIN_STEEP_PARALLAX_MIN_SAMPLES 5
#define TERRAIN_STEEP_PARALLAX_MAX_SAMPLES 10
#elif BUMP_QUALITY == MIDDLE_QUALITY
#define TERRAIN_STEEP_PARALLAX_START_FADE 12
#define TERRAIN_STEEP_PARALLAX_STOP_FADE 17
#define TERRAIN_PARALLAX_START_FADE 17
#define TERRAIN_PARALLAX_STOP_FADE 22
#define TERRAIN_STEEP_PARALLAX_MIN_SAMPLES 6
#define TERRAIN_STEEP_PARALLAX_MAX_SAMPLES 12
#elif BUMP_QUALITY == HIGHT_QUALITY
#define TERRAIN_STEEP_PARALLAX_START_FADE 15
#define TERRAIN_STEEP_PARALLAX_STOP_FADE 20
#define TERRAIN_PARALLAX_START_FADE 20
#define TERRAIN_PARALLAX_STOP_FADE 25
#define TERRAIN_STEEP_PARALLAX_MIN_SAMPLES 8
#define TERRAIN_STEEP_PARALLAX_MAX_SAMPLES 14
#elif BUMP_QUALITY == ULTRA_QUALITY
#define TERRAIN_STEEP_PARALLAX_START_FADE 20
#define TERRAIN_STEEP_PARALLAX_STOP_FADE 25
#define TERRAIN_PARALLAX_START_FADE 25
#define TERRAIN_PARALLAX_STOP_FADE 30
#define TERRAIN_STEEP_PARALLAX_MIN_SAMPLES 12
#define TERRAIN_STEEP_PARALLAX_MAX_SAMPLES 17
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float3 GetEyefloat(float3 Position, float3x3 TBN)
{
    TBN = transpose(TBN);
    return normalize(mul(TBN, -Position));
}

float4 GetDetailMask(float2 UV)
{
    float4 Mask = tex2D(s_mask, UV);
    return Mask / dot(Mask, 1.0h);
}

float3 GetDetailAlbedoWithCAS(float4 Mask, float2 UV)
{
    float3 RedChannelAlbedo = CAS(s_dt_r, UV, 0.05h, 0.2h) * Mask.r;
    float3 GreenChannelAlbedo = CAS(s_dt_g, UV, 0.05h, 0.2h) * Mask.g;
    float3 BlueChannelAlbedo = CAS(s_dt_b, UV, 0.05h, 0.2h) * Mask.b;
    float3 AlphaChannelAlbedo = CAS(s_dt_a, UV, 0.05h, 0.2h) * Mask.a;
    return RedChannelAlbedo + GreenChannelAlbedo + BlueChannelAlbedo + AlphaChannelAlbedo;
}

float3 GetDetailAlbedo(float4 Mask, float2 UV)
{
    float3 RedChannelAlbedo = tex2D(s_dt_r, UV) * Mask.r;
    float3 GreenChannelAlbedo = tex2D(s_dt_g, UV) * Mask.g;
    float3 BlueChannelAlbedo = tex2D(s_dt_b, UV) * Mask.b;
    float3 AlphaChannelAlbedo = tex2D(s_dt_a, UV) * Mask.a;
    return RedChannelAlbedo + GreenChannelAlbedo + BlueChannelAlbedo + AlphaChannelAlbedo;
}

float4 GetDetailBump(float4 Mask, float2 UV)
{
    float4 RedChannelBump = tex2D(s_dn_r, UV) * Mask.r;
    float4 GreenChannelBump = tex2D(s_dn_g, UV) * Mask.g;
    float4 BlueChannelBump = tex2D(s_dn_b, UV) * Mask.b;
    float4 AlphaChannelBump = tex2D(s_dn_a, UV) * Mask.a;
    return RedChannelBump + GreenChannelBump + BlueChannelBump + AlphaChannelBump;
}

float4 GetDetailBumpDecompression(float4 Mask, float2 UV)
{
    float4 RedChannelBumpX = tex2D(s_dn_rX, UV) * Mask.r;
    float4 GreenChannelBumpX = tex2D(s_dn_gX, UV) * Mask.g;
    float4 BlueChannelBumpX = tex2D(s_dn_bX, UV) * Mask.b;
    float4 AlphaChannelBumpX = tex2D(s_dn_aX, UV) * Mask.a;
    return RedChannelBumpX + GreenChannelBumpX + BlueChannelBumpX + AlphaChannelBumpX;
}

float GetDetailHeight(float4 Mask, float2 UV)
{
    float RedChannelHeight = tex2Dlod0(s_dn_rX, UV).a * Mask.r;
    float GreenChannelHeight = tex2Dlod0(s_dn_gX, UV).a * Mask.g;
    float BlueChannelHeight = tex2Dlod0(s_dn_bX, UV).a * Mask.b;
    float AlphaChannelHeight = tex2Dlod0(s_dn_aX, UV).a * Mask.a;
    return RedChannelHeight + GreenChannelHeight + BlueChannelHeight + AlphaChannelHeight;
}

float2 CalculateDetailParallaxMapping(float3 Position, float3x3 TBN, float2 UV, float4 Mask)
{
    float3 eye = GetEyefloat(Position, TBN);
    if (Position.z < TERRAIN_PARALLAX_STOP_FADE)
    {
        float height = GetDetailHeight(Mask, UV);
        height *= constant_terrain_parallax_scale.x + constant_terrain_parallax_scale.y;
        float fParallaxFade = smoothstep(TERRAIN_PARALLAX_STOP_FADE, TERRAIN_PARALLAX_START_FADE, Position.z);
        UV += height * eye * fParallaxFade;
    }
    return UV;
}

float2 CalculateDetailSteepParallaxOcclusionMapping(float3 Position, float3x3 TBN, float2 UV, float4 Mask)
{
    if (Position.z < TERRAIN_STEEP_PARALLAX_STOP_FADE)
    {
        float3 eye = GetEyefloat(Position, TBN);

        // Calculate number of steps
        float nNumSteps = lerp(TERRAIN_STEEP_PARALLAX_MAX_SAMPLES, TERRAIN_STEEP_PARALLAX_MIN_SAMPLES, eye.z);

        float fStepSize = 1.0h / nNumSteps;
        float2 vDelta = -eye.xy * constant_terrain_parallax_scale.x * 1.2f;
        float2 vTexOffsetPerStep = fStepSize * vDelta;

        // Prepare start data for cycle
        float2 vTexCurrentOffset = UV;
        float fCurrHeight = 0.0h;
        float fCurrentBound = 1.0h;

        for (; fCurrHeight < fCurrentBound; fCurrentBound -= fStepSize)
        {
            vTexCurrentOffset += vTexOffsetPerStep;
            fCurrHeight = GetDetailHeight(Mask, vTexCurrentOffset);
        }

        // Reconstruct previouse step's data
        vTexCurrentOffset -= vTexOffsetPerStep;
        float fPrevHeight = GetDetailHeight(Mask, vTexCurrentOffset);

        // Smooth tc position between current and previouse step
        float fDelta2 = ((fCurrentBound + fStepSize) - fPrevHeight);
        float fDelta1 = (fCurrentBound - fCurrHeight);
        float fParallaxAmount =
            (fCurrentBound * fDelta2 - (fCurrentBound + fStepSize) * fDelta1) / (fDelta2 - fDelta1);
        float fParallaxFade =
            smoothstep(TERRAIN_STEEP_PARALLAX_STOP_FADE, TERRAIN_STEEP_PARALLAX_START_FADE, Position.z);
        float2 vParallaxOffset = vDelta * ((1 - fParallaxAmount) * fParallaxFade);

        UV += vParallaxOffset;
    }

    return UV;
}

float2 GetDisplacement(float3 Position, float3x3 TBN, float2 UV, float4 Mask)
{
#if defined(USE_TERRAIN_PARALLAX_MAPPING)
    UV = CalculateDetailParallaxMapping(Position, TBN, UV, Mask);
#elif defined(USE_TERRAIN_STEEP_PARALLAX_MAPPING)
    UV = CalculateDetailSteepParallaxOcclusionMapping(Position, TBN, UV, Mask);
#endif
    return UV;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // TERRAIN_FUNCTIONS_INCLUDED
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
