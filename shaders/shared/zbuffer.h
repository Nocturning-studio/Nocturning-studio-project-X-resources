////////////////////////////////////////////////////////////////////////////
//	Created		: 09.03.2024
//	Author		: Deathman, Mihan-323
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
float LinearizeDepth(float depth)
{
	return 2.0f * pos_decompression_params.z * pos_decompression_params.w
		/ (pos_decompression_params.w + pos_decompression_params.z - (depth * 2.0f - 1.0f)
			* (pos_decompression_params.w - pos_decompression_params.z));
}

float DelinearizeDepth(float depth)
{
	return 0.5f * ((pos_decompression_params.w + pos_decompression_params.z)
		/ (pos_decompression_params.w - pos_decompression_params.z) - (2.0f * pos_decompression_params.z / depth)
		/ (pos_decompression_params.w - pos_decompression_params.z) + 1.0f);
}

float GetZBufferFromINTZ(float2 TexCoords)
{
	return tex2D(s_zb, TexCoords).a;
}

float GetZBufferFromRAWZ(float2 TexCoords)
{
	float4 BGRA = tex2D(s_zb, TexCoords);
	return dot(BGRA.arg, float3(0.9960938, 0.0038909, 1.5199185e-5);
}
////////////////////////////////////////////////////////////////////////////
