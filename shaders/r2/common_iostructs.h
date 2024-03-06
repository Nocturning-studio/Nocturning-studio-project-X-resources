//////////////////////////////////////////////////////////////////////////////////////////
//	Created		: 27.07.2022
//	Author		: Deathman
//  Nocturning studio for NS Project X
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef COMMON_IOSTRUCTS_H_INCLUDED
#define COMMON_IOSTRUCTS_H_INCLUDED
//////////////////////////////////////////////////////////////////////////////////////////
//	This file contains io structs:
//	v_name	:	input for vertex shader.
//	v2p_name:	output for vertex shader.
//	p_name	:	input for pixel shader.
//////////////////////////////////////////////////////////////////////////////////////////
struct v_static
{
    float4 P : POSITION;    // (float,float,float,1)
    float4 Nh : NORMAL;     // (nx,ny,nz,hemi occlusion)
    float4 T : TANGENT;     // tangent
    float4 B : BINORMAL;    // binormal
    float2 tc : TEXCOORD0;  // (u,v)
    float2 lmh : TEXCOORD1; // (lmu,lmv)
    float4 color : COLOR0;  // (r,g,b,dir-occlusion)
};
//////////////////////////////////////////////////////////////////////////////////////////
struct v_tree
{
    float4 P : POSITION;   // (float,float,float,1)
    float4 Nh : NORMAL;    // (nx,ny,nz)
    float3 T : TANGENT;    // tangent
    float3 B : BINORMAL;   // binormal
    float4 tc : TEXCOORD0; // (u,v,frac,???)
};
//////////////////////////////////////////////////////////////////////////////////////////
struct v_model
{
    float4 P : POSITION;   // (float,float,float,1)
    float3 N : NORMAL;     // (nx,ny,nz)
    float3 T : TANGENT;    // (nx,ny,nz)
    float3 B : BINORMAL;   // (nx,ny,nz)
    float2 tc : TEXCOORD0; // (u,v)
};
//////////////////////////////////////////////////////////////////////////////////////////
struct v_shadow_direct_aref
{
    float4 hpos : POSITION; // Clip-space position 	(for rasterization)
    float2 tc0 : TEXCOORD1; // Diffuse map for aref
};

struct v_shadow_direct
{
    float4 hpos : POSITION; // Clip-space position 	(for rasterization)
};
//////////////////////////////////////////////////////////////////////////////////////////
#endif //	COMMON_IOSTRUCTS_H_INCLUDED
//////////////////////////////////////////////////////////////////////////////////////////