///////////////////////////////////////////////////////////////////////////////////
// Created: 25.10.2023
// Author: Deathman
// Nocturning studio for NS Project X
///////////////////////////////////////////////////////////////////////////////////
#include "common.h"
///////////////////////////////////////////////////////////////////////////////////
struct VertexData
{
	float4 Position: POSITION;
    int4 Misc: TEXCOORD0;
};
///////////////////////////////////////////////////////////////////////////////////
struct Interpolators
{
	float4 HomogeniousPosition: POSITION;
	float3 Position: TEXCOORD0;
	float3 Normal: TEXCOORD3;
	float2 UV: TEXCOORD4;
	float2 Lighting: TEXCOORD5;
};
///////////////////////////////////////////////////////////////////////////////////
uniform float4 consts; // {1/quant,1/quant,diffusescale,ambient}
uniform float4 array[200]: register(c12);
///////////////////////////////////////////////////////////////////////////////////
Interpolators main (VertexData Input)
{
    Interpolators Output;
	// index
    int i = Input.Misc.w;
	float4 m0 = array[i + 0];
	float4 m1 = array[i + 1];
	float4 m2 = array[i + 2];
	float4 c0 = array[i + 3];

	// Transform pos to world coords
	float4 pos;
    pos.x = dot (m0, Input.Position);
    pos.y = dot (m1, Input.Position);
    pos.z = dot (m2, Input.Position);
    pos.w = 1.0h;

	// Normal in world coords
	float3 norm;
    norm.x = pos.x - m0.w;
    norm.y = pos.y - m1.w + 0.75f; // avoid zero
    norm.z = pos.z - m2.w;

	float4 Position = mul (m_WVP, pos);
    Output.Position = mul (m_WV, pos);
    Output.HomogeniousPosition = mul (m_WVP, pos);
    Output.Normal = mul (m_WV, normalize (norm));
    Output.UV = (Input.Misc * consts);

    Output.Lighting.x = c0.w;
    Output.Lighting.y = c0.x;

    return Output;
}
///////////////////////////////////////////////////////////////////////////////////