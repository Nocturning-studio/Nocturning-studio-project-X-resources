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
float3 ChromaticAberration(texture2D ImageTexture, float3 ImageColor, float2 TexCoords)
{
    float d = distance(TexCoords, float2(0.5f, 0.5f));
    float f = smoothstep(fBaseRadius, fFalloffRadius, d + 0.075f * d);
    float3 chroma = pow(f + fvChroma, fChromaPower);

    float2 tr = ((2.0f * TexCoords - 1.0f) * chroma.r) * 0.5f + 0.5f;
    float2 tg = ((2.0f * TexCoords - 1.0f) * chroma.g) * 0.5f + 0.5f;
    float2 tb = ((2.0f * TexCoords - 1.0f) * chroma.b) * 0.5f + 0.5f;

    float3 color;
    color.x = ImageTexture.Sample(smp_nofilter, tr).r;
    color.y = ImageTexture.Sample(smp_nofilter, tg).g;
    color.z = ImageTexture.Sample(smp_nofilter, tb).b;
    color *= (1.0f - f);

    return lerp(ImageColor, color, 0.35f);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
