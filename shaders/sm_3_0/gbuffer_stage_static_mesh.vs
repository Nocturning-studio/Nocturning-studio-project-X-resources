///////////////////////////////////////////////////////////////////////////////////
// Created: 06.08.2023
// Author: Deathman
// Nocturning studio for NS Project X
///////////////////////////////////////////////////////////////////////////////////
#include "common.h"
///////////////////////////////////////////////////////////////////////////////////
struct VertexData
{
	float4 Position: POSITION;
	float4 Tangent: TANGENT;
	float4 Binormal: BINORMAL;
	float4 Normal: NORMAL;
	float4 Color: COLOR;
	float2 UV: TEXCOORD0;
	float2 LightMapUV: TEXCOORD1;
};
///////////////////////////////////////////////////////////////////////////////////
struct Interpolators
{
	float4 HomogeniousPosition: POSITION;
	float3 Position: TEXCOORD0;
	float3 TBN0: TEXCOORD1;
	float3 TBN1: TEXCOORD2;
	float3 TBN2: TEXCOORD3;
	float2 UV: TEXCOORD4;
	float2 LightMapTexcoords: TEXCOORD5;
	float2 Lighting: TEXCOORD6;
};
///////////////////////////////////////////////////////////////////////////////////
Interpolators main (VertexData Input)
{
    Interpolators Output;
	
    Output.HomogeniousPosition = mul (m_WVP, Input.Position);
    Output.Position = mul (m_WV, Input.Position);

	float3 Tangent = unpack_bx4 (Input.Tangent.xyz);
	float3 Bitangent = unpack_bx4 (Input.Binormal.xyz);
	float3 Normal = unpack_bx4 (Input.Normal.xyz);

	float3x3 TBN = mul ((float3x3)m_WV, float3x3 (Tangent.x, Bitangent.x, Normal.x,
												     Tangent.y, Bitangent.y, Normal.y,
												     Tangent.z, Bitangent.z, Normal.z));
    Output.TBN0 = TBN[0];
    Output.TBN1 = TBN[1];
    Output.TBN2 = TBN[2];

    Output.UV = (Input.UV + float2 (Input.Tangent.w, Input.Binormal.w)) * (32.f / 32768.f);
    Output.LightMapTexcoords = unpack_tc_lmap (Input.LightMapUV);
    Output.Lighting.x = Input.Normal.w;
    Output.Lighting.y = Input.Color.w;

    return Output;
}
///////////////////////////////////////////////////////////////////////////////////