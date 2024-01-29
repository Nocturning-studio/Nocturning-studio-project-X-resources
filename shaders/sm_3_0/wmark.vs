#include "common.h"

struct vf
{
	vector4 hpos: POSITION;
	vector2 tc0: TEXCOORD0;
	vector3 position: TEXCOORD1;
	vector3 c0: COLOR0; // c0=all lighting
};

vf main (v_static v)
{
    vf o;

	vector3 N = unpack_normal (v.Nh);
	vector4 P = wmark_shift (v.P, N);
    o.hpos = mul (m_VP, P); // xform, input in world coords
    o.tc0 = unpack_tc_base (v.tc, v.T.w, v.B.w); // copy tc
    o.position = mul (m_WV, P);

	//vector3 	L_rgb 	= v.color.xyz;					// precalculated RGB lighting
	//vector3 	L_hemi 	= v_hemi(N)*v.norm.w;				// hemisphere
	//vector3 	L_sun 	= v_sun(N)*v.color.w;				// sun
	//vector3 	L_final	= L_rgb + L_hemi + L_sun + L_ambient		;

    o.c0 = 0; //L_final;

    return o;
}
