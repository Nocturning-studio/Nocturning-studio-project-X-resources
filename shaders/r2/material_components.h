////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Created: 27.06.2024
// Author: Deathman
// Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "normal_map_functions.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float3 GetViewVector(float3 Position, float3x3 TBN)
{
    TBN = transpose(TBN);
    return normalize(mul(TBN, -Position));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Material components
float3 GetAlbedo(float2 UV)
{
    return tex2D(s_base, UV);
}

float GetOpacity(float2 UV)
{
    return tex2D(s_base, UV).a;
}

float3 GetNormalMap(float2 UV)
{
#if USE_BUMP
    // DXT Compressed normals
    float2 CompressedNormalMap = tex2D(s_bump, UV).ab;

    // Decompressing data + 0.5f
    float2 DecompressingData = tex2D(s_bumpX, UV).rg;

    // Transform normal map color from [0; 1] space to [-1; 1]
    float2 PackedNormal = (CompressedNormalMap + (DecompressingData - 1.0f));

    // Reconstruct Z component
    return Unpack_Normal_Map(PackedNormal);
#elif USE_CUSTOM_NORMAL
    return tex2D(s_custom_normal, UV) * 2.0f - 1.0f;
#else
    return float3(0.0f, 0.0f, 1.0f);
#endif
}

float GetHeight(float2 UV)
{
#if USE_BUMP
    return tex2Dlod0(s_bumpX, UV).a;
#else
    return 0.5f;
#endif
}

float GetRoughness(float2 UV)
{
#if USE_BUMP
    return invert(tex2D(s_bump, UV).r);
#elif USE_CUSTOM_ROUGHNESS
    return tex2D(s_custom_roughness, UV).r;
#else
    return 1.0f;
#endif
}

float GetBakedAO(float2 UV)
{
    return tex2D(s_baked_ao, UV).r;
}

float3 GetDetailAlbedo(float2 UV)
{
    return tex2D(s_detail, UV);
}

float3 GetDetailNormal(float2 UV)
{
#if USE_DETAIL_BUMP
    // DXT Compressed normals
    float2 CompressedNormalMap = tex2D(s_detailBump, UV).ab;

    // Decompressing data + 0.5f
    float2 DecompressingData = tex2D(s_detailBumpX, UV).rg;

    // Transform normal map color from [0; 1] space to [-1; 1]
    float2 PackedNormal = (CompressedNormalMap + (DecompressingData - 1.0f));

    // Reconstruct Z component
    return Unpack_Normal_Map(PackedNormal);
#else
    return float3(0.0f, 0.0f, 1.0f);
#endif
}

float GetDetailRoughness(float2 UV)
{
#if USE_DETAIL_BUMP
    return invert(tex2D(s_detailBump, UV).r);
#else
    return 1.0f;
#endif
}

float GetDetailHeight(float2 UV)
{
#if USE_DETAIL_BUMP
    return tex2Dlod0(s_detailBumpX, UV).a;
#else
    return 0.5f;
#endif
}

float4 GetTerrainDetailMask(float2 UV)
{
    float4 Mask = tex2D(s_mask, UV);
    return Mask / dot(Mask, 1.0f);
}

float3 GetTerrainDetailAlbedo(float4 Mask, float2 UV)
{
    float3 RedChannelAlbedo = tex2D(s_dt_r, UV) * Mask.r;
    float3 GreenChannelAlbedo = tex2D(s_dt_g, UV) * Mask.g;
    float3 BlueChannelAlbedo = tex2D(s_dt_b, UV) * Mask.b;
    float3 AlphaChannelAlbedo = tex2D(s_dt_a, UV) * Mask.a;
    return RedChannelAlbedo + GreenChannelAlbedo + BlueChannelAlbedo + AlphaChannelAlbedo;
}

float4 GetTerrainDetailBump(float4 Mask, float2 UV)
{
    float4 RedChannelBump = tex2D(s_dn_r, UV) * Mask.r;
    float4 GreenChannelBump = tex2D(s_dn_g, UV) * Mask.g;
    float4 BlueChannelBump = tex2D(s_dn_b, UV) * Mask.b;
    float4 AlphaChannelBump = tex2D(s_dn_a, UV) * Mask.a;
    return RedChannelBump + GreenChannelBump + BlueChannelBump + AlphaChannelBump;
}

float4 GetTerrainDetailBumpDecompression(float4 Mask, float2 UV)
{
    float4 RedChannelBumpX = tex2D(s_dn_rX, UV) * Mask.r;
    float4 GreenChannelBumpX = tex2D(s_dn_gX, UV) * Mask.g;
    float4 BlueChannelBumpX = tex2D(s_dn_bX, UV) * Mask.b;
    float4 AlphaChannelBumpX = tex2D(s_dn_aX, UV) * Mask.a;
    return RedChannelBumpX + GreenChannelBumpX + BlueChannelBumpX + AlphaChannelBumpX;
}

float GetTerrainDetailHeight(float4 Mask, float2 UV)
{
    float RedChannelHeight = tex2Dlod0(s_dn_rX, UV).a * Mask.r;
    float GreenChannelHeight = tex2Dlod0(s_dn_gX, UV).a * Mask.g;
    float BlueChannelHeight = tex2Dlod0(s_dn_bX, UV).a * Mask.b;
    float AlphaChannelHeight = tex2Dlod0(s_dn_aX, UV).a * Mask.a;
    return RedChannelHeight + GreenChannelHeight + BlueChannelHeight + AlphaChannelHeight;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////