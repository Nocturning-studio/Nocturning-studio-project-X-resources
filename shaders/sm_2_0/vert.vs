#include "common.h"

struct vf
{
	half4 hpos	: POSITION;
	half2 tc0	: TEXCOORD0;
	half3 c0	: COLOR0;		// c0=all lighting
	half  fog	: FOG;
};

vf main (v_vert v)
{
	vf 		o;

	half3 	N 	= unpack_normal		(v.N);
	o.hpos 		= mul			(m_VP, v.P);			// xform, input in world coords
	o.tc0		= unpack_tc_base	(v.uv,v.T.w,v.B.w);		// copy tc
//	o.tc0		= unpack_tc_base	(v.tc);				// copy tc

	half3 	L_rgb 	= v.color.xyz;						// precalculated RGB lighting
	half3 	L_hemi 	= v_hemi(N)*v.N.w;					// hemisphere
	half3 	L_sun 	= v_sun(N)*v.color.w;					// sun
	half3 	L_final	= L_rgb + L_hemi + L_sun + L_ambient;

	o.c0		= L_final;
	o.fog 		= calc_fogging 		(v.P);			// fog, input in world coords

	return o;
}
