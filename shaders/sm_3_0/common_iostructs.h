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
    vector4 P : POSITION;    // (vector,vector,vector,1)
    vector4 Nh : NORMAL;     // (nx,ny,nz,hemi occlusion)
    vector4 T : TANGENT;     // tangent
    vector4 B : BINORMAL;    // binormal
    vector2 tc : TEXCOORD0;  // (u,v)
    vector2 lmh : TEXCOORD1; // (lmu,lmv)
    vector4 color : COLOR0;  // (r,g,b,dir-occlusion)
};
//////////////////////////////////////////////////////////////////////////////////////////
struct v_tree
{
    vector4 P : POSITION;   // (vector,vector,vector,1)
    vector4 Nh : NORMAL;    // (nx,ny,nz)
    vector3 T : TANGENT;    // tangent
    vector3 B : BINORMAL;   // binormal
    vector4 tc : TEXCOORD0; // (u,v,frac,???)
};
//////////////////////////////////////////////////////////////////////////////////////////
struct v_model
{
    vector4 P : POSITION;   // (vector,vector,vector,1)
    vector3 N : NORMAL;     // (nx,ny,nz)
    vector3 T : TANGENT;    // (nx,ny,nz)
    vector3 B : BINORMAL;   // (nx,ny,nz)
    vector2 tc : TEXCOORD0; // (u,v)
};
//////////////////////////////////////////////////////////////////////////////////////////
struct v_detail
{
    vector4 pos : POSITION; // (vector,vector,vector,1)
    int4 misc : TEXCOORD0;  // (u(Q),v(Q),frac,matrix-id)
};
//////////////////////////////////////////////////////////////////////////////////////////
struct v_shadow_direct_aref
{
    vector4 hpos : POSITION; // Clip-space position 	(for rasterization)
    vector2 tc0 : TEXCOORD1; // Diffuse map for aref
};

struct v_shadow_direct
{
    vector4 hpos : POSITION; // Clip-space position 	(for rasterization)
};
//////////////////////////////////////////////////////////////////////////////////////////
struct p_bumped
{
    vector4 hpos : POSITION;
#if defined(USE_R2_STATIC_SUN) && !defined(USE_LIGHTMAP)
    vector4 tcdh : TEXCOORD0; // Texture coordinates with sun occlusion
#else
    vector2 tcdh : TEXCOORD0; // Texture coordinates
#endif
    vector4 position : TEXCOORD1; // position + hemi
    vector3 M1 : TEXCOORD2;       // nmap 2 eye - 1
    vector3 M2 : TEXCOORD3;       // nmap 2 eye - 2
    vector3 M3 : TEXCOORD4;       // nmap 2 eye - 3
#ifdef USE_TDETAIL
    vector2 tcdbump : TEXCOORD5; // d-bump
#ifdef USE_LIGHTMAP
    vector2 lmh : TEXCOORD6; // lm-hemi
#endif
#else
#ifdef USE_LIGHTMAP
    vector2 lmh : TEXCOORD5; // lm-hemi
#endif
#endif
};
//////////////////////////////////////////////////////////////////////////////////////////
struct p_flat
{
    vector4 hpos : POSITION;
#if defined(USE_R2_STATIC_SUN) && !defined(USE_LIGHTMAP)
    vector4 tcdh : TEXCOORD0; // Texture coordinates with sun occlusion
#else
    vector2 tcdh : TEXCOORD0; // Texture coordinates
#endif
    vector4 position : TEXCOORD1; // position + hemi
    vector3 N : TEXCOORD2;        // Eye-space normal        (for lighting)
#ifdef USE_TDETAIL
    vector2 tcdbump : TEXCOORD3; // d-bump
#ifdef USE_LIGHTMAP
    vector2 lmh : TEXCOORD4; // lm-hemi
#endif
#else
#ifdef USE_LIGHTMAP
    vector2 lmh : TEXCOORD3; // lm-hemi
#endif
#endif
};
//////////////////////////////////////////////////////////////////////////////////////////
struct p_screen
{
    vector4 hpos : POSITION;
    vector2 tc0 : TEXCOORD0; // Texture coordinates 	(for sampling maps)
};
//////////////////////////////////////////////////////////////////////////////////////////
#endif //	COMMON_IOSTRUCTS_H_INCLUDED
