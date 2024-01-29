#include "common.h"
#include "skin.h"

// #define SKIN_2

struct vf
{
	half4 hpos	: POSITION;
	half2 tc0	: TEXCOORD0;		// base
	half4 tc1	: TEXCOORD1;		// projected lmap
	half3 c0	: COLOR0;		// sun-color
	half4 c1	: COLOR1;		// lq-color + factor
	half  fog	: FOG;
};

vf 	_main (v_model v)
{
	vf 		o;

	half4 	pos 	= v.pos;
	half3  pos_w 	= mul			(m_W, pos);
	half4  pos_w4 	= half4		(pos_w,1);
	half3 	norm_w 	= normalize 		(mul(m_W,v.norm));

	o.hpos 		= mul			(m_WVP, pos);		// xform, input in world coords
	o.tc0		= v.tc.xy;					// copy tc
	o.tc1		= calc_model_lmap 	(pos_w);		// 
	o.c0 		= calc_sun		(norm_w);  		// sun
	o.c1 		= half4		(calc_model_lq_lighting(norm_w),m_plmap_clamp[0].w);
	o.fog 		= calc_fogging 		(pos_w4);		// fog, input in world coords

#ifdef SKIN_COLOR
	o.c1.rgb	*= v.rgb_tint;
	o.c1.w		= 1;
#endif

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
