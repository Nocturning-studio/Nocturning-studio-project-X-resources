////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ported: 16.10.2023
// Source: https://github.com/zachsaw/RenderScripts/blob/master/RenderScripts/ImageProcessingShaders/MPC-HC/Sharpen.hlsl
// Ported to Nocturning studio for NS Project X by Deathman
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define val0 (2.0)
#define val1 (-0.125)
#define effect_width (1.6)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
vector4 get_image_with_sharpen(sampler2D image, vector2 texcoords)
{
    vector dx = effect_width / screen_res.x;
    vector dy = effect_width / screen_res.y;

    vector4 c1 = tex2Dlod0(image, texcoords + float2(-dx, -dy)) * val1;
    vector4 c2 = tex2Dlod0(image, texcoords + float2(0, -dy)) * val1;
    vector4 c3 = tex2Dlod0(image, texcoords + float2(-dx, 0)) * val1;
    vector4 c4 = tex2Dlod0(image, texcoords + float2(dx, 0)) * val1;
    vector4 c5 = tex2Dlod0(image, texcoords + float2(0, dy)) * val1;
    vector4 c6 = tex2Dlod0(image, texcoords + float2(dx, dy)) * val1;
    vector4 c7 = tex2Dlod0(image, texcoords + float2(-dx, +dy)) * val1;
    vector4 c8 = tex2Dlod0(image, texcoords + float2(+dx, -dy)) * val1;
    vector4 c9 = tex2Dlod0(image, texcoords) * val0;

    return (c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8 + c9);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
