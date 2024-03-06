#include "common.h"

struct vf
{
	half4 hpos	: POSITION;
	half2 tc0	: TEXCOORD0;		// base
	half4 c0	: COLOR0;		// color
};

vf main (v_vert v)
{
	vf 		o;

	o.hpos 		= mul			(m_WVP, v.P);		// xform, input in world coords
	o.tc0		= unpack_tc_base	(v.uv,v.T.w,v.B.w);	// copy tc
//	o.tc0		= unpack_tc_base	(v.tc);			// copy tc

	// calculate fade
	half3  dir_v 	= normalize		(mul(m_WV,v.P));
	half3 	norm_v 	= normalize 		(mul(m_WV,unpack_normal(v.N)));
	half 	fade 	= abs			(dot(dir_v,norm_v));
	o.c0		= fade;

	return o;
}
