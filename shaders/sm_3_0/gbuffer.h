////////////////////////////////////////////////////////////////////////////
//	Created		: 20.11.2022
//	Author		: Deathman
//	Basic idea 	: xRay engine 2.0 sm 3.0
////////////////////////////////////////////////////////////////////////////
#ifndef GBUFFER_INCLUDED
#define GBUFFER_INCLUDED
////////////////////////////////////////////////////////////////////////////
#include "common.h"
////////////////////////////////////////////////////////////////////////////
struct GBuffer
{
    float3 Position;
    float BakedAO;
    float3 Normal;
    float AO;
    float3 Albedo;
    float Glossiness;
};
////////////////////////////////////////////////////////////////////////////
struct GBufferPacked
{
    float4 GBuffer0 : COLOR0;
    float4 GBuffer1 : COLOR1;
    float4 GBuffer2 : COLOR2;
};

GBufferPacked PackGBuffer(GBuffer Input)
{
    GBufferPacked GBuffer;

    GBuffer.GBuffer0.rgb = Input.Position;
    GBuffer.GBuffer0.a = Input.BakedAO;

    GBuffer.GBuffer1.rgb = Input.Normal;
    GBuffer.GBuffer1.a = Input.AO;

    GBuffer.GBuffer2.rgb = Input.Albedo;
    GBuffer.GBuffer2.a = Input.Glossiness;

    return GBuffer;
}

float3 GetPosition(float2 TexCoords)
{
    float3 Position = tex2Dlod0(s_gbuffer_position, TexCoords);

    // Check skybox intersection
    if (all(Position <= 0.0001h))
        Position.z = 400.0h;

    return Position;
}

GBuffer UnpackGBuffer(float2 TexCoords)
{
    GBuffer GBuffer;

    GBuffer.Position = GetPosition(TexCoords);
    GBuffer.BakedAO = tex2D(s_gbuffer_position, TexCoords).a;

    float4 NormalAO = tex2D(s_gbuffer_normal, TexCoords);

    GBuffer.Normal = NormalAO.rgb;
    GBuffer.AO = NormalAO.a;

    float4 AlbedoGloss = tex2D(s_gbuffer_albedo, TexCoords);

    GBuffer.Albedo = AlbedoGloss.rgb;
    GBuffer.Glossiness = AlbedoGloss.a;

    return GBuffer;
}

float3 GetPositionProjected(float4 TexCoords)
{
    float3 Position = tex2Dproj(s_gbuffer_position, TexCoords);

    // Check skybox intersection
    if (all(Position <= 0.0001h))
        Position.z = 400.0h;

    return Position;
}

GBuffer UnpackGBufferWithProjection(float4 TexCoords)
{
    GBuffer GBuffer;

    GBuffer.Position = GetPositionProjected(TexCoords);
    GBuffer.BakedAO = tex2Dproj(s_gbuffer_position, TexCoords).a;

    float4 NormalAO = tex2Dproj(s_gbuffer_normal, TexCoords);

    GBuffer.Normal = NormalAO.rgb;
    GBuffer.AO = NormalAO.a;

    float4 AlbedoGloss = tex2Dproj(s_gbuffer_albedo, TexCoords);

    GBuffer.Albedo = AlbedoGloss.rgb;
    GBuffer.Glossiness = AlbedoGloss.a;

    return GBuffer;
}
////////////////////////////////////////////////////////////////////////////
#endif // GBUFFER_INCLUDED
////////////////////////////////////////////////////////////////////////////
