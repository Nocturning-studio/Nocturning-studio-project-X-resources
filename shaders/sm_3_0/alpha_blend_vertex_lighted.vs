#include "common.h"

struct vf
{
	vector4 hpos: POSITION;
	vector2 tc0: TEXCOORD0;
	vector3 position: TEXCOORD1;
	vector3 c0: COLOR0; // c0=all lighting
    vector fog: FOG;
};

vf main (v_static v)
{
    vf o;

	vector3 N = unpack_normal (v.Nh);
    o.hpos = mul (m_VP, v.P);
    o.tc0 = unpack_tc_base (v.tc, v.T.w, v.B.w);
    o.position = mul (m_WV, v.P);

	vector3 L_rgb = v.color.xyz;
	vector3 L_hemi = v_hemi (N) * v.Nh.w;
	vector3 L_sun = v_sun (N) * v.color.w;
	vector3 L_final = L_rgb + L_hemi + L_sun + L_ambient;

    o.c0 = L_final;
    o.fog = calc_fogging (v.P);

    return o;
}
