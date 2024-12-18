///////////////////////////////////////////////////////////////////////////////////
// Created: 25.10.2023
// Author: Deathman
// Nocturning studio for NS Project X
///////////////////////////////////////////////////////////////////////////////////
#include "common.xrh"
///////////////////////////////////////////////////////////////////////////////////
struct VertexData
{
    float4 Position: POSITION; // (float,float,float,1)
    float4 Normal: NORMAL;
    float3 Tangent: TANGENT;
    float3 Binormal: BINORMAL;
    float4 UV: TEXCOORD0;
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
    float Lighting: TEXCOORD6;
};
///////////////////////////////////////////////////////////////////////////////////
Interpolators main (VertexData Input)
{
    Interpolators Output;

    float3 pos = mul (m_xform, Input.Position);
#ifdef	USE_TREEWAVE
	float base = m_xform._24;								// take base height from matrix
	float dp = calc_cyclic(wave.w+dot(pos,(float3)wave));
	float H = pos.y - base;									// height of vertex (scaled, rotated, etc.)
	float frac = Input.UV.z * consts.x;						// fractional (or rigidity)
	float inten = H * dp;									// intensity
	float2 result = calc_xz_wave(wind.xz*inten, frac);
#else
    float2 result = 0.0f;
#endif

    float4 w_pos = float4 (pos.x + result.x, pos.y, pos.z + result.y, 1.0f);

    Output.UV = (Input.UV * consts).xy;
    Output.HomogeniousPosition = mul (m_VP, w_pos);
    Output.Position = mul (m_V, w_pos);

    float3 Tangent = unpack_bx4 (Input.Tangent);
    float3 Bitangent = unpack_bx4 (Input.Binormal);
    float3 Normal = unpack_bx4 (Input.Normal.xyz);
    float3x3 TBN = mul ((float3x3)m_xform_v, float3x3 (Tangent.x, Bitangent.x, Normal.x,
													   Tangent.y, Bitangent.y, Normal.y,
													   Tangent.z, Bitangent.z, Normal.z));
    Output.TBN0 = TBN[0];
    Output.TBN1 = TBN[1];
    Output.TBN2 = TBN[2];

    Output.Lighting.x = (Input.Normal.w * 0.5f * c_scale.w + c_bias.w);

    return Output;
}
///////////////////////////////////////////////////////////////////////////////////
