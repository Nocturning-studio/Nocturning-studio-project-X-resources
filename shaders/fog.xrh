////////////////////////////////////////////////////////////////////////////
//	Created		: 26.11.2023
//	Author		: Deathman, mihan-323
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
#define FOG_DENSITY_MULTIPLIER 0.02f
#define VERTICAL_FOG_DENSITY_MULTIPLIER 0.1f
////////////////////////////////////////////////////////////////////////////
float CalcSqaredExponentialFog(float3 Position)
{
	float Distance = length(Position);
	float FogDensity = fog_params.a * FOG_DENSITY_MULTIPLIER;
	return saturate(1.0f - exp((-Distance * FogDensity) * (Distance * FogDensity)));
}

// --Calculate linear fog with density only--
float CalcLinearFog(float3 Position)
{
	float DepthBuffer = length(Position);
	float NearBorder = mul_sat(far_plane, (invert(vertical_fog_density)));
	float FarBorder = far_plane;
	float r = 1.0f / (FarBorder - NearBorder);
	return mad_sat(DepthBuffer, r, (-NearBorder * r));
}

float CalcVerticalFog(float3 Position)
{
	float3 WorldSpacePosition = mul(m_v2w, float4(Position.xyz, 1.0f));
	float VerticalFog = smoothstep(vertical_fog_height, -vertical_fog_height, WorldSpacePosition.y);
	VerticalFog = saturate(VerticalFog);
	return CalcLinearFog(Position) * VerticalFog * vertical_fog_density;
}

float CalcVertexFogness(float3 Position)
{
	Position = Position - eye_position;

	return CalcSqaredExponentialFog(Position);
}

float CalcFogness(float3 Position)
{
	return CalcSqaredExponentialFog(Position);
}

// Использование одной и той же константы в верт. и пикс. 
// шейдере вызывает ошибку, так что для блендинга необходимо использовать 
// цвет в отдельной константе
float3 ApplyFogness(float3 Color, float Fogness)
{
	return lerp(Color, fog_color.rgb, Fogness);
}
////////////////////////////////////////////////////////////////////////////