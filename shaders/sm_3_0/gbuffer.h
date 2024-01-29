////////////////////////////////////////////////////////////////////////////
//	Created		: 20.11.2022
//	Author		: Deathman
//	Basic idea 	: xRay engine 2.0 sm 3.0
////////////////////////////////////////////////////////////////////////////
/* Deathman to all:
   Необходим рефакторинг содержимого геометрического буфера 
   с переработкой материалов игры.
   MatID можно заменить на карту металличности, Glossiness можно 
   заменить на шероховатость.
   Позицию необходимо получать из глубины, а вместо освободившихся 
   компонентов можно хранить AO и эмиссию, чтобы использовать 
   их свободно в аккумуляции света.
*/
////////////////////////////////////////////////////////////////////////////
#ifndef GBUFFER_INCLUDED
#define GBUFFER_INCLUDED
////////////////////////////////////////////////////////////////////////////
#include "common.h"
////////////////////////////////////////////////////////////////////////////
struct GBufferData
{
    vector3 Position;
    vector Material;
    vector3 Normal;
    vector AO;
    vector3 Albedo;
    vector Glossiness;
};
////////////////////////////////////////////////////////////////////////////
struct GBuffer
{
    vector4 PositionMtl : COLOR0;
    vector4 NormalAO : COLOR1;
    vector4 AlbedoGloss : COLOR2;
};

GBuffer PackGBuffer(vector3 Position, vector Material, vector3 Normal, vector AO, vector3 Albedo, vector Gloss)
{
    GBuffer GData;

    GData.PositionMtl.rgb = Position;
    GData.PositionMtl.a = Material;

    GData.NormalAO.rgb = Normal;
    GData.NormalAO.a = AO;

    GData.AlbedoGloss.rgb = Albedo;
    GData.AlbedoGloss.a = Gloss;

    return GData;
}

vector3 GetPosition(vector2 TexCoords)
{
    vector3 Position = tex2Dlod0(s_gbuffer_position, TexCoords);

    // Check skybox intersection
    if (all(Position <= 0.0001h))
        Position.z = 400.0h;

    return Position;
}

GBufferData UnpackGBuffer(vector2 TexCoords)
{
    GBufferData GData;

    GData.Position = GetPosition(TexCoords);
    GData.Material = tex2D(s_gbuffer_position, TexCoords).a;

    vector4 NormalAO = tex2D(s_gbuffer_normal, TexCoords);

    GData.Normal = NormalAO.rgb;
    GData.AO = NormalAO.a;

    vector4 AlbedoGloss = tex2D(s_gbuffer_albedo, TexCoords);

    GData.Albedo = AlbedoGloss.rgb;
    GData.Glossiness = AlbedoGloss.a;

    return GData;
}

vector3 GetPositionProjected(vector4 TexCoords)
{
    vector3 Position = tex2Dproj(s_gbuffer_position, TexCoords);

    // Check skybox intersection
    if (all(Position <= 0.0001h))
        Position.z = 400.0h;

    return Position;
}

GBufferData UnpackGBufferWithProjection(vector4 TexCoords)
{
    GBufferData GData;

    GData.Position = GetPositionProjected(TexCoords);
    GData.Material = tex2Dproj(s_gbuffer_position, TexCoords).a;

    vector4 NormalAO = tex2Dproj(s_gbuffer_normal, TexCoords);

    GData.Normal = NormalAO.rgb;
    GData.AO = NormalAO.a;

    vector4 AlbedoGloss = tex2Dproj(s_gbuffer_albedo, TexCoords);

    GData.Albedo = AlbedoGloss.rgb;
    GData.Glossiness = AlbedoGloss.a;

    return GData;
}
////////////////////////////////////////////////////////////////////////////
#endif // GBUFFER_INCLUDED
////////////////////////////////////////////////////////////////////////////
