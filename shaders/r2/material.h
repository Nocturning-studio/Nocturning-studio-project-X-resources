////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Created: 06.08.2023
// Author: Deathman
// Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Deathman to all:
    Необходим рефакторинг с переработкой игровых материалов.
    Необходимо заменить glossiness на шероховатость, а
    содержимое синего канала bump# на металличность.
    Продумать хранение Emissive map.
    Сопутствующий ущерб - необходимо написать конвертер стандартных карт в новые материалы
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "displacement.h"
#include "height_map_to_normal.h"
#include "normal_map_blending.h"
#include "fxaa_atoc.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct MaterialParams
{
    float3 Albedo;
    float Opacity;
    float3 Normal;
    float Height;
    float Glossiness;
    float AO;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MaterialParams GetMaterial(float2 UV, float3x3 TBN, float3 Position)
{
    MaterialParams Material;

    // Update UV with displacement
#ifdef USE_BUMP
    UV = GetDisplacement(s_bumpX, Position, TBN, UV);
#endif

#if defined(USE_ALPHA_TEST) && (ALPHA_TEST_AA == FXAA_ATOC)
    // Get Albedo and opacity with fxaa
    calc_fxaa_atoc(UV, Material.Albedo, Material.Opacity);
#else//USE_ALPHA_TEST && (ALPHA_TEST_AA == FXAA_ATOC)
    float4 BaseTextureData = tex2D(s_base, UV);
    Material.Albedo = BaseTextureData.rgb;
    Material.Opacity = BaseTextureData.a;
#endif//USE_ALPHA_TEST && (ALPHA_TEST_AA == FXAA_ATOC)

#ifdef USE_BUMP
    // Get DXT comressed normals + AO + glossmap
    float4 NormalMapData = tex2D(s_bump, UV);

    // Get DXT decompressing data + heightmap
    float4 NormalMapDecompressionData = tex2D(s_bumpX, UV);

    // Transform normal map color from [0; 1] space to [-1; 1] with DXT decompressing
    Material.Normal = (NormalMapData.abg + (NormalMapDecompressionData.rgb - 1.0f));

    // Reconstruct z component for normals (actually z component used for Baked AO)
    Material.Normal.z = sqrt(1.0f - dot(Material.Normal.xy, Material.Normal.xy));

    Material.Height = NormalMapDecompressionData.a;

    Material.Glossiness = NormalMapData.r;
#else//USE_BUMP
    Material.Normal = float3(0.0f, 0.0f, 1.0f);
    Material.Height = 1.0f;
    Material.Glossiness = 0.0f;
#endif//USE_BUMP

#ifdef USE_BAKED_AO
    Material.AO = tex2D(s_baked_ao, UV).r;
#else
    Material.AO = 1.0f;
#endif

#ifdef USE_TDETAIL
    // Create detail UV - dt_params is scaler
    float2 DetailUV = UV * dt_params;

    // Calculate displacement for detail texture only if we have good quality for materials
#if USE_DETAIL_BUMP && BUMP_QUALITY >= MIDDLE_QUALITY
    DetailUV = GetDetailDisplacement(s_detailBumpX, Position, TBN, DetailUV);
#endif // DETAIL_BUMP && BUMP_QUALITY >= MIDDLE_QUALITY

    // Combine main albedo with detail
    float3 DetailAlbedo = tex2D(s_detail, DetailUV);
    Material.Albedo.rgb *= 2.0f * DetailAlbedo;

#ifdef USE_DETAIL_BUMP
    float4 DetailNormalMapData = tex2D(s_detailBump, DetailUV);
    float4 DetailNormalMapDecompressionData = tex2D(s_detailBumpX, DetailUV);

    // Transform normal map color from [0; 1] space to [-1; 1] with decompressing
    float3 DetailNormal = (DetailNormalMapData.abg + (DetailNormalMapDecompressionData.rgb - 1.0f));

    // Reconstruct z component for normals (actually z component used for Baked AO)
    DetailNormal.z = sqrt(1.0f - dot(DetailNormal.xy, DetailNormal.xy));

    // Correctly blend two maps
    Material.Normal = Blend_Normal_Maps(Material.Normal, DetailNormal);

    // Combine main height map with detail height map, multiplicated by coeffient, make detail height map influence is
    // smaller
    Material.Height += DetailNormalMapDecompressionData.a;

    Material.Glossiness = DetailNormalMapData.r * Material.Glossiness + Material.Glossiness;
#endif // DETAIL_BUMP
#endif // USE_TDETAIL

#if defined(USE_BUMP) || defined(USE_DETAIL_BUMP)
    // If we have good material quality - create MORE relief by convertation height to normal map
#if BUMP_QUALITY == ULTRA_QUALITY
    float3 NormalFromHeight = convert_height_to_normal(Material.Height);
    Material.Normal /= NormalFromHeight;
#endif

    // Transformate height map from [0; 1] space to [1; 2] and make normals more power
    Material.Normal.xy *= Material.Height + 1.0f;

    // Make normals more power with heightmap
    Material.Normal.z *= Material.Height;
#endif//defined(USE_BUMP) || defined(USE_DETAIL_BUMP)

    Material.Normal = normalize(Material.Normal);

    return Material;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
