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
/*
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
*/
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
// https://www.shadertoy.com/view/WscyRl
////////////////////////////////////////////////////////////////////////////
float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return a2 / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

float3 fresnelSchlick(float3 F0, float cosTheta)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float3 fresnelSchlickRoughness(float3 F0, float cosTheta, float roughness)
{
    return F0 + (max(1.0 - roughness, F0) - F0) * pow(1.0 - cosTheta, 5.0);
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
    float RoughnessSqr = pow(Roughness, 2.0f);

    // Oren-Nayar diffuse model
    float s = VdotL - NdotL * NdotV;
    float t = lerp(NdotL, min(1, NdotL / NdotV), step(0.0f, s));
    float Diffuse = NdotL * ((1.0f - 0.5f * RoughnessSqr / (RoughnessSqr + 0.33f)) + (0.45f * RoughnessSqr / (RoughnessSqr + 0.09f) * s * t));

    float3 F0 = 0.04;
    //F0 = mix(F0, albedo, metalness);

    float D = DistributionGGX(Normal, HalfWay, Roughness);
    float3 F = fresnelSchlick(F0, HdotV);
    float G = GeometrySmith(Normal, ViewDirection, LightDirection, Roughness);

    float3 numerator = (D * G) * F;
    float denominator = 4.0 * NdotV * NdotL;

    float Specular = numerator / max(denominator, 0.001f);

    Light.Diffuse = Diffuse;
    Light.Specular = Specular;

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
