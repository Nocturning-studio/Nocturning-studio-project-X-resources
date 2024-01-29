////////////////////////////////////////////////////////////////////////////
//	Created		: 20.11.2023
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
#ifndef BRDF_INCLUDED
#define BRDF_INCLUDED
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
vector3 fresnelSchlickRoughness(vector NdotL, vector3 F0, vector roughness)
{
    return F0 + (max((1.0 - roughness), F0) - F0) * pow(1.0 - NdotL, 5.0);
}

vector3 EnvironmentBRDF_LUT(vector3 Point, vector3 Normal, vector Glossiness)
{
    vector3 v = -normalize(Point);
    vector vDotN = saturate(dot(Normal, v));

    // Index of refraction for common dielectrics. Corresponds to f0 4%
    float IOR = 1.5h;
    // Reflectance of the surface when looking straight at it along the negative normal
    vector F0 = vector(pow(IOR - 1.0h, 2.0h) / pow(IOR + 1.0h, 2.0h));
    // F0 = lerp(F0, 1.0h, Metalness);

    vector roughness = 1.0h - Glossiness;
    vector3 F = fresnelSchlickRoughness(vDotN, F0.xxx, roughness);
    vector2 brdf = tex2D(s_brdf_lut, vector2(vDotN, roughness));
    return F * (brdf.x + brdf.y);
}
////////////////////////////////////////////////////////////////////////////
//  https://learnopengl.com/Advanced-Lighting/Advanced-Lighting
////////////////////////////////////////////////////////////////////////////
vector Calculate_Energy_Conservation(vector Specular)
{
    const vector kShininess = 16.0h;
    const vector kEnergyConservation = (8.0h + kShininess) / (8.0h * PI);
    return pow(Specular, kEnergyConservation);
}
////////////////////////////////////////////////////////////////////////////
vector Fresnel(vector Specular, vector3 ViewDirection, vector3 HalfAngle)
{
    return (Specular + (1.0h - Specular) * pow(1.0h - saturate(dot(ViewDirection, HalfAngle)), 5.0h));
}
////////////////////////////////////////////////////////////////////////////
vector Blinn_Phong_Specular(vector3 HalfAngle, vector3 Normal)
{
    vector Specular = max(0.0h, dot(HalfAngle, Normal));

    Specular = pow(Specular, 24.0h);

    Specular = Calculate_Energy_Conservation(Specular);

    return Specular;
}
////////////////////////////////////////////////////////////////////////////
// https://www.shadertoy.com/view/ltfyD8
// https://www.shadertoy.com/view/Wt23zt
////////////////////////////////////////////////////////////////////////////
vector Oren_Nayar_Diffuse(vector3 LightDirection, vector3 ViewDirection, vector3 Normal, vector Glossiness)
{
    vector NdotL = saturate(dot(Normal, LightDirection));
    vector VdotL = dot(ViewDirection, LightDirection);
    vector NdotV = abs(dot(Normal, ViewDirection)) + 0.1f;
    vector RoughnessSqr = pow(1.0h - Glossiness, 2);

    vector s = VdotL - NdotL * NdotV;
    vector t = lerp(NdotL, min(1, NdotL / NdotV), step(0.0h, s));
    return NdotL * ((1.0h - 0.5f * RoughnessSqr / (RoughnessSqr + 0.33f)) +
                    (0.45f * RoughnessSqr / (RoughnessSqr + 0.09f) * s * t));
}
////////////////////////////////////////////////////////////////////////////
// Ultimate lighting model
////////////////////////////////////////////////////////////////////////////
vector4 Calculate_Lighting_Model(vector Material, vector Glossiness, vector3 Point, vector3 Normal,
                                 vector3 LightDirection)
{
    Normal = normalize(Normal);
    LightDirection = -normalize(LightDirection);
    vector3 ViewDirection = -normalize(Point);
    vector VdotN = max(0.0h, dot(ViewDirection, Normal));
    vector NdotL = max(0.0h, dot(LightDirection, Normal));
    vector3 HalfAngle = normalize(LightDirection + ViewDirection);

    // --Indirect specular from light--
    vector Specular = Blinn_Phong_Specular(HalfAngle, Normal);
    Specular = Fresnel(Specular, ViewDirection, HalfAngle) * NdotL * Glossiness;

    // --Indirect diffuse from light--
    vector Diffuse = Oren_Nayar_Diffuse(LightDirection, ViewDirection, Normal, Glossiness);
    Diffuse = Calculate_Energy_Conservation(Diffuse);

    return vector4(Diffuse.xxx, Specular);
    // vector4 LightingModel = tex3D(s_material, vector3(Diffuse, Specular, Material));
    //  return LightingModel;
}
////////////////////////////////////////////////////////////////////////////
// Spot\Point lighting
////////////////////////////////////////////////////////////////////////////
vector CalculateAttenuation(vector3 Point, vector3 LightPosition, vector LightSourceRange)
{
    // Calculate standard X-Ray Engine light attenuation
    vector3 LightDirection = Point - LightPosition;
    vector LightRadiusSquared = dot(LightDirection, LightDirection);
    vector AttenuationFactor = saturate(1.0h - LightRadiusSquared * LightSourceRange);

    // Make attenuation more realistic - light brightness falls down with pow 2.2
    //  Need work with lighting on game levels
    // AttenuationFactor = pow(AttenuationFactor, 2.2f);

    return AttenuationFactor;
}
////////////////////////////////////////////////////////////////////////////
#endif // BRDF_INCLUDED
////////////////////////////////////////////////////////////////////////////
