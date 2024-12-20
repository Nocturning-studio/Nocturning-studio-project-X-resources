#include "common.xrh"
#include "skinning.xrh"

struct vf
{
	float4 hpos: POSITION;
	float2 tc0: TEXCOORD0; // base
	float3 tc1: TEXCOORD1; // environment
	float3 position: TEXCOORD2;
	float3 c0: COLOR0; // color
};

vf _main (v_model v)
{
    vf o;

	float4 pos = v.P;
	float3 pos_w = mul (m_W, pos);
    float3 norm_w = normalize(mul(m_W, float4(v.N, 1.0f)));

    o.hpos = mul (m_WVP, pos); // xform, input in world coords
    o.tc0 = v.tc.xy; // copy tc
    o.tc1 = calc_reflection (pos_w, norm_w);
    o.position = pos.xyz; //mul (m_WV, pos);
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
