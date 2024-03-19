////////////////////////////////////////////////////////////////////////////
//	Created		: 26.02.2023
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
//https://www.shadertoy.com/view/MdfXWr
////////////////////////////////////////////////////////////////////////////
float3 lin2srgb(float3 color)
{
    float3 S1 = sqrt(color);
    float3 S2 = sqrt(S1);
    float3 S3 = sqrt(S2);
    return 0.585122381f * S1 + 0.783140355f * S2 - 0.368262736f * S3;
}

float3 ff_filmic_gamma3(float3 Color) {
    float3 x = max(0.0, Color - 0.004);
    return (x * (x * 6.2 + 0.5)) / (x * (x * 6.2 + 1.7) + 0.06);
}

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
    const float white_level = 0.8f;
    const float luminance_saturation = 1.0f;
    const float pixel_luminance = get_luminance(Color);
    const float tone_mapped_luminance = 1.0f - exp(-pixel_luminance / white_level);

    return tone_mapped_luminance * pow(Color / pixel_luminance, luminance_saturation);
}
////////////////////////////////////////////////////////////////////////////
float3 CalcTonemap(float3 Color)
{
	// r1 hack
	Color *= 2;
	
    float white = 1.7f;
    float whitesqr = white * white;
    Color = (Color * (1.0f + Color / whitesqr)) / (Color + 1.0f);

    return Color;
}
////////////////////////////////////////////////////////////////////////////
