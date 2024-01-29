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
#include "contrast_adaptive_sharpening.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef MATERIAL_INCLUDED
#define MATERIAL_INCLUDED
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct MaterialParams
{
    vector3 Albedo;
    vector Opacity;
    vector3 Normal;
    vector Height;
    vector Glossiness;
    vector AO;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MaterialParams GetMaterial(vector2 UV, matrix3x3 TBN, vector3 Position)
{
    MaterialParams Material;

    // We must get ao from texture before UV displacement for avoid ao bugs
    Material.AO = tex2D(s_baked_ao, UV).r;

    // Update UV with displacement
    UV = GetDisplacement(s_bumpX, Position, TBN, UV);

#if defined(USE_ALPHA_TEST) && (ALPHA_TEST_AA == FXAA_ATOC)
    // Get Albedo and opacity with fxaa
    calc_fxaa_atoc(UV, Material.Albedo, Material.Opacity);
#else//USE_ALPHA_TEST && (ALPHA_TEST_AA == FXAA_ATOC)
    vector4 BaseTextureData = tex2D(s_base, UV);
    Material.Albedo = BaseTextureData.rgb;
    Material.Opacity = BaseTextureData.a;
#endif//USE_ALPHA_TEST && (ALPHA_TEST_AA == FXAA_ATOC)

    // Get DXT comressed normals + AO + glossmap
    vector4 NormalMapData = tex2D(s_bump, UV);

    // Get DXT decompressing data + heightmap
    vector4 NormalMapDecompressionData = tex2D(s_bumpX, UV);

    // Transform normal map color from [0; 1] space to [-1; 1] with DXT decompressing
    Material.Normal = (NormalMapData.abg + (NormalMapDecompressionData.rgb - 1.0h));

    // Reconstruct z component for normals (actually z component used for Baked AO)
    Material.Normal.z = sqrt(1.0h - dot(Material.Normal.xy, Material.Normal.xy));

    Material.AO *= NormalMapData.g;

    Material.Height = NormalMapDecompressionData.a;

    Material.Glossiness = NormalMapData.r;

    // If for material used detail texture - we use this block with analogic code, but with detail textures
#ifdef USE_TDETAIL
    // Create detail UV - dt_params is scaler
    vector2 DetailUV = UV * dt_params;

    // Calculate displacement for detail texture only if we have good quality for materials
#if BUMP_QUALITY >= MIDDLE_QUALITY
    DetailUV = GetDetailDisplacement(s_detailBumpX, Position, TBN, DetailUV);
#endif

    // Get albedo, comressed normals + AO + glossmap, and decompressing data + heightmap
    vector3 DetailAlbedo = tex2D(s_detail, DetailUV);
    vector4 DetailNormalMapData = tex2D(s_detailBump, DetailUV);
    vector4 DetailNormalMapDecompressionData = tex2D(s_detailBumpX, DetailUV);

    // Transform normal map color from [0; 1] space to [-1; 1] with decompressing
    vector3 DetailNormal = (DetailNormalMapData.abg + (DetailNormalMapDecompressionData.rgb - 1.0h));

    // Reconstruct z component for normals (actually z component used for Baked AO)
    DetailNormal.z = sqrt(1.0h - dot(DetailNormal.xy, DetailNormal.xy));

    // Correctly blend two maps
    Material.Normal = Blend_Normal_Maps(Material.Normal, DetailNormal);

    // Combine main albedo with detail
    Material.Albedo.rgb *= 2.0h * DetailAlbedo;

    Material.AO *= DetailNormalMapData.g;

    // Combine main height map with detail height map, multiplicated by coeffient, make detail height map influence is
    // smaller
    Material.Height += DetailNormalMapDecompressionData.a * 0.1f;

    Material.Glossiness = DetailNormalMapData.r * Material.Glossiness + Material.Glossiness;
#endif // USE_TDETAIL

    // If we have good material quality - create MORE relief by convertation height to normal map
#if BUMP_QUALITY == ULTRA_QUALITY
    vector3 NormalFromHeight = convert_height_to_normal(Material.Height);
    Material.Normal /= NormalFromHeight;
#endif

    // Transformate height map from [0; 1] space to [1; 2] and make normals more power
    Material.Normal.xy *= Material.Height + 1.0h;

    // Make normals more power with inverted heightmap
    Material.Normal.z *= invert(Material.Height);
    
    Material.Normal = normalize(Material.Normal);

    return Material;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // MATERIAL_INCLUDED
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
