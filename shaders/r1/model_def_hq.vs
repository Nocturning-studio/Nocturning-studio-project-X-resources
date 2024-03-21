#include "common.h"
#include <skinning.h>

struct vf
{
    // float4 hpos: POSITION;
    float2 tc0: TEXCOORD0; // base
    float4 tc1: TEXCOORD1; // projected lmap
    float3 c0: COLOR0; // sun-color
    float4 c1: COLOR1; // lq-color + factor
    //float fog: COLOR2;
    float4 hpos: SV_Position;
};

vf _main (v_model v)
{
    vf o;

    float4 pos = v.P;
    float3 pos_w = mul (m_W, pos);
    float3 norm_w = normalize (mul (m_W, v.N));

    o.hpos = mul (m_WVP, pos); // xform, input in world coords
    o.tc0 = v.tc.xy; // copy tc
    o.tc1 = calc_model_lmap (pos_w); // 
    o.c0 = calc_sun (norm_w); // sun
    o.c1 = float4 (calc_model_lq_lighting (norm_w), m_plmap_clamp[0].w);
    //o.fog = CalcVertexFogness (pos_w); // fog, input in world coords

    return o;
}

/////////////////////////////////////////////////////////////////////////
#ifdef 	SKIN_NONE
vf	main(v_model v) 		{ return _main(v); 		}
#endif

#ifdef 	SKIN_0
vf	main(v_model_skinned_0 v) 	{ return _main(skinning_0(v)); 	}
#endif

#ifdef	SKIN_1
vf	main(v_model_skinned_1 v) 	{ return _main(skinning_1(v)); 	}
#endif

#ifdef	SKIN_2
vf	main(v_model_skinned_2 v) 	{ return _main(skinning_2(v)); 	}
#endif
