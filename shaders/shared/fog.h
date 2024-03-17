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

float CalcVertexFogness(float3 Position)
{
	return CalcSqaredExponentialFog(Position - eye_position);
}

float CalcFogness(float3 Position)
{
	return CalcSqaredExponentialFog(Position);
}

// ������������� ����� � ��� �� ��������� � ����. � ����. 
// ������� �������� ������, ��� ��� ��� ��������� ���������� ������������ 
// ���� � ��������� ���������
float3 ApplyFogness(float3 Color, float Fogness)
{
	return lerp(Color, fog_color.rgb, Fogness);
}
////////////////////////////////////////////////////////////////////////////