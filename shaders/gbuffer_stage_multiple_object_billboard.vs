///////////////////////////////////////////////////////////////////////////////////
// Created: 25.10.2023
// Author: Deathman
// Nocturning studio for NS Project X
///////////////////////////////////////////////////////////////////////////////////
#include "common.xrh"
///////////////////////////////////////////////////////////////////////////////////
struct VertexData
{
	float3 Position0: POSITION0;
	float3 Position1: POSITION1;
	float3 Normal0: NORMAL0;
	float3 Normal1: NORMAL1;
	float3 UV0: TEXCOORD0;
	float3 UV1: TEXCOORD1;
	float4 ColorWithAmbient0: TEXCOORD2;
	float4 ColorWithAmbient1: TEXCOORD3;
	float4 SunOcclusionWithOpacityAndTransition: COLOR0;
};

struct Interpolators
{
	float4 HomogeniousPosition: POSITION;
	float3 Position: TEXCOORD0;
	float2 TexCoords0: TEXCOORD1;
	float2 TexCoords1: TEXCOORD2;
    float TransitionFactor: TEXCOORD3;
    float Ambient: TEXCOORD4;
    float SunOcclusion: TEXCOORD5;
    float OpacityFactor: TEXCOORD6;
};
///////////////////////////////////////////////////////////////////////////////////
Interpolators main (VertexData Input)
{
    Interpolators Output;

    float HightToLowQualityTransitionFactor = Input.SunOcclusionWithOpacityAndTransition.w;

	float3 InterpolatedVertexPosition = lerp (Input.Position0, Input.Position1, HightToLowQualityTransitionFactor);
	float4 VertexPosition = float4 (InterpolatedVertexPosition, 1.0f);

    float Ambient = lerp (Input.ColorWithAmbient0.w, Input.ColorWithAmbient1.w, HightToLowQualityTransitionFactor);

    float SunOcclusion = lerp (Input.SunOcclusionWithOpacityAndTransition.x, Input.SunOcclusionWithOpacityAndTransition.y, HightToLowQualityTransitionFactor);

    Output.HomogeniousPosition = mul (m_VP, VertexPosition);
    Output.Position = mul (m_V, VertexPosition);
    Output.TexCoords0 = Input.UV0.xy;
    Output.TexCoords1 = Input.UV1.xy;
    Output.TransitionFactor = HightToLowQualityTransitionFactor;
    Output.Ambient = Ambient;
    Output.SunOcclusion = SunOcclusion;
    Output.OpacityFactor = Input.SunOcclusionWithOpacityAndTransition.z;

    return Output;
}
///////////////////////////////////////////////////////////////////////////////////
