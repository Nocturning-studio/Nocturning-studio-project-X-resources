///////////////////////////////////////////////////////////////////////////////////
// Created: 25.10.2023
// Author: Deathman
// Nocturning studio for NS Project X
///////////////////////////////////////////////////////////////////////////////////
#include "common.h"
///////////////////////////////////////////////////////////////////////////////////
struct VertexData
{
	vector3 Position0: POSITION0;
	vector3 Position1: POSITION1;
	vector3 Normal0: NORMAL0;
	vector3 Normal1: NORMAL1;
	vector3 UV0: TEXCOORD0;
	vector3 UV1: TEXCOORD1;
	vector4 ColorWithAmbient0: TEXCOORD2;
	vector4 ColorWithAmbient1: TEXCOORD3;
	vector4 SunOcclusionWithOpacityAndTransition: COLOR0;
};

struct Interpolators
{
	vector4 HomogeniousPosition: POSITION;
	vector3 Position: TEXCOORD0;
	vector2 TexCoords0: TEXCOORD1;
	vector2 TexCoords1: TEXCOORD2;
    vector TransitionFactor: TEXCOORD3;
    vector Ambient: TEXCOORD4;
    vector SunOcclusion: TEXCOORD5;
    vector OpacityFactor: TEXCOORD6;
};
///////////////////////////////////////////////////////////////////////////////////
Interpolators main (VertexData Input)
{
    Interpolators Output;

    vector HightToLowQualityTransitionFactor = Input.SunOcclusionWithOpacityAndTransition.w;

	vector3 InterpolatedVertexPosition = lerp (Input.Position0, Input.Position1, HightToLowQualityTransitionFactor);
	vector4 VertexPosition = vector4 (InterpolatedVertexPosition, 1.0h);

    vector Ambient = lerp (Input.ColorWithAmbient0.w, Input.ColorWithAmbient1.w, HightToLowQualityTransitionFactor);

    vector SunOcclusion = lerp (Input.SunOcclusionWithOpacityAndTransition.x, Input.SunOcclusionWithOpacityAndTransition.y, HightToLowQualityTransitionFactor);

    Output.HomogeniousPosition = mul (m_VP, VertexPosition);
    Output.Position = mul (m_V, VertexPosition);
    Output.TexCoords0 = Input.UV0;
    Output.TexCoords1 = Input.UV1;
    Output.TransitionFactor = HightToLowQualityTransitionFactor;
    Output.Ambient = Ambient;
    Output.SunOcclusion = SunOcclusion;
    Output.OpacityFactor = Input.SunOcclusionWithOpacityAndTransition.z;

    return Output;
}
///////////////////////////////////////////////////////////////////////////////////
