#include "common.h"

struct vf
{
	half4 hpos	: POSITION;
	half2 tc0	: TEXCOORD0;
	half2 tc1	: TEXCOORD1;
	half2 tch	: TEXCOORD2;
	half2 tc2	: TEXCOORD3;
	half4 c0	: COLOR0;		// c0=hemi+v-lights, 	c0.a = dt*
	half4 c1	: COLOR1;		// c1=sun, 		c1.a = dt+
	half  fog	: FOG;
};

vf main (v_lmap v)
{
	vf 		o;

	half2 	dt 	= calc_detail		(v.P);
	half3	N 	= unpack_normal		(v.N);

	o.hpos 		= mul			(m_VP, v.P);			// xform, input in world coords
	o.tc0		= unpack_tc_base	(v.uv0,v.T.w,v.B.w);		// copy tc
//	o.tc0		= unpack_tc_base	(v.tc0);			// copy tc
	o.tc1		= unpack_tc_lmap	(v.uv1);			// copy tc 
	o.tch 		= o.tc1;
	o.tc2		= o.tc0*dt_params;					// dt tc
	o.c0		= half4 		(v_hemi(N),dt.x);		// c0=hemi+v-lights, 	c0.a = dt*
	o.c1 		= half4 		(v_sun(N),dt.y);		// c1=sun, 		c1.a = dt+
	o.fog 		= calc_fogging 		(v.P);				// fog, input in world coords

	return o;
}
