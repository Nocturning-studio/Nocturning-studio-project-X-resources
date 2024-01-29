#include "common.h"

struct v_lmap
{
	vector4 pos: POSITION; // (vector,vector,vector,1)
	vector4 norm: NORMAL; // (nx,ny,nz,hemi occlusion)
	vector2 tc0: TEXCOORD0; // (base)
	vector2 tc1: TEXCOORD1; // (lmap/compressed)
};

struct vf
{
	vector4 hpos: POSITION;
	vector2 tc0: TEXCOORD0;
	vector2 tc1: TEXCOORD1;
	vector2 tch: TEXCOORD2;
	vector3 tc2: TEXCOORD3;
	vector3 position: TEXCOORD4;
	vector3 c0: COLOR0; // c0=hemi+v-lights, 	c0.a = dt*
	vector3 c1: COLOR1; // c1=sun, 		c1.a = dt+
};

vf main (v_static v)
{
    vf o;

	vector3 pos_w = v.P;
	vector3 norm_w = normalize (unpack_normal (v.Nh));
	
    o.hpos = mul (m_VP, v.P); // xform, input in world coords
    o.tc0 = unpack_tc_base (v.tc, v.T.w, v.B.w); // copy tc
    o.tc1 = unpack_tc_lmap (v.lmh); // copy tc 
    o.tch = o.tc1;
    o.tc2 = calc_reflection (pos_w, norm_w);
    o.position = mul (m_WV, v.P);
    o.c0 = v_hemi (norm_w); // just hemisphere
    o.c1 = v_sun (norm_w); // sun

    return o;
}
