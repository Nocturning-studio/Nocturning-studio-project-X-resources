////////////////////////////////////////////////////////////////////////////
//	Created		: 20.11.2023
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
#ifndef LIGHTING_ACCUMULATION_HELPER_INCLUDED
#define LIGHTING_ACCUMULATION_HELPER_INCLUDED
////////////////////////////////////////////////////////////////////////////
#include "common.h"
////////////////////////////////////////////////////////////////////////////
struct LightingData
{
	float3 Diffuse;
	float3 Specular;
};

struct Accumulator
{
	float4 DiffuseAccumulator: COLOR0;
	float4 SpecularAccumulator: COLOR1;
};

Accumulator PackLightingAccumulator(float3 Diffuse, float3 Specular)
{
	Accumulator Output;

	Output.DiffuseAccumulator.rgb = Diffuse;
	Output.DiffuseAccumulator.a = 0.0h;
	Output.SpecularAccumulator.rgb = Specular;
	Output.SpecularAccumulator.a = 0.0h;

	return Output;
}

LightingData UnpackLightingAccumulator(float2 TexCoords)
{
	LightingData Output;
	
	float4 DiffuseAccumulator = tex2D(s_diffuse_accumulator, TexCoords);
	float4 SpecularAccumulator = tex2D(s_specular_accumulator, TexCoords);

	Output.Diffuse = DiffuseAccumulator.rgb;
	Output.Specular = SpecularAccumulator.rgb;

	return Output;
}
////////////////////////////////////////////////////////////////////////////
#endif //LIGHTING_ACCUMULATION_HELPER_INCLUDED
////////////////////////////////////////////////////////////////////////////
