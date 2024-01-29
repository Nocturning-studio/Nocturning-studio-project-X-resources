////////////////////////////////////////////////////////////////////////////
//	Created		: 26.11.2023
//	Author		: Deathman
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
struct FogComponents
{
	vector3 Color;
	vector Fogness;
};
////////////////////////////////////////////////////////////////////////////
vector GetDistance(vector3 Position)
{
#if FOG_QUALITY == ULTRA_QUALITY 
	return length(Position);
#else
	return Position.z;
#endif
}

vector CalcEponentialFog(vector3 Position)
{
	vector Distance = GetDistance(Position);
	vector FogDensity = fog_density * FOG_DENSITY_MULTIPLIER;
	return saturate(1.0h - exp(-Distance * FogDensity));
}

vector CalcSqaredExponentialFog(vector3 Position)
{
	vector Distance = GetDistance(Position);
	vector FogDensity = fog_density * FOG_DENSITY_MULTIPLIER;
	return saturate(1.0h - exp((-Distance * FogDensity) * (Distance * FogDensity)));
}

vector CalcVerticalFog(vector3 Position)
{
	vector Fog = CalcSqaredExponentialFog(Position);
	vector3 WorldSpacePosition = mul(m_v2w, vector4(Position.xyz, 1.0h));
	vector VerticalFog = saturate(smoothstep(vertical_fog_height, -vertical_fog_height, WorldSpacePosition.y));
	VerticalFog *= vertical_fog_density * VERTICAL_FOG_DENSITY_MULTIPLIER * Fog;
	return saturate(VerticalFog + Fog);
}

vector CalcFogness(vector3 Position)
{
#if FOG_QUALITY == UNDEFINED_QUALITY || FOG_QUALITY == LOW_QUALITY || FOG_QUALITY == MIDDLE_QUALITY
	return CalcEponentialFog(Position);
#elif FOG_QUALITY == HIGHT_QUALITY
	return CalcSqaredExponentialFog(Position);
#elif FOG_QUALITY == ULTRA_QUALITY
	return CalcVerticalFog(Position);
#endif
}

FogComponents CalcFog(vector3 Position)
{
	FogComponents Output;

	Output.Fogness = CalcFogness(Position);
	Output.Color = fog_color.rgb;

	return Output;
}
////////////////////////////////////////////////////////////////////////////
#endif//FOG_INCLUDED
////////////////////////////////////////////////////////////////////////////
