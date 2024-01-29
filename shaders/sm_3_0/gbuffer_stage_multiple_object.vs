///////////////////////////////////////////////////////////////////////////////////
// Created: 25.10.2023
// Author: Deathman
// Nocturning studio for NS Project X
///////////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "skinning.h"
///////////////////////////////////////////////////////////////////////////////////
struct VertexData
{
	vector4 Position: POSITION; // (vector,vector,vector,1)
	vector4 Normal: NORMAL;
	vector3 Tangent: TANGENT;
	vector3 Binormal: BINORMAL;
	vector4 UV: TEXCOORD0;
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
	vector2 Lighting: TEXCOORD6;
};
///////////////////////////////////////////////////////////////////////////////////
uniform matrix3x4 m_xform;
uniform matrix3x4 m_xform_v;
uniform vector4 consts; // {1/quant,1/quant,???,???}
uniform vector4 c_scale;
uniform vector4 c_bias;
uniform vector4 wind;
uniform vector4 wave;
uniform vector2 c_sun; // x=*, y=+
///////////////////////////////////////////////////////////////////////////////////
Interpolators main (VertexData Input)
{
    Interpolators Output;

	vector3 pos = mul (m_xform, Input.Position);
#ifdef	USE_TREEWAVE
	vector base = m_xform._24;									// take base height from matrix
	vector dp = calc_cyclic(wave.w+dot(pos,(vector3)wave));
	vector H = pos.y - base;									// height of vertex (scaled, rotated, etc.)
	vector frac = Input.UV.z * consts.x;						// fractional (or rigidity)
	vector inten = H * dp;										// intensity
	vector2 result = calc_xz_wave(wind.xz*inten, frac);
#else
	vector2 result = 0.0h;
#endif

	vector4 w_pos = vector4 (pos.x + result.x, pos.y, pos.z + result.y, 1.0h);

    Output.UV = (Input.UV * consts).xy;
    Output.HomogeniousPosition = mul (m_VP, w_pos);
    Output.Position = mul (m_V, w_pos);

	vector3 Tangent = unpack_bx4 (Input.Tangent);
	vector3 Bitangent = unpack_bx4 (Input.Binormal);
	vector3 Normal = unpack_bx4 (Input.Normal);
	matrix3x3 TBN = mul ((matrix3x3)m_xform_v, matrix3x3 (Tangent.x, Bitangent.x, Normal.x,
														  Tangent.y, Bitangent.y, Normal.y,
														  Tangent.z, Bitangent.z, Normal.z));
    Output.TBN0 = TBN[0];
    Output.TBN1 = TBN[1];
    Output.TBN2 = TBN[2];

    Output.Lighting.x = (Input.Normal.w * 0.5f * c_scale.w + c_bias.w);

#ifdef USE_R2_STATIC_SUN
	vector suno = Input.Normal.w * c_sun.x + c_sun.y;
	Output.Lighting.y = suno;
#else
    Output.Lighting.y = 1.0h;
#endif

    return Output;
}
///////////////////////////////////////////////////////////////////////////////////
