#include "common.h"

struct vf
{
	// float4 hpos	: POSITION	;
	float2 tbase	: TEXCOORD0	;
	float2 tdist0	: TEXCOORD1	;
	float2 tdist1	: TEXCOORD2	;
    float4 hpos: SV_Position;
};

vf main (v_vert v)
{
	vf 		o;

	v.N = bgra_to_rgba(v.N);
	v.T = bgra_to_rgba(v.T);
	v.B = bgra_to_rgba(v.B);

	float4 	P 	= v.P;
	float3 	N 	= unpack_normal		(v.N);
		P 	= watermove		(P);

	o.tbase		= unpack_tc_base	(v.uv,v.T.w,v.B.w);		// copy tc
	o.tdist0	= watermove_tc 		(o.tbase*W_DISTORT_BASE_TILE_0, P.xz, W_DISTORT_AMP_0);
	o.tdist1	= watermove_tc 		(o.tbase*W_DISTORT_BASE_TILE_1, P.xz, W_DISTORT_AMP_1);
	o.hpos 		= mul			(m_VP, P);			// xform, input in world coords

	return o;
}
