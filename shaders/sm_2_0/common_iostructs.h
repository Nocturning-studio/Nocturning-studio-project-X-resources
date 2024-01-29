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
	half4 	P	: POSITION;	// (half,half,half,1)
	half4	N	: NORMAL;	// (nx,ny,nz,hemi occlusion)
	half4 	T	: TANGENT;
	half4 	B	: BINORMAL;
	half2 	uv0	: TEXCOORD0;	// (base)
	half2	uv1	: TEXCOORD1;	// (lmap/compressed)
};
struct	v_vert
{
	half4 	P	: POSITION;	// (half,half,half,1)
	half4	N	: NORMAL;	// (nx,ny,nz,hemi occlusion)
	half4 	T	: TANGENT;
	half4 	B	: BINORMAL;
	half4	color	: COLOR0;	// (r,g,b,dir-occlusion)
	half2 	uv	: TEXCOORD0;	// (u0,v0)
};
struct 	v_model
{
	half4 	pos	: POSITION;	// (half,half,half,1)
	half3	norm	: NORMAL;	// (nx,ny,nz)
	half3	T	: TANGENT;	// (nx,ny,nz)
	half3	B	: BINORMAL;	// (nx,ny,nz)
	half2	tc	: TEXCOORD0;	// (u,v)
#ifdef SKIN_COLOR
	half3 	rgb_tint;
#endif
};
struct	v_detail
{
	half4 	pos	: POSITION;	// (half,half,half,1)
	int4 	misc	: TEXCOORD0;	// (u(Q),v(Q),frac,matrix-id)
};
struct 	vf_spot
{
	half4 hpos	: POSITION;
	half2 tc0	: TEXCOORD0;	// base
	half4 tc1	: TEXCOORD1;	// lmap, projected
	half2 tc2	: TEXCOORD2;	// att + clipper
	half4 color	: COLOR0;
};
struct 	vf_point
{
	half4 hpos	: POSITION;
	half2 tc0	: TEXCOORD0;	// base
	half2 tc1	: TEXCOORD1;	// att1 + clipper
	half2 tc2	: TEXCOORD2;	// att2 + clipper
	half4 color	: COLOR0;
};
//////////////////////////////////////////////////////////////////////////////////////////
#endif	//	COMMON_IOSTRUCTS_H_INCLUDED