////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function from R.E.B.I.R.T.H. Shaders
// Tweaked to NSPX by Deathman
// Date: 29.01.2023
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Contants table
#define fvChroma float3(1.000h, 0.997h, 1.007h) // displacement scales of red, green and blue respectively
#define fBaseRadius 0.9h                       // below this radius the effect is less visible
#define fFalloffRadius 1.8h                    // over this radius the effects is maximal
#define fChromaPower 0.5h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float3 ChromaticAberration(float3 ImageColor, float2 tex)
{
    float d = distance(tex, float2(0.5h, 0.5h));
    float f = smoothstep(fBaseRadius, fFalloffRadius, d + 0.075h * d);
    float3 chroma = pow(f + fvChroma, fChromaPower);

    float2 tr = ((2.0h * tex - 1.0h) * chroma.r) * 0.5h + 0.5h;
    float2 tg = ((2.0h * tex - 1.0h) * chroma.g) * 0.5h + 0.5h;
    float2 tb = ((2.0h * tex - 1.0h) * chroma.b) * 0.5h + 0.5h;

    float3 color = float3(tex2D(s_base, tr).r, tex2D(s_base, tg).g, tex2D(s_base, tb).b) * (1.0h - f);

    return lerp(ImageColor, float3(color.r, color.g, color.b), 0.35h);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
