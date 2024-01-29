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
#define constant_terrain_parallax_scale vector2(TERRAIN_PARALLAX_H, -TERRAIN_PARALLAX_H * 0.5f)
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
vector3 GetEyeVector(vector3 Position, matrix3x3 TBN)
{
    TBN = transpose(TBN);
    return normalize(mul(TBN, -Position));
}

vector4 GetDetailMask(vector2 UV)
{
    vector4 Mask = tex2D(s_mask, UV);
    return Mask / dot(Mask, 1.0h);
}

vector3 GetDetailAlbedoWithCAS(vector4 Mask, vector2 UV)
{
    vector3 RedChannelAlbedo = CAS(s_dt_r, UV, 0.05h, 0.2h) * Mask.r;
    vector3 GreenChannelAlbedo = CAS(s_dt_g, UV, 0.05h, 0.2h) * Mask.g;
    vector3 BlueChannelAlbedo = CAS(s_dt_b, UV, 0.05h, 0.2h) * Mask.b;
    vector3 AlphaChannelAlbedo = CAS(s_dt_a, UV, 0.05h, 0.2h) * Mask.a;
    return RedChannelAlbedo + GreenChannelAlbedo + BlueChannelAlbedo + AlphaChannelAlbedo;
}

vector3 GetDetailAlbedo(vector4 Mask, vector2 UV)
{
    vector3 RedChannelAlbedo = tex2D(s_dt_r, UV) * Mask.r;
    vector3 GreenChannelAlbedo = tex2D(s_dt_g, UV) * Mask.g;
    vector3 BlueChannelAlbedo = tex2D(s_dt_b, UV) * Mask.b;
    vector3 AlphaChannelAlbedo = tex2D(s_dt_a, UV) * Mask.a;
    return RedChannelAlbedo + GreenChannelAlbedo + BlueChannelAlbedo + AlphaChannelAlbedo;
}

vector4 GetDetailBump(vector4 Mask, vector2 UV)
{
    vector4 RedChannelBump = tex2D(s_dn_r, UV) * Mask.r;
    vector4 GreenChannelBump = tex2D(s_dn_g, UV) * Mask.g;
    vector4 BlueChannelBump = tex2D(s_dn_b, UV) * Mask.b;
    vector4 AlphaChannelBump = tex2D(s_dn_a, UV) * Mask.a;
    return RedChannelBump + GreenChannelBump + BlueChannelBump + AlphaChannelBump;
}

vector4 GetDetailBumpDecompression(vector4 Mask, vector2 UV)
{
    vector4 RedChannelBumpX = tex2D(s_dn_rX, UV) * Mask.r;
    vector4 GreenChannelBumpX = tex2D(s_dn_gX, UV) * Mask.g;
    vector4 BlueChannelBumpX = tex2D(s_dn_bX, UV) * Mask.b;
    vector4 AlphaChannelBumpX = tex2D(s_dn_aX, UV) * Mask.a;
    return RedChannelBumpX + GreenChannelBumpX + BlueChannelBumpX + AlphaChannelBumpX;
}

vector GetDetailHeight(vector4 Mask, vector2 UV)
{
    vector RedChannelHeight = tex2Dlod0(s_dn_rX, UV).a * Mask.r;
    vector GreenChannelHeight = tex2Dlod0(s_dn_gX, UV).a * Mask.g;
    vector BlueChannelHeight = tex2Dlod0(s_dn_bX, UV).a * Mask.b;
    vector AlphaChannelHeight = tex2Dlod0(s_dn_aX, UV).a * Mask.a;
    return RedChannelHeight + GreenChannelHeight + BlueChannelHeight + AlphaChannelHeight;
}

vector2 CalculateDetailParallaxMapping(vector3 Position, matrix3x3 TBN, vector2 UV, vector4 Mask)
{
    vector3 eye = GetEyeVector(Position, TBN);
    if (Position.z < TERRAIN_PARALLAX_STOP_FADE)
    {
        vector height = GetDetailHeight(Mask, UV);
        height *= constant_terrain_parallax_scale.x + constant_terrain_parallax_scale.y;
        vector fParallaxFade = smoothstep(TERRAIN_PARALLAX_STOP_FADE, TERRAIN_PARALLAX_START_FADE, Position.z);
        UV += height * eye * fParallaxFade;
    }
    return UV;
}

vector2 CalculateDetailSteepParallaxOcclusionMapping(vector3 Position, matrix3x3 TBN, vector2 UV, vector4 Mask)
{
    if (Position.z < TERRAIN_STEEP_PARALLAX_STOP_FADE)
    {
        vector3 eye = GetEyeVector(Position, TBN);

        // Calculate number of steps
        vector nNumSteps = lerp(TERRAIN_STEEP_PARALLAX_MAX_SAMPLES, TERRAIN_STEEP_PARALLAX_MIN_SAMPLES, eye.z);

        vector fStepSize = 1.0h / nNumSteps;
        vector2 vDelta = -eye.xy * constant_terrain_parallax_scale.x * 1.2f;
        vector2 vTexOffsetPerStep = fStepSize * vDelta;

        // Prepare start data for cycle
        vector2 vTexCurrentOffset = UV;
        vector fCurrHeight = 0.0h;
        vector fCurrentBound = 1.0h;

        for (; fCurrHeight < fCurrentBound; fCurrentBound -= fStepSize)
        {
            vTexCurrentOffset += vTexOffsetPerStep;
            fCurrHeight = GetDetailHeight(Mask, vTexCurrentOffset);
        }

        // Reconstruct previouse step's data
        vTexCurrentOffset -= vTexOffsetPerStep;
        vector fPrevHeight = GetDetailHeight(Mask, vTexCurrentOffset);

        // Smooth tc position between current and previouse step
        vector fDelta2 = ((fCurrentBound + fStepSize) - fPrevHeight);
        vector fDelta1 = (fCurrentBound - fCurrHeight);
        vector fParallaxAmount =
            (fCurrentBound * fDelta2 - (fCurrentBound + fStepSize) * fDelta1) / (fDelta2 - fDelta1);
        vector fParallaxFade =
            smoothstep(TERRAIN_STEEP_PARALLAX_STOP_FADE, TERRAIN_STEEP_PARALLAX_START_FADE, Position.z);
        vector2 vParallaxOffset = vDelta * ((1 - fParallaxAmount) * fParallaxFade);

        UV += vParallaxOffset;
    }

    return UV;
}

vector2 GetDisplacement(vector3 Position, matrix3x3 TBN, vector2 UV, vector4 Mask)
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
