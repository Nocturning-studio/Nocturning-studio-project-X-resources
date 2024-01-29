////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Created: 27.12.2023
//      FXAA - Nvidia
//      Integrate to S.T.A.L.K.E.R: Call of Pripyat - Zackin5 (https://github.com/Zackin5/StalkerCop-FxaaShaders)
//      Integrate to Nocturning studio for NS Project X - Deathman
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef FXAA_ATOC_INCLUDED
#define FXAA_ATOC_INCLUDED
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FXAA settings
#define FXAA_QUALITY__PRESET 39

// Choose the amount of sub-pixel aliasing removal.
// This can effect sharpness.
//   1.00 - upper limit (softer)
//   0.75 - default amount of filtering
//   0.50 - lower limit (sharper, less sub-pixel aliasing removal)
//   0.25 - almost off
//   0.00 - completely off
#define FXAA_Subpix 1.0

// The minimum amount of local contrast required to apply algorithm.
//   0.333 - too little (faster)
//   0.250 - low quality
//   0.166 - default
//   0.125 - high quality
//   0.063 - overkill (slower)
#define FXAA_EdgeThreshold 0.01

// Trims the algorithm from processing darks.
//   0.0833 - upper limit (default, the start of visible unfiltered edges)
//   0.0625 - high quality (faster)
//   0.0312 - visible limit (slower)
// Special notes when using FXAA_GREEN_AS_LUMA,
//   Likely want to set this to zero.
//   As colors that are mostly not-green
//   will appear very dark in the green channel!
//   Tune by looking at mostly non-green content,
//   then start at zero and increase until aliasing is a problem.
#define FXAA_EdgeThresholdMin 0.0
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "fxaa.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO - заменить на актуальное разрешение ткстуры
#define ALBEDO_TEXTURE_RESOLUTION vector2(1024.0h, 1024.0h)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void calc_fxaa_atoc(vector2 TexCoords, out vector3 Albedo, out vector Opacity)
{
    vector2 rcpFrame = vector2(1.0f / ALBEDO_TEXTURE_RESOLUTION.x, 1.0f / ALBEDO_TEXTURE_RESOLUTION.y);

    vector4 TextureColor =
        FxaaPixelShader(TexCoords, s_base, rcpFrame, FXAA_Subpix, FXAA_EdgeThreshold, FXAA_EdgeThresholdMin);

    Albedo = TextureColor.rgb;
    Opacity = TextureColor.a;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // FXAA_ATOC_INCLUDED
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
