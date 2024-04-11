////////////////////////////////////////////////////////////////////////////
//	Created		: 20.11.2023
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
#include "common.h"
////////////////////////////////////////////////////////////////////////////
// Более оптимизированная версия, с тем же результатом, так как все вычисления уже отрендерены в текстуру и правильным
// F0 (Наверное)
// https://github.com/Zackin5/StalkerCop-FxaaShaders + https://learnopengl.com/PBR/IBL/Specular-IBL +
// https://www.shadertoy.com/view/3tlBW7
////////////////////////////////////////////////////////////////////////////
uniform sampler2D s_brdf_lut;
////////////////////////////////////////////////////////////////////////////
float3 fresnelSchlickRoughness(float NdotL, float3 F0, float roughness)
{
    return F0 + (max((1.0 - roughness), F0) - F0) * pow(1.0 - NdotL, 5.0);
}

float3 EnvironmentBRDF_LUT(float3 Point, float3 Normal, float Roughness)
{
    float3 v = -normalize(Point);
    float vDotN = saturate(dot(Normal, v));

    // Index of refraction for common dielectrics. Corresponds to f0 4%
    float IOR = 1.5f;
    // Reflectance of the surface when looking straight at it along the negative normal
    float F0 = float(pow(IOR - 1.0f, 2.0f) / pow(IOR + 1.0f, 2.0f));
    // F0 = lerp(F0, 1.0f, Metalness);

    float3 F = fresnelSchlickRoughness(vDotN, F0.xxx, Roughness);
    float2 brdf = tex2D(s_brdf_lut, float2(vDotN, Roughness));
    return F * (brdf.x + brdf.y);
}
////////////////////////////////////////////////////////////////////////////
float Fresnel(float Specular, float3 ViewDirection, float3 floatAngle)
{
    return (Specular + (1.0f - Specular) * pow(1.0f - saturate(dot(ViewDirection, floatAngle)), 5.0f));
}
////////////////////////////////////////////////////////////////////////////
float Blinn_Phong_Specular(float3 floatAngle, float3 Normal)
{
    float Specular = max(0.0f, dot(floatAngle, Normal));

    return pow(Specular, 16.0f);
}
////////////////////////////////////////////////////////////////////////////
//https://habr.com/ru/articles/326852/
////////////////////////////////////////////////////////////////////////////
#define F0 float3(1.0, 0.86, 0.56)
////////////////////////////////////////////////////////////////////////////
float GGX_PartialGeometry(float cosThetaN, float alpha)
{
    float cosTheta_sqr = saturate(cosThetaN * cosThetaN);
    float tan2 = (1 - cosTheta_sqr) / cosTheta_sqr;
    float GP = 2 / (1 + sqrt(1 + alpha * alpha * tan2));
    return GP;
}

float GGX_Distribution(float cosThetaNH, float alpha)
{
    float alpha2 = alpha * alpha;
    float NH_sqr = saturate(cosThetaNH * cosThetaNH);
    float den = NH_sqr * alpha2 + (1.0 - NH_sqr);
    return alpha2 / (PI * den * den);
}

float3 FresnelSchlick(float3 f0, float cosTheta)
{
    return f0 + (1.0 - f0) * pow(1.0 - saturate(cosTheta), 5.0);
}

float3 CookTorrance_GGX(float3 n, float3 l, float3 v, float Roughness)
{
    n = normalize(n);
    v = normalize(v);
    l = normalize(l);
    float3 h = normalize(v + l);
    //precompute dots
    float NL = saturate(dot(n, l));
    float NV = abs(dot(n, v)) + 0.1f;
    float NH = dot(n, h);
    float HV = dot(h, v);

    //precompute roughness square
    float roug_sqr = pow(Roughness, 2.0f);

    //calc coefficients
    float G = GGX_PartialGeometry(NV, roug_sqr) * GGX_PartialGeometry(NL, roug_sqr);
    float D = GGX_Distribution(NH, roug_sqr);
    float3 F = FresnelSchlick(F0, HV);

    //mix
    float3 specK = G * D * F * 0.25f / NV;
    return saturate(specK);
}
////////////////////////////////////////////////////////////////////////////
// https://www.shadertoy.com/view/ltfyD8
// https://www.shadertoy.com/view/Wt23zt
////////////////////////////////////////////////////////////////////////////
float Oren_Nayar_Diffuse(float3 LightDirection, float3 ViewDirection, float3 Normal, float Roughness)
{
    float NdotL = saturate(dot(Normal, LightDirection));
    float VdotL = dot(ViewDirection, LightDirection);
    float NdotV = abs(dot(Normal, ViewDirection)) + 0.1f;
    float RoughnessSqr = pow(Roughness, 2.0f);

    float s = VdotL - NdotL * NdotV;
    float t = lerp(NdotL, min(1, NdotL / NdotV), step(0.0f, s));
    return NdotL * ((1.0f - 0.5f * RoughnessSqr / (RoughnessSqr + 0.33f)) +
                    (0.45f * RoughnessSqr / (RoughnessSqr + 0.09f) * s * t));
}
////////////////////////////////////////////////////////////////////////////
// Ultimate lighting model
////////////////////////////////////////////////////////////////////////////
struct LightComponents
{
    float Diffuse;
    float Specular;
};

LightComponents Calculate_Lighting_Model(float Roughness, float3 Point, float3 Normal, float AO, float3 LightDirection)
{
    LightComponents Light;

    Normal = normalize(Normal);
    LightDirection = -normalize(LightDirection);
    float3 ViewDirection = -normalize(Point);
    float3 HalfWay = normalize(ViewDirection + LightDirection);
    float NdotL = saturate(dot(Normal, LightDirection));
    float VdotL = dot(ViewDirection, LightDirection);
    float NdotV = abs(dot(Normal, ViewDirection)) + 0.1f;
    float NdotH = dot(Normal, HalfWay);
    float HdotV = dot(HalfWay, ViewDirection);
    float RoughnessSqr = pow(Roughness, 1.0f);
    float BakedAO = (AO + 1.0f) * 0.5f;

    // Oren-Nayar diffuse model
    float s = VdotL - NdotL * NdotV;
    float t = lerp(NdotL, min(1, NdotL / NdotV), step(0.0f, s));
    float Diffuse = NdotL * ((1.0f - 0.5f * RoughnessSqr / (RoughnessSqr + 0.33f)) + (0.45f * RoughnessSqr / (RoughnessSqr + 0.09f) * s * t));

    // Cook-Torrance GGX Specular model
    float G = GGX_PartialGeometry(NdotV, RoughnessSqr) * GGX_PartialGeometry(NdotL, RoughnessSqr);
    float D = GGX_Distribution(NdotH, RoughnessSqr);
    float3 F = FresnelSchlick(F0 * BakedAO, HdotV);
    float3 Specular = saturate(G * D * F * 0.25f / NdotV);

    Light.Diffuse = Diffuse * BakedAO;
    Light.Specular = pow(Specular, 1.5f);

    return Light;
}
////////////////////////////////////////////////////////////////////////////
// Spot\Point lighting
////////////////////////////////////////////////////////////////////////////
float CalculateAttenuation(float3 Point, float3 LightPosition, float LightSourceRange)
{
    // Calculate standard X-Ray Engine light attenuation
    float3 LightDirection = Point - LightPosition;
    float LightRadiusSquared = dot(LightDirection, LightDirection);
    float AttenuationFactor = saturate(1.0f - LightRadiusSquared * LightSourceRange);

    // Make attenuation more realistic - light brightness falls down with pow 2.2
    //  Need work with lighting on game levels
    // AttenuationFactor = pow(AttenuationFactor, 2.2f);

    return AttenuationFactor;
}
////////////////////////////////////////////////////////////////////////////
