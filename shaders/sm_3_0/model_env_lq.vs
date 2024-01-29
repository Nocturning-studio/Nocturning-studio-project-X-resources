#include "common.h"
#include "skinning.h"

struct vf
{
	vector4 hpos: POSITION;
	vector2 tc0: TEXCOORD0; // base
	vector3 tc1: TEXCOORD1; // environment
	vector3 position: TEXCOORD2;
	vector3 c0: COLOR0; // color
};

vf _main (v_model v)
{
    vf o;

	vector4 pos = v.P;
	vector3 pos_w = mul (m_W, pos);
	vector3 norm_w = normalize (mul (m_W, v.N));

    o.hpos = mul (m_WVP, pos); // xform, input in world coords
    o.tc0 = v.tc.xy; // copy tc
    o.tc1 = calc_reflection (pos_w, norm_w);
    o.position = mul (m_WV, pos);
    o.c0 = calc_model_lq_lighting (norm_w);

    return o;
}

/////////////////////////////////////////////////////////////////////////
#ifdef 	SKIN_NONE
vf	main(v_model v) 		{ return _main(v); 		}
#endif

#ifdef 	SKIN_0
vf	main(v_model_skinned_0 v) 	{ return _main(skinning_0(v)); }
#endif

#ifdef	SKIN_1
vf	main(v_model_skinned_1 v) 	{ return _main(skinning_1(v)); }
#endif

#ifdef	SKIN_2
vf	main(v_model_skinned_2 v) 	{ return _main(skinning_2(v)); }
#endif
