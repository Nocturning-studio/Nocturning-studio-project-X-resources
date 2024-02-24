#include "common.h"

struct vf
{
	half4 hpos	: POSITION;
	half2 tc0	: TEXCOORD0;
	half2 tc1	: TEXCOORD1;		// detail
	half4 c0	: COLOR0;		// c0=all lighting
	half4 c1	: COLOR1;		// ps_1_1 read ports
	half  fog	: FOG;
};

vf main (v_vert v)
{
	vf 		o;

	half3 	N 	= unpack_normal		(v.N);
	o.hpos 		= mul			(m_VP, v.P);			// xform, input in world coords
	o.tc0		= unpack_tc_base	(v.uv,v.T.w,v.B.w);		// copy tc
//	o.tc0		= unpack_tc_base	(v.tc);				// copy tc
	o.tc1		= o.tc0*dt_params;					// dt tc

	half3 	L_rgb 	= v.color.xyz;						// precalculated RGB lighting
	half3 	L_hemi 	= v_hemi(N)*v.N.w;					// hemisphere
	half3 	L_sun 	= v_sun(N)*v.color.w;					// sun
	half3 	L_final	= L_rgb + L_hemi + L_sun + L_ambient;

	half2	dt 	= calc_detail		(v.P);

	o.c0		= half4(L_final.x,L_final.y,L_final.z,dt.x);
	o.c1		= dt.y;							//
	o.fog 		= CalcVertexFogness(v.P);			// fog, input in world coords

	return o;
}
