////////////////////////////////////////////////////////////////////////////
//	Created		: 26.02.2023
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
float3 ACES(const float3 x)
{
    const float a = 2.51f;
    const float b = 0.03f;
    const float c = 2.43f;
    const float d = 0.59f;
    const float e = 0.14f;
    return (x * (a * x + b)) / (x * (c * x + d) + e);
}
////////////////////////////////////////////////////////////////////////////
// X-Ray 2.0
////////////////////////////////////////////////////////////////////////////
static const float3 luminance_weights = float3(0.2125f, 0.7154f, 0.0721f);
////////////////////////////////////////////////////////////////////////////
float get_luminance(float3 Color)
{
    return dot(Color, luminance_weights);
}

float3 exponential_tonemapping(float3 Color)
{
    const float white_level = 1.25f;
    const float luminance_saturation = 1.05f;
    //Color *= white_level;
    const float pixel_luminance = get_luminance(Color);
    const float tone_mapped_luminance = 1.0f - exp(-pixel_luminance / white_level);

    return tone_mapped_luminance * pow(Color / pixel_luminance, luminance_saturation) * white_level;
}
////////////////////////////////////////////////////////////////////////////
float3 uncharted2(float3 x)
{
    float A = 0.15f;
    float B = 0.50f;
    float C = 0.10f;
    float D = 0.20f;
    float E = 0.02f;
    float F = 0.30f;
    return (((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F);
}
////////////////////////////////////////////////////////////////////////////
#define USE_SRGB
////////////////////////////////////////////////////////////////////////////
float3 sRgbToLinear(float3 vValue)
{
#ifdef USE_SRGB
    return vValue * (vValue * (vValue * 0.305306011 + 0.682171111) + 0.012522878);
#else
    return vValue;
#endif
}

float3 LinearTosRgb(float3 vColor)
{
#ifdef USE_SRGB
    float3 S1 = sqrt(vColor);
    float3 S2 = sqrt(S1);
    float3 S3 = sqrt(S2);
    return (0.585122381 * S1 + 0.783140355 * S2 - 0.368262736 * S3);
#else
    return vColor;
#endif
}
////////////////////////////////////////////////////////////////////////////
float3 Reinhard(float3 Color) 
{
    return Color / (Color + 1.0f);
}
////////////////////////////////////////////////////////////////////////////
float3 Reinhard2(float3 Color, float Exposure) 
{
    return (Color * (1.0f + Color / pow2(Exposure))) / (1.0f + Color);
}
////////////////////////////////////////////////////////////////////////////
// https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
float3 ACESFilm(float3 x) {
    return clamp((x * (2.51f * x + 0.03f)) / (x * (2.43f * x + 0.59f) + 0.14f), 0.0f, 1.0f);
}
////////////////////////////////////////////////////////////////////////////
float3 CalcTonemap(float3 Color)
{
    //Color = sRgbToLinear(Color);
    Color = Reinhard2(Color, 4.0f) * 2.0;
    //Color = pow(Color, 0.8f);
    //Color = Color / (Color + 1.0f);
    //Color = 1.0f - exp(-Color * 1.5);
    //Color = pow(Color, 1.0f / 2.2f);
    //Color = LinearTosRgb(Color);
    return Color;
}
////////////////////////////////////////////////////////////////////////////
float3 GammaCorrect(float3 Color)
{
    return pow(Color, 2.2f);
}
////////////////////////////////////////////////////////////////////////////
float3 CalcExposure(float3 Color, float Exposure)
{
    float whitesqr = Exposure * Exposure;
    Color *= Exposure;
    return (Color * (1.0f + Color / whitesqr)) / (Color + 1.0f);
}
////////////////////////////////////////////////////////////////////////////
