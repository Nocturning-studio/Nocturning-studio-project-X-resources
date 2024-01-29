///////////////////////////////////////////////////////////////////////////////////
// Created: 06.08.2023
// Author: Deathman
// Nocturning studio for NS Project X
///////////////////////////////////////////////////////////////////////////////////
#include "common.h"
///////////////////////////////////////////////////////////////////////////////////
struct VertexData
{
	vector4 Position: POSITION;
	vector4 Tangent: TANGENT;
	vector4 Binormal: BINORMAL;
	vector4 Normal: NORMAL;
	vector4 Color: COLOR;
	vector2 UV: TEXCOORD0;
	vector2 LightMapUV: TEXCOORD1;
};
///////////////////////////////////////////////////////////////////////////////////
struct Interpolators
{
	vector4 HomogeniousPosition: POSITION;
	vector3 Position: TEXCOORD0;
	vector3 TBN0: TEXCOORD1;
	vector3 TBN1: TEXCOORD2;
	vector3 TBN2: TEXCOORD3;
	vector2 UV: TEXCOORD4;
	vector2 LightMapTexcoords: TEXCOORD5;
	vector2 Lighting: TEXCOORD6;
};
///////////////////////////////////////////////////////////////////////////////////
Interpolators main (VertexData Input)
{
    Interpolators Output;
	
    Output.HomogeniousPosition = mul (m_WVP, Input.Position);
    Output.Position = mul (m_WV, Input.Position);

	vector3 Tangent = unpack_bx4 (Input.Tangent.xyz);
	vector3 Bitangent = unpack_bx4 (Input.Binormal.xyz);
	vector3 Normal = unpack_bx4 (Input.Normal.xyz);

	matrix3x3 TBN = mul ((matrix3x3)m_WV, matrix3x3 (Tangent.x, Bitangent.x, Normal.x,
												     Tangent.y, Bitangent.y, Normal.y,
												     Tangent.z, Bitangent.z, Normal.z));
    Output.TBN0 = TBN[0];
    Output.TBN1 = TBN[1];
    Output.TBN2 = TBN[2];

    Output.UV = (Input.UV + vector2 (Input.Tangent.w, Input.Binormal.w)) * (32.f / 32768.f);
    Output.LightMapTexcoords = unpack_tc_lmap (Input.LightMapUV);
    Output.Lighting.x = Input.Normal.w;
    Output.Lighting.y = Input.Color.w;

    return Output;
}
///////////////////////////////////////////////////////////////////////////////////