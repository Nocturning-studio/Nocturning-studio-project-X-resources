////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function from R.E.B.I.R.T.H. Shaders
// Tweaked to NSPX by Deathman
// Date: 29.01.2023
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Contants table
#define fvChroma float3(1.000, 0.997, 1.007) // displacement scales of red, green and blue respectively
#define fBaseRadius 0.9                       // below this radius the effect is less visible
#define fFalloffRadius 1.8                    // over this radius the effects is maximal
#define fChromaPower 0.5
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float3 ChromaticAberration(sampler2D image, float2 tex)
{
    float d = distance(tex, float2(0.5, 0.5));
    float f = smoothstep(fBaseRadius, fFalloffRadius, d + 0.075 * d);
    float3 chroma = pow(f + fvChroma, fChromaPower);

    float2 tr = ((2.0 * tex - 1.0) * chroma.r) * 0.5 + 0.5;
    float2 tg = ((2.0 * tex - 1.0) * chroma.g) * 0.5 + 0.5;
    float2 tb = ((2.0 * tex - 1.0) * chroma.b) * 0.5 + 0.5;

    float3 color = float3(tex2Dlod0(image, tr).r, tex2Dlod0(image, tg).g, tex2Dlod0(image, tb).b) * (1.0 - f);

    float3 ImageColor = tex2Dlod0(image, tex);
    return lerp(ImageColor, float3(color.r, color.g, color.b), 0.35);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////