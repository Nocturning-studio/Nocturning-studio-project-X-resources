////////////////////////////////////////////////////////////////////////////
//	Created		: 26.11.2023
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
#include "common.h"
////////////////////////////////////////////////////////////////////////////
#define FOG_DENSITY_MULTIPLIER 0.02f
#define VERTICAL_FOG_DENSITY_MULTIPLIER 0.1f
////////////////////////////////////////////////////////////////////////////
struct FogComponents
{
	float3 Color;
	float Fogness;
};
////////////////////////////////////////////////////////////////////////////
float GetDistance(float3 Position)
{
#if FOG_QUALITY == ULTRA_QUALITY 
	return length(Position);
#else
	return Position.z;
#endif
}

float CalcEponentialFog(float3 Position)
{
	float Distance = GetDistance(Position);
	float FogDensity = fog_density * FOG_DENSITY_MULTIPLIER;
	return saturate(1.0h - exp(-Distance * FogDensity));
}

float CalcSqaredExponentialFog(float3 Position)
{
	float Distance = GetDistance(Position);
	float FogDensity = fog_density * FOG_DENSITY_MULTIPLIER;
	return saturate(1.0h - exp((-Distance * FogDensity) * (Distance * FogDensity)));
}

float CalcVerticalFog(float3 Position)
{
	float Fog = CalcSqaredExponentialFog(Position);
	float3 WorldSpacePosition = mul(m_v2w, float4(Position.xyz, 1.0h));
	float VerticalFog = saturate(smoothstep(vertical_fog_height, -vertical_fog_height, WorldSpacePosition.y));
	VerticalFog *= vertical_fog_density * VERTICAL_FOG_DENSITY_MULTIPLIER * Fog;
	return saturate(VerticalFog + Fog);
}

float CalcFogness(float3 Position)
{
#if FOG_QUALITY == UNDEFINED_QUALITY || FOG_QUALITY == LOW_QUALITY || FOG_QUALITY == MIDDLE_QUALITY
	return CalcEponentialFog(Position);
#elif FOG_QUALITY == HIGHT_QUALITY
	return CalcSqaredExponentialFog(Position);
#elif FOG_QUALITY == ULTRA_QUALITY
	return CalcVerticalFog(Position);
#endif
}

FogComponents CalcFog(float3 Position)
{
	FogComponents Output;

	Output.Fogness = CalcFogness(Position);
	Output.Color = fog_color.rgb;

	return Output;
}
////////////////////////////////////////////////////////////////////////////