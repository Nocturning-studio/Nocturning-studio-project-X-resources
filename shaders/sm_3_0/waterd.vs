#include "common.h"

struct v_vert
{
	vector4 P: POSITION; // (vector,vector,vector,1)
	vector4 N: NORMAL; // (nx,ny,nz,hemi occlusion)
	vector4 T: TANGENT;
	vector4 B: BINORMAL;
	vector4 color: COLOR0; // (r,g,b,dir-occlusion)
	vector2 uv: TEXCOORD0; // (u0,v0)
};
struct vf
{
	vector4 hpos: POSITION;
	vector2 tbase: TEXCOORD0;
	vector2 tdist0: TEXCOORD1;
	vector2 tdist1: TEXCOORD2;
#ifdef	USE_SOFT_WATER
#ifdef	NEED_SOFT_WATER
	vector4 tctexgen: TEXCOORD3;
#endif	//	USE_SOFT_WATER
#endif	//	NEED_SOFT_WATER
	vector3 position: TEXCOORD4;
};

vf main (v_vert v)
{
    vf o;

	vector4 P = watermove (v.P);
	vector3 N = unpack_normal (v.N);

    o.tbase = unpack_tc_base (v.uv, v.T.w, v.B.w); // copy tc
    o.tdist0 = watermove_tc (o.tbase * W_DISTORT_BASE_TILE_0, P.xz, W_DISTORT_AMP_0);
    o.tdist1 = watermove_tc (o.tbase * W_DISTORT_BASE_TILE_1, P.xz, W_DISTORT_AMP_1);
    o.hpos = mul (m_VP, P); // xform, input in world coords
    o.position = mul (m_WV, P);
//	Igor: for additional depth dest
#ifdef	USE_SOFT_WATER
#ifdef	NEED_SOFT_WATER
	o.tctexgen = mul( m_texgen, P);
	vector3	Pe	= mul(m_V,  P);
	o.tctexgen.z = Pe.z;
#endif	//	USE_SOFT_WATER
#endif	//	NEED_SOFT_WATER

    return o;
}
