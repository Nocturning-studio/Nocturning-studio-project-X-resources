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
	float3 Normal: TEXCOORD1;
	float2 UV: TEXCOORD2;
	float2 DetailUV: TEXCOORD3;
};
////////////////////////////////////////////////////////////////////////////////
Interpolators main (VertexData Input)
{
    Interpolators Output;

    Output.HomogeniousPosition = mul (m_WVP, Input.Position);

	float3 Position = mul (m_WV, Input.Position);
    Output.Position = Position;

    Output.Normal = mul (m_WV, unpack_bx2 (Input.Normal));

	float2 UV = unpack_tc_base (Input.UV, Input.Tangent.w, Input.Binormal.w);
    Output.UV = UV;
    Output.DetailUV = UV * dt_params.xy;

    return Output;
}
////////////////////////////////////////////////////////////////////////////////