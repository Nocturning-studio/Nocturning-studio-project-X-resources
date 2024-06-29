////////////////////////////////////////////////////////////////////////////
//	Created		: 20.11.2022
//	Author		: Deathman, Mihan-323
//	Basic idea 	: xRay engine 2.0 sm 3.0
////////////////////////////////////////////////////////////////////////////
#include "common.h"
////////////////////////////////////////////////////////////////////////////
uniform sampler2D s_gbuffer_1;
uniform sampler2D s_gbuffer_2;
uniform sampler2D s_gbuffer_3;
////////////////////////////////////////////////////////////////////////////
struct GBuffer
{
    float3 Albedo;
    float3 Position;
    float3 Normal;
    float Roughness;
    float Metallness;
    float BakedAO;
    float AO;
};

struct GBufferPacked
{
    float4 rt_GBuffer_1 : COLOR0;
    float4 rt_GBuffer_2 : COLOR1;
    float4 rt_GBuffer_3 : COLOR2;
};
////////////////////////////////////////////////////////////////////////////
float2 PackNormal(float3 norm)
{
   float2 res;
   res = 0.5 * (norm.xy + float2(1, 1));
   res.x *= (norm.z < 0 ? -1.0 : 1.0);
   res.x = res.x * 0.5 + 0.5;
   return res;
}

float3 UnpackNormal(float2 norm)
{
   float3 res;
   norm.x = norm.x * 2.0 - 1.0;
   res.xy = (2.0 * abs(norm)) - float2(1,1);
   res.z = (norm.x < 0 ? -1.0 : 1.0) * sqrt(abs(1 - res.x*res.x - res.y*res.y));
   return res;
}

float PackPosition(float3 position)
{
   return position.z;
}

float3 UnpackPosition(float position, float2 tc)
{
	float3 pos;
	pos.z = position.x;
	pos.xy = pos.z * (tc * 2.0f * pos_decompression_params.xy - pos_decompression_params.xy);
	return pos;
}
////////////////////////////////////////////////////////////////////////////
float GetDepth(float2 TexCoords)
{
    float Depth = tex2Dlod0(s_gbuffer_2, TexCoords).b;
    Depth = Depth < pos_decompression_params.z ? pos_decompression_params.w : Depth;

    return Depth;
}

float3 GetPosition(float2 TexCoords)
{
    float Depth = tex2D(s_gbuffer_2, TexCoords).b;
    Depth = Depth < pos_decompression_params.z ? pos_decompression_params.w : Depth;
    float3 Position = UnpackPosition(Depth, TexCoords);

    return Position;
}

float3 GetNormal(float2 TexCoords)
{
    float2 PackedNormal = tex2D(s_gbuffer_2, TexCoords).rg;
    return UnpackNormal(PackedNormal);
}

void GetPositionAndNormal(in float2 TexCoords, inout float3 Position, inout float3 Normal)
{
    float4 GBuffer_2 = tex2D(s_gbuffer_2, TexCoords);

    float Depth = GBuffer_2.z < pos_decompression_params.z ? pos_decompression_params.w : GBuffer_2.z;

    Position = UnpackPosition(Depth, TexCoords);

    Normal = UnpackNormal(GBuffer_2.rg);
}
////////////////////////////////////////////////////////////////////////////
GBufferPacked PackGBuffer(GBuffer Input)
{
    GBufferPacked GBuffer;

    GBuffer.rt_GBuffer_1 = float4(Input.Albedo, Input.Roughness);
    GBuffer.rt_GBuffer_2 = float4(PackNormal(Input.Normal), PackPosition(Input.Position), 0);
    GBuffer.rt_GBuffer_3 = float4(Input.Metallness, Input.AO, Input.BakedAO, 0);

    return GBuffer;
}

GBuffer UnpackGBuffer(float2 TexCoords)
{
    GBuffer GBuffer;

	float4 GBuffer_1 = tex2D(s_gbuffer_1, TexCoords);
	float4 GBuffer_2 = tex2D(s_gbuffer_2, TexCoords);
	float4 GBuffer_3 = tex2D(s_gbuffer_3, TexCoords);

	float Depth = GBuffer_2.z < pos_decompression_params.z ? pos_decompression_params.w : GBuffer_2.z;

    GBuffer.Albedo = GBuffer_1.rgb;
    GBuffer.Roughness = GBuffer_1.a;
    GBuffer.Normal = UnpackNormal(GBuffer_2.rg);
    GBuffer.Position = UnpackPosition(Depth, TexCoords);
    GBuffer.Metallness = GBuffer_3.r;
    GBuffer.AO = GBuffer_3.g;
    GBuffer.BakedAO = GBuffer_3.b;

    return GBuffer;
}

float4 PackPositionAndNormal(float3 Position, float3 Normal)
{
    return float4(PackNormal(Normal), PackPosition(Position), 0);
}
////////////////////////////////////////////////////////////////////////////
