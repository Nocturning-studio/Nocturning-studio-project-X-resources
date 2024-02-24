////////////////////////////////////////////////////////////////////////////
//	Created		: 26.11.2023
//	Author		: Deathman, mihan-323
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
#ifndef FOG_INCLUDED
#define FOG_INCLUDED
////////////////////////////////////////////////////////////////////////////
#include "common.h"
////////////////////////////////////////////////////////////////////////////
#define FOG_DENSITY_MULTIPLIER 0.02f
#define VERTICAL_FOG_DENSITY_MULTIPLIER 0.1f
////////////////////////////////////////////////////////////////////////////
float CalcSqaredExponentialFogness(float3 Position)
{
	float Distance = length(Position);
	float FogDensity = fog_density * FOG_DENSITY_MULTIPLIER;
	return saturate(1.0h - exp((-Distance * FogDensity) * (Distance * FogDensity)));
}

float CalcVertexFogness(float3 Position)
{
	return CalcSqaredExponentialFogness(Position - eye_position);
}

void ApplyVertexFogness(inout float3 Color, float VertexFog)
{
	Color = lerp(Color, fog_color, VertexFog);
}

void ApplyPixelFogness(inout float3 Color, inout float Alpha, float3 Position)
{
	float Fog = CalcSqaredExponentialFogness(Position);
	Color = lerp(Color, fog_color, Fog);
	Alpha *= sqrt(Fog);
}
////////////////////////////////////////////////////////////////////////////
#endif//FOG_INCLUDED
////////////////////////////////////////////////////////////////////////////
