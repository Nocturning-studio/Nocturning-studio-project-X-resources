#include "common.h"

struct vf
{
	half4 hpos	: POSITION;
	half4 c0	: COLOR0;		// c0=all lighting
};

vf main (v_vert v)
{
	vf 		o;

	half3 	N 	= unpack_normal		(v.N);
	half3 	L_rgb 	= v.color.xyz;						// precalculated RGB lighting
	half3 	L_hemi 	= v_hemi(N)*v.N.w;					// hemisphere
	half 	L_sun 	= v.color.w;						// sun occl only
	half3 	L_final	= L_rgb + L_hemi + L_ambient;

	o.hpos 		= mul			(m_VP, v.P);			// xform, input in world coords
	o.c0 		= half4 		(L_final.x,L_final.y,L_final.z,L_sun);
	return o;
}
