///////////////////////////////////////////////////////////////////////////////////
// Created: 25.10.2023
// Author: Deathman
// Nocturning studio for NS Project X
///////////////////////////////////////////////////////////////////////////////////
#include "common.h"
///////////////////////////////////////////////////////////////////////////////////
struct VertexData
{
	vector4 Position: POSITION;
    int4 Misc: TEXCOORD0;
};
///////////////////////////////////////////////////////////////////////////////////
struct Interpolators
{
	vector4 HomogeniousPosition: POSITION;
	vector3 Position: TEXCOORD0;
	vector3 Normal: TEXCOORD3;
	vector2 UV: TEXCOORD4;
	vector2 Lighting: TEXCOORD5;
};
///////////////////////////////////////////////////////////////////////////////////
uniform vector4 consts; // {1/quant,1/quant,diffusescale,ambient}
uniform vector4 array[200]: register(c12);
///////////////////////////////////////////////////////////////////////////////////
Interpolators main (VertexData Input)
{
    Interpolators Output;
	// index
    int i = Input.Misc.w;
	vector4 m0 = array[i + 0];
	vector4 m1 = array[i + 1];
	vector4 m2 = array[i + 2];
	vector4 c0 = array[i + 3];

	// Transform pos to world coords
	vector4 pos;
    pos.x = dot (m0, Input.Position);
    pos.y = dot (m1, Input.Position);
    pos.z = dot (m2, Input.Position);
    pos.w = 1.0h;

	// Normal in world coords
	vector3 norm;
    norm.x = pos.x - m0.w;
    norm.y = pos.y - m1.w + 0.75f; // avoid zero
    norm.z = pos.z - m2.w;

	vector4 Position = mul (m_WVP, pos);
    Output.Position = mul (m_WV, pos);
    Output.HomogeniousPosition = mul (m_WVP, pos);
    Output.Normal = mul (m_WV, normalize (norm));
    Output.UV = (Input.Misc * consts);

    Output.Lighting.x = c0.w;
    Output.Lighting.y = c0.x;

    return Output;
}
///////////////////////////////////////////////////////////////////////////////////