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
	vector3 Normal: TEXCOORD1;
	vector2 UV: TEXCOORD2;
	vector2 DetailUV: TEXCOORD3;
};
////////////////////////////////////////////////////////////////////////////////
Interpolators main (VertexData Input)
{
    Interpolators Output;

    Output.HomogeniousPosition = mul (m_WVP, Input.Position);

	vector3 Position = mul (m_WV, Input.Position);
    Output.Position = Position;

    Output.Normal = mul (m_WV, unpack_bx2 (Input.Normal));

	vector2 UV = unpack_tc_base (Input.UV, Input.Tangent.w, Input.Binormal.w);
    Output.UV = UV;
    Output.DetailUV = UV * dt_params.xy;

    return Output;
}
////////////////////////////////////////////////////////////////////////////////