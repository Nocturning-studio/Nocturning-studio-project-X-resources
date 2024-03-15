////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function from R.E.B.I.R.T.H. Shaders
// Tweaked to NSPX by Deathman
// Date: 09.03.2024
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Contants table
#define fvChroma float3(1.000f, 0.997f, 1.007f) // displacement scales of red, green and blue respectively
#define fBaseRadius 0.9f                       // below this radius the effect is less visible
#define fFalloffRadius 1.8f                    // over this radius the effects is maximal
#define fChromaPower 0.5f
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float3 ChromaticAberration(type_sampler2D(Image), float3 ImageColor, float2 tex)
{
    float d = distance(tex, float2(0.5f, 0.5f));
    float f = smoothstep(fBaseRadius, fFalloffRadius, d + 0.075f * d);
    float3 chroma = pow(f + fvChroma, fChromaPower);

    float2 tr = ((2.0f * tex - 1.0f) * chroma.r) * 0.5f + 0.5f;
    float2 tg = ((2.0f * tex - 1.0f) * chroma.g) * 0.5f + 0.5f;
    float2 tb = ((2.0f * tex - 1.0f) * chroma.b) * 0.5f + 0.5f;

    float3 color = float3(tex2D(Image, tr).r, tex2D(Image, tg).g, tex2D(Image, tb).b) * (1.0f - f);

    return lerp(ImageColor, color, 0.35f);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
