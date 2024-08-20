////////////////////////////////////////////////////////////////////////////////
// Created: 07.08.2023
// Autor: Deathman
// Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////////
#include "common.xrh"
////////////////////////////////////////////////////////////////////////////////
struct VertexData
{
	float4 Position: POSITION;
	float4 Tangent: TANGENT;
	float4 Binormal: BINORMAL;
	float3 Normal: NORMAL;
	float4 Color: COLOR;
	float2 UV: TEXCOORD0;
};

struct Interpolators
{
	float4 HomogeniousPosition: POSITION;
	float3 Position: TEXCOORD0;
	float3 TBN0: TEXCOORD1;
	float3 TBN1: TEXCOORD2;
	float3 TBN2: TEXCOORD3;
	float2 UV: TEXCOORD4;
	float2 DetailUV: TEXCOORD5;
};
////////////////////////////////////////////////////////////////////////////////
Interpolators main (VertexData Input)
{
    Interpolators Output;

    Output.HomogeniousPosition = mul (m_WVP, Input.Position);

	float3 Position = mul (m_WV, Input.Position);
    Output.Position = Position;

	float3 Tangent = unpack_bx4 (Input.Tangent.xyz);
	float3 Bitangent = unpack_bx4 (Input.Binormal.xyz);
	float3 Normal = unpack_bx4 (Input.Normal);
	float3x3 TBN = mul ((float3x3)m_WV, float3x3 (Tangent.x, Bitangent.x, Normal.x,
													 Tangent.y, Bitangent.y, Normal.y,
													 Tangent.z, Bitangent.z, Normal.z));
    Output.TBN0 = TBN[0];
    Output.TBN1 = TBN[1];
    Output.TBN2 = TBN[2];

	float2 UV = unpack_tc_base (Input.UV, Input.Tangent.w, Input.Binormal.w);
    Output.UV = UV;
    Output.DetailUV = UV * dt_params.xy;

    return Output;
}
////////////////////////////////////////////////////////////////////////////////
