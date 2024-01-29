////////////////////////////////////////////////////////////////////////////////
// Created: 07.08.2023
// Autor: Deathman
// Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////////
#include "common.h"
////////////////////////////////////////////////////////////////////////////////
struct VertexData
{
	vector4 Position: POSITION;
	vector4 Tangent: TANGENT;
	vector4 Binormal: BINORMAL;
	vector3 Normal: NORMAL;
	vector4 Color: COLOR;
	vector2 UV: TEXCOORD0;
};

struct Interpolators
{
	vector4 HomogeniousPosition: POSITION;
	vector3 Position: TEXCOORD0;
	vector3 TBN0: TEXCOORD1;
	vector3 TBN1: TEXCOORD2;
	vector3 TBN2: TEXCOORD3;
	vector2 UV: TEXCOORD4;
	vector2 DetailUV: TEXCOORD5;
};
////////////////////////////////////////////////////////////////////////////////
Interpolators main (VertexData Input)
{
    Interpolators Output;

    Output.HomogeniousPosition = mul (m_WVP, Input.Position);

	vector3 Position = mul (m_WV, Input.Position);
    Output.Position = Position;

	vector3 Tangent = unpack_bx4 (Input.Tangent.xyz);
	vector3 Bitangent = unpack_bx4 (Input.Binormal.xyz);
	vector3 Normal = unpack_bx4 (Input.Normal);
	matrix3x3 TBN = mul ((matrix3x3)m_WV, matrix3x3 (Tangent.x, Bitangent.x, Normal.x,
													 Tangent.y, Bitangent.y, Normal.y,
													 Tangent.z, Bitangent.z, Normal.z));
    Output.TBN0 = TBN[0];
    Output.TBN1 = TBN[1];
    Output.TBN2 = TBN[2];

	vector2 UV = unpack_tc_base (Input.UV, Input.Tangent.w, Input.Binormal.w);
    Output.UV = UV;
    Output.DetailUV = UV * dt_params.xy;

    return Output;
}
////////////////////////////////////////////////////////////////////////////////
