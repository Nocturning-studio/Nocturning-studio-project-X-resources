uniform half4x4 	m_texgen;

#define	NEED_SOFT_WATER

#include "common.h"

struct	v_verx
{
	half4 	P	: POSITION;	// (half,half,half,1)
	half4	N	: NORMAL;	// (nx,ny,nz,hemi occlusion)
	half4 	T	: TANGENT;
	half4 	B	: BINORMAL;
	half4	color	: COLOR0;	// (r,g,b,dir-occlusion)
	half2 	uv	: TEXCOORD0;	// (u0,v0)
};

struct vf
{
	half4 hpos	: POSITION	;
	half2 tbase	: TEXCOORD0	;
	half2 tdist0	: TEXCOORD1	;
	half2 tdist1	: TEXCOORD2	;
#ifdef	NEED_SOFT_WATER
	half4      tctexgen    :         TEXCOORD3        ;
#endif	//	NEED_SOFT_WATER	
};

vf main (v_verx v)
{
	vf 		o;

	half4 	P 	= v.P;
	half3 	N 	= unpack_normal		(v.N);
		P 	= watermove		(P);

	o.tbase		= unpack_tc_base	(v.uv,v.T.w,v.B.w);		// copy tc
	o.tdist0	= watermove_tc 		(o.tbase*W_DISTORT_BASE_TILE_0, P.xz, W_DISTORT_AMP_0);
	o.tdist1	= watermove_tc 		(o.tbase*W_DISTORT_BASE_TILE_1, P.xz, W_DISTORT_AMP_1);
	o.hpos 		= mul			(m_VP, P);			// xform, input in world coords

//	Igor: for additional depth dest
#ifdef	NEED_SOFT_WATER
	o.tctexgen = mul( m_texgen, P);
	half3	Pe	= mul		(m_V,  P);
	o.tctexgen.z = Pe.z;
#endif	//	NEED_SOFT_WATER

	return o;
}
