////////////////////////////////////////////////////////////////////////////
//	Created		: 26.02.2023
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
#ifndef HDR_INCLUDED
#define HDR_INCLUDED
////////////////////////////////////////////////////////////////////////////
#include "common.h"
////////////////////////////////////////////////////////////////////////////
uniform float3 hdr_params;
////////////////////////////////////////////////////////////////////////////
float3 Calc_hdr(float3 Color)
{
    float fWhiteIntensitySQR = pow(hdr_params.x, 2.0h);

    Color.rgb = float3(((Color.rgb * (1.0h + Color.rgb / fWhiteIntensitySQR)) / (Color.rgb + 1.0h)));

    Color = pow(Color, hdr_params.y);

    Color *= hdr_params.z;

    return Color;
}
////////////////////////////////////////////////////////////////////////////
#endif//HDR_INCLUDED
////////////////////////////////////////////////////////////////////////////