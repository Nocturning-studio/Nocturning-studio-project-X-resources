////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Created: 09.12.2022
// Author: Deathman
// Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constant table
#define TERRAIN_PARALLAX_H 0.02f
#define constant_terrain_parallax_scale float2(TERRAIN_PARALLAX_H, -TERRAIN_PARALLAX_H / 2)
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
#define TERRAIN_STEEP_PARALLAX_START_FADE 8
#define TERRAIN_STEEP_PARALLAX_STOP_FADE 12
#define TERRAIN_PARALLAX_START_FADE 15
#define TERRAIN_PARALLAX_STOP_FADE 20
#define TERRAIN_STEEP_PARALLAX_MIN_SAMPLES 5
#define TERRAIN_STEEP_PARALLAX_MAX_SAMPLES 10
#elif BUMP_QUALITY == MIDDLE_QUALITY
#define TERRAIN_STEEP_PARALLAX_START_FADE 10
#define TERRAIN_STEEP_PARALLAX_STOP_FADE 15
#define TERRAIN_PARALLAX_START_FADE 17
#define TERRAIN_PARALLAX_STOP_FADE 22
#define TERRAIN_STEEP_PARALLAX_MIN_SAMPLES 8
#define TERRAIN_STEEP_PARALLAX_MAX_SAMPLES 14
#elif BUMP_QUALITY == HIGHT_QUALITY
#define TERRAIN_STEEP_PARALLAX_START_FADE 12
#define TERRAIN_STEEP_PARALLAX_STOP_FADE 17
#define TERRAIN_PARALLAX_START_FADE 20
#define TERRAIN_PARALLAX_STOP_FADE 25
#define TERRAIN_STEEP_PARALLAX_MIN_SAMPLES 10
#define TERRAIN_STEEP_PARALLAX_MAX_SAMPLES 17
#elif BUMP_QUALITY == ULTRA_QUALITY
#define TERRAIN_STEEP_PARALLAX_START_FADE 15
#define TERRAIN_STEEP_PARALLAX_STOP_FADE 20
#define TERRAIN_PARALLAX_START_FADE 25
#define TERRAIN_PARALLAX_STOP_FADE 30
#define TERRAIN_STEEP_PARALLAX_MIN_SAMPLES 15
#define TERRAIN_STEEP_PARALLAX_MAX_SAMPLES 20
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float3 GetViewVector(float3 Position, float3x3 TBN)
{
    TBN = transpose(TBN);
    return normalize(mul(TBN, -Position));
}

float4 GetDetailMask(float2 UV)
{
    float4 Mask = tex2D(s_mask, UV);
    return Mask / dot(Mask, 1.0h);
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
    if (Position.z < TERRAIN_PARALLAX_STOP_FADE)
    {
        float3 ViewVector = GetViewVector(Position, TBN);

        float height = GetDetailHeight(Mask, UV);
        height *= constant_terrain_parallax_scale.x;
        height += constant_terrain_parallax_scale.y;

        float fParallaxFade = smoothstep(TERRAIN_STEEP_PARALLAX_STOP_FADE, TERRAIN_STEEP_PARALLAX_START_FADE, Position.z);
        UV += height * ViewVector * fParallaxFade;
    }

    return UV;
}

float2 CalculateDetailParallaxOcclusionMapping(float3 Position, float3x3 TBN, float2 UV, float4 Mask)
{
    if (Position.z < TERRAIN_STEEP_PARALLAX_STOP_FADE)
    {
        float3 ViewVector = GetViewVector(Position, TBN);

        // Calculate number of steps
        float nNumSteps = lerp(TERRAIN_STEEP_PARALLAX_MAX_SAMPLES, TERRAIN_STEEP_PARALLAX_MIN_SAMPLES, ViewVector.z);

        float fStepSize = 1.0h / nNumSteps;
        float2 vDelta = -ViewVector.xy * constant_terrain_parallax_scale.x;
        float2 vTexOffsetPerStep = fStepSize * vDelta;

        // Prepare start data for cycle
        float2 vTexCurrentOffset = UV;
        float fCurrHeight = 0.0h;
        float fCurrentBound = 1.0h;

        for (; fCurrHeight < fCurrentBound; fCurrentBound -= fStepSize)
        {
            vTexCurrentOffset += vTexOffsetPerStep;
            float HeightMap = GetDetailHeight(Mask, vTexCurrentOffset.xy);
            fCurrHeight = HeightMap;
        }

        // Reconstruct previouse step's data
        vTexCurrentOffset -= vTexOffsetPerStep;
        float fPrevHeight = GetDetailHeight(Mask, vTexCurrentOffset.xy);

        // Smooth tc position between current and previouse step
        float fDelta2 = ((fCurrentBound + fStepSize) - fPrevHeight);
        float fDelta1 = (fCurrentBound - fCurrHeight);
        float fParallaxAmount = (fCurrentBound * fDelta2 - (fCurrentBound + fStepSize) * fDelta1) / (fDelta2 - fDelta1);
        float fParallaxFade = smoothstep(TERRAIN_STEEP_PARALLAX_STOP_FADE, TERRAIN_STEEP_PARALLAX_START_FADE, Position.z);

        UV += vDelta * ((1.0h - fParallaxAmount) * fParallaxFade);
    }

    return UV;
}

float2 GetDisplacement(float3 Position, float3x3 TBN, float2 UV, float4 Mask)
{
#if defined(USE_PARALLAX_MAPPING)
    UV = CalculateDetailParallaxMapping(Position, TBN, UV, Mask);
#elif defined(USE_STEEP_PARALLAX_MAPPING)
    UV = CalculateDetailParallaxOcclusionMapping(Position, TBN, UV, Mask);
#endif
    return UV;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
