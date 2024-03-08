//////////////////////////////////////////////////////////////////////////////////////////
//	Created		: 22.12.2022
//	Author		: Deathman
//	Basic idea 	: xRay engine 2.0 sm 4.0 
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef	COMMON_IOSTRUCTS_H_INCLUDED
#define	COMMON_IOSTRUCTS_H_INCLUDED
//////////////////////////////////////////////////////////////////////////////////////////
struct 	v_lmap
{
	float4 	P	: POSITION;	// (float,float,float,1)
	float4	N	: NORMAL;	// (nx,ny,nz,hemi occlusion)
	float4 	T	: TANGENT;
	float4 	B	: BINORMAL;
	float2 	uv0	: TEXCOORD0;	// (base)
	float2	uv1	: TEXCOORD1;	// (lmap/compressed)
};
struct	v_vert
{
	float4 	P	: POSITION;	// (float,float,float,1)
	float4	N	: NORMAL;	// (nx,ny,nz,hemi occlusion)
	float4 	T	: TANGENT;
	float4 	B	: BINORMAL;
	float4	color	: COLOR0;	// (r,g,b,dir-occlusion)
	float2 	uv	: TEXCOORD0;	// (u0,v0)
};

struct	v_detail
{
	float4 	pos	: POSITION;	// (float,float,float,1)
	int4 	misc	: TEXCOORD0;	// (u(Q),v(Q),frac,matrix-id)
};
struct 	vf_spot
{
	float4 hpos	: POSITION;
	float2 tc0	: TEXCOORD0;	// base
	float4 tc1	: TEXCOORD1;	// lmap, projected
	float2 tc2	: TEXCOORD2;	// att + clipper
	float4 color	: COLOR0;
};
struct 	vf_point
{
	float4 hpos	: POSITION;
	float2 tc0	: TEXCOORD0;	// base
	float2 tc1	: TEXCOORD1;	// att1 + clipper
	float2 tc2	: TEXCOORD2;	// att2 + clipper
	float4 color	: COLOR0;
};
//////////////////////////////////////////////////////////////////////////////////////////
#endif	//	COMMON_IOSTRUCTS_H_INCLUDED