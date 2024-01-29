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
	vector3 Diffuse;
	vector3 Specular;
};

struct Accumulator
{
	vector4 DiffuseAccumulator: COLOR0;
	vector4 SpecularAccumulator: COLOR1;
};

Accumulator PackLightingAccumulator(vector3 Diffuse, vector3 Specular)
{
	Accumulator Output;

	Output.DiffuseAccumulator.rgb = Diffuse;
	Output.DiffuseAccumulator.a = 0.0h;
	Output.SpecularAccumulator.rgb = Specular;
	Output.SpecularAccumulator.a = 0.0h;

	return Output;
}

LightingData UnpackLightingAccumulator(vector2 TexCoords)
{
	LightingData Output;
	
	vector4 DiffuseAccumulator = tex2D(s_diffuse_accumulator, TexCoords);
	vector4 SpecularAccumulator = tex2D(s_specular_accumulator, TexCoords);

	Output.Diffuse = DiffuseAccumulator.rgb;
	Output.Specular = SpecularAccumulator.rgb;

	return Output;
}
////////////////////////////////////////////////////////////////////////////
#endif //LIGHTING_ACCUMULATION_HELPER_INCLUDED
////////////////////////////////////////////////////////////////////////////
