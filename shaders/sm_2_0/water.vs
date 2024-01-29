#include "common.h"

struct vf
{
	half4 hpos	: POSITION	;
	half2 tbase	: TEXCOORD0	;
	half3 tenv	: TEXCOORD1	;
	half4 c0	: COLOR0	;	// c0=all lighting, c0.a = refl amount
	half  fog	: FOG;
};

vf main (v_vert v)
{
	vf 		o;

	half4 	P 	= v.P;
	half3 	N 	= unpack_normal	(v.N);
		
		P 	= watermove	(P);

	half2 	tc_base	= unpack_tc_base	(v.uv,v.T.w,v.B.w);		// copy tc

	half 	amount	;
	half3 	tc_refl	= waterrefl 		(amount, P,N);

	o.tbase		= tc_base;
	o.tenv		= tc_refl;

	half3 	L_rgb 	= v.color.xyz;						// precalculated RGB lighting
	half3 	L_hemi 	= v_hemi(N)*v.N.w;					// hemisphere
	half3 	L_sun 	= v_sun(N)*v.color.w;					// sun
	half3 	L_final	= L_rgb + L_hemi + L_sun + L_ambient;

	o.hpos 		= mul			(m_VP, P);			// xform, input in world coords
	o.c0		= half4		(L_final,amount);
	o.fog 		= calc_fogging 		(P);				// fog, input in world coords

	return o;
}
