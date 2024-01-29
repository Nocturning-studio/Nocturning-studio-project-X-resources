////////////////////////////////////////////////////////////////////////////
//	Created		: 07.02.2023
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
#ifndef COMMON_MATH_H
#define COMMON_MATH_H
////////////////////////////////////////////////////////////////////////////
#include "common_defines.h"
////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////
#ifdef USE_VECTOR_AS_HALF
#define vector half
#define vector2 half2
#define vector3 half3
#define vector4 half4
#else
#define vector float
#define vector2 float2
#define vector3 float3
#define vector4 float4
#endif

#ifdef USE_MATRIX_AS_HALF
#define matrix1x1 half1x1
#define matrix1x2 half1x2
#define matrix1x3 half1x3
#define matrix1x4 half1x4

#define matrix2x1 half2x1
#define matrix2x2 half2x2
#define matrix2x3 half2x3
#define matrix2x4 half2x4

#define matrix3x1 half3x1
#define matrix3x2 half3x2
#define matrix3x3 half3x3
#define matrix3x4 half3x4

#define matrix4x1 half4x1
#define matrix4x2 half4x2
#define matrix4x3 half4x3
#define matrix4x4 half4x4
#else
#define matrix1x1 float1x1
#define matrix1x2 float1x2
#define matrix1x3 float1x3
#define matrix1x4 float1x4

#define matrix2x1 float2x1
#define matrix2x2 float2x2
#define matrix2x3 float2x3
#define matrix2x4 float2x4

#define matrix3x1 float3x1
#define matrix3x2 float3x2
#define matrix3x3 float3x3
#define matrix3x4 float3x4

#define matrix4x1 float4x1
#define matrix4x2 float4x2
#define matrix4x3 float4x3
#define matrix4x4 float4x4
#endif
////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////
// Inverting
#define invert(x) 1.0h - x
////////////////////////////////////////////////////////////////////////////
// Average functions
vector avg2(vector2 tex)
{
    return (tex.x + tex.y) * 0.5f;
};

vector avg3(vector3 tex)
{
    return (tex.x + tex.y + tex.z) * 0.3333333f;
};
////////////////////////////////////////////////////////////////////////////
vector avg4(vector4 tex)
{
    return (tex.x + tex.y + tex.z + tex.w) * 0.25f;
};
////////////////////////////////////////////////////////////////////////////
// Tex2D functions
vector4 tex2Dlod0(sampler2D smp, vector2 tc)
{
    return tex2Dlod(smp, vector4(tc, -0.5f, -0.5f));
}
////////////////////////////////////////////////////////////////////////////
// Approximation functions for sin/cos created by Skye Adaire
////////////////////////////////////////////////////////////////////////////
#define tauinv32 0.1591549431f
////////////////////////////////////////////////////////////////////////////
vector sinapprox(vector x)
{
    vector f = abs(frac(x * tauinv32 - 0.25) - 0.5);
    return f * f * (24.0 - 32.0 * f) - 1.0;
}
////////////////////////////////////////////////////////////////////////////
// the same as sin but with an x shift
vector cosapprox(vector x)
{
    vector f = frac(x * tauinv32 + 0.25);
    vector c = abs(f * 2.0 - 1.0) * 2.0 - 1.0;
    return (1.0 - c * c) * sign((1.0 - f) * 2.0 - 1.0);
}
////////////////////////////////////////////////////////////////////////////
#endif // COMMON_MATH_H
////////////////////////////////////////////////////////////////////////////
