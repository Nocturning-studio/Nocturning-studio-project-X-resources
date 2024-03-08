////////////////////////////////////////////////////////////////////////////
//	Created		: 20.11.2022
//	Author		: Deathman
//	Basic idea 	: xRay engine 2.0 sm 3.0
////////////////////////////////////////////////////////////////////////////
#include "common.h"
////////////////////////////////////////////////////////////////////////////
// def GBUFFER_OPT_MODE:
// 0 - none,			  BGRA8 + RGBA16F + RGBA16F  = 160bit
// 1 - partial,           BGRA8 + BGRA8   + R16F     = 80bit
// 2 - full,      'INTZ'  BGRA8 + BGRA8              = 64bit
// 3 - full,      'RAWZ'  BGRA8 + BGRA8              = 64bit
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
struct GBufferPacked
{
    float4 rt_Albedo   : COLOR0;
    float4 rt_Normal   : COLOR1;
#if GBUFFER_OPT_MODE <= 1
    float4 rt_Position : COLOR2;
#endif
};
////////////////////////////////////////////////////////////////////////////
#if GBUFFER_OPT_MODE > 0
float2 gbuf_pack_normal(float3 norm)
{
   float2 res;
   res = 0.5 * (norm.xy + float2(1, 1));
   res.x *= (norm.z < 0 ? -1.0 : 1.0);
   res.x = res.x * 0.5 + 0.5;
   return res;
}

float3 gbuf_unpack_normal(float2 norm)
{
   float3 res;
   norm.x = norm.x * 2.0 - 1.0;
   res.xy = (2.0 * abs(norm)) - float2(1,1);
   res.z = (norm.x < 0 ? -1.0 : 1.0) * sqrt(abs(1 - res.x*res.x - res.y*res.y));
   return res;
}

float linearize_depth(float depth)
{
	return 2.0 * pos_decompression_params.z * pos_decompression_params.w 
		/ (pos_decompression_params.w + pos_decompression_params.z - (depth * 2.0 - 1.0) 
		* (pos_decompression_params.w - pos_decompression_params.z));
}

float delinearize_depth(float depth)
{
	return 0.5 * ((pos_decompression_params.w + pos_decompression_params.z) 
		/ (pos_decompression_params.w - pos_decompression_params.z) - (2.0 * pos_decompression_params.z / depth) 
		/ (pos_decompression_params.w - pos_decompression_params.z) + 1.0);
}

float gbuf_pack_position(float3 position)
{
   return position.z;
}

float3 gbuf_unpack_position(float position, float2 tc)
{
	float3 pos;
	pos.z = position.x;
	pos.xy = pos.z * (tc * 2.0f * pos_decompression_params.xy - pos_decompression_params.xy);
	return pos;
}
#endif
////////////////////////////////////////////////////////////////////////////

GBufferPacked PackGBuffer(GBuffer Input)
{
    GBufferPacked GBuffer;

#if GBUFFER_OPT_MODE > 0
    GBuffer.rt_Albedo = float4(Input.Albedo, Input.Glossiness); // .rgb = Albedo, .a = Gloss
    GBuffer.rt_Normal = float4(gbuf_pack_normal(Input.Normal), Input.AO, Input.BakedAO); // .rg = Normal, .b = Hemi, .a = BackedAO
 #if GBUFFER_OPT_MODE == 1
    GBuffer.rt_Position = float4(gbuf_pack_position(Input.Position), 0, 0, 0); // .r = Depth
 #endif
#else
    GBuffer.rt_Albedo   = float4(Input.Albedo,   Input.Glossiness); // .rgb = Albedo,   .a = Gloss
    GBuffer.rt_Normal   = float4(Input.Normal,   Input.AO);         // .rgb = Normal,   .a = Hemi
    GBuffer.rt_Position = float4(Input.Position, Input.BakedAO);    // .rgb = Position, .a = BackedAO
#endif

    return GBuffer;
}

float3 GetPosition(float2 TexCoords)
{
#if GBUFFER_OPT_MODE == 1
	float  Depth    = tex2D(s_gbuffer_position, TexCoords).r; // .r = Depth
	Depth = Depth < pos_decompression_params.z ? pos_decompression_params.w : Depth;
    float3 Position = gbuf_unpack_position(Depth, TexCoords);
#elif GBUFFER_OPT_MODE == 2
	float  Depth    = tex2D(s_zb, TexCoords).a; // .a = hw deph
	Depth = linearize_depth(Depth);
    float3 Position = gbuf_unpack_position(Depth, TexCoords);
#elif GBUFFER_OPT_MODE == 3
	float4 BGRA = tex2D(s_zb, TexCoords); // BGRA format, b - stencil, gra - depth
	float Depth = dot(BGRA.arg, float3(0.9960938, 0.0038909, 1.5199185e-5)); // represent to 24bit float
	Depth = linearize_depth(Depth);
    float3 Position = gbuf_unpack_position(Depth, TexCoords);
#else
    float3 Position = tex2Dlod0(s_gbuffer_position, TexCoords);
    if (Position.z < pos_decompression_params.z)
        Position.z = pos_decompression_params.w;
#endif

    return Position;
}

GBuffer UnpackGBuffer(float2 TexCoords)
{
    GBuffer GBuffer;

#if GBUFFER_OPT_MODE > 0
	float4 rt_Albedo   = tex2D(s_gbuffer_albedo,   TexCoords); // .rgb = Albedo, .a = Gloss
	float4 rt_Normal   = tex2D(s_gbuffer_normal,   TexCoords); // .rg = Normal, .b = Hemi, .a = BackedAO
	
 #if GBUFFER_OPT_MODE == 1
	float  rt_Depth    = tex2D(s_gbuffer_position, TexCoords).r; // .r = Depth
	rt_Depth = rt_Depth < pos_decompression_params.z ? pos_decompression_params.w : rt_Depth;
 #elif GBUFFER_OPT_MODE == 2
	float  rt_Depth    = tex2D(s_zb, TexCoords).a; // .a = hw deph
	rt_Depth = linearize_depth(rt_Depth);
 #elif GBUFFER_OPT_MODE == 3
	float4 BGRA = tex2D(s_zb, TexCoords); // BGRA format, b - stencil, gra - depth
	float rt_Depth = dot(BGRA.arg, float3(0.9960938, 0.0038909, 1.5199185e-5)); // represent to 24bit float
	rt_Depth = linearize_depth(rt_Depth);
    float3 Position = gbuf_unpack_position(rt_Depth, TexCoords);
 #endif

    GBuffer.Albedo     = rt_Albedo.rgb;
    GBuffer.Glossiness = rt_Albedo.a;
    GBuffer.Normal     = gbuf_unpack_normal(rt_Normal.rg);
    GBuffer.AO         = rt_Normal.b;
    GBuffer.Position   = gbuf_unpack_position(rt_Depth, TexCoords);
    GBuffer.BakedAO    = rt_Normal.a;
#else
	float4 rt_Albedo   = tex2D(s_gbuffer_albedo,   TexCoords); // .rgb = Albedo,   .a = Gloss
	float4 rt_Normal   = tex2D(s_gbuffer_normal,   TexCoords); // .rgb = Normal,   .a = Hemi
	float4 rt_Position = tex2D(s_gbuffer_position, TexCoords); // .rgb = Position, .a = BackedAO
	
    GBuffer.Albedo     = rt_Albedo.rgb;
    GBuffer.Glossiness = rt_Albedo.a;
    GBuffer.Normal     = rt_Normal.rgb;
    GBuffer.AO         = rt_Normal.a;
    GBuffer.Position   = rt_Position.rgb;
    GBuffer.BakedAO    = rt_Position.a;
	
    if (GBuffer.Position.z < pos_decompression_params.z)
        GBuffer.Position.z = pos_decompression_params.w;
#endif

    return GBuffer;
}

////////////////////////////////////////////////////////////////////////////