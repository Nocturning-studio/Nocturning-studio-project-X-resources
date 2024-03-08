///////////////////////////////////////////////////////////////////////////////////
// Created: 25.10.2023
// Author: Deathman
// Nocturning studio for NS Project X
///////////////////////////////////////////////////////////////////////////////////
#include "common.h"
///////////////////////////////////////////////////////////////////////////////////
struct Interpolators
{
    float4 HomogeniousPosition: POSITION;
    float3 Position: TEXCOORD0;
    float3 TBN0: TEXCOORD1;
    float3 TBN1: TEXCOORD2;
    float3 TBN2: TEXCOORD3;
    float2 UV: TEXCOORD4;
    float2 LightMapUV: TEXCOORD5;
    float2 Lighting: TEXCOORD6;
};
///////////////////////////////////////////////////////////////////////////////////
Interpolators _main (v_model Input)
{
    Interpolators Output;

    Output.HomogeniousPosition = mul (m_WVP, Input.P);
    Output.Position = mul (m_WV, Input.P);

    float3 Normal = Input.N;
    float3 Tangent = Input.T;
    float3 Bitangent = Input.B;
    float3x3 TBN = mul ((float3x3)m_WV, float3x3 (2.0h * Tangent.x, 2.0h * Bitangent.x, 2.0h * Normal.x,
												  2.0h * Tangent.y, 2.0h * Bitangent.y, 2.0h * Normal.y,
												  2.0h * Tangent.z, 2.0h * Bitangent.z, 2.0h * Normal.z));
    Output.TBN0 = TBN[0];
    Output.TBN1 = TBN[1];
    Output.TBN2 = TBN[2];

    Output.UV = Input.tc;
    Output.LightMapUV = Input.tc;
    Output.Lighting.x = L_material.x;
    Output.Lighting.y = L_material.y;

    return Output;
}
///////////////////////////////////////////////////////////////////////////////////
#if defined(SKIN_NONE)
Interpolators main(v_model v) { return _main(v); }
#elif defined(SKIN_0)
Interpolators main(v_model_skinned_0 v) { return _main(skinning_0(v)); }
#elif defined(SKIN_1)
Interpolators main(v_model_skinned_1 v) { return _main(skinning_1(v)); }
#elif defined(SKIN_2)
Interpolators main(v_model_skinned_2 v) { return _main(skinning_2(v)); }
#endif
///////////////////////////////////////////////////////////////////////////////////