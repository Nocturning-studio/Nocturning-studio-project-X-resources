#include "common.h"

struct vf
{
	float4 hpos: POSITION;
	float2 tc0: TEXCOORD0;
	float3 position: TEXCOORD1;
	float3 c0: COLOR0; // c0=all lighting
};

vf main (v_static v)
{
    vf o;

	float3 N = unpack_normal (v.Nh);
	float4 P = wmark_shift (v.P, N);
    o.hpos = mul (m_VP, P); // xform, input in world coords
    o.tc0 = unpack_tc_base (v.tc, v.T.w, v.B.w); // copy tc
    o.position = mul (m_WV, P);

	//float3 	L_rgb 	= v.color.xyz;					// precalculated RGB lighting
	//float3 	L_hemi 	= v_hemi(N)*v.norm.w;				// hemisphere
	//float3 	L_sun 	= v_sun(N)*v.color.w;				// sun
	//float3 	L_final	= L_rgb + L_hemi + L_sun + L_ambient		;

    o.c0 = 0; //L_final;

    return o;
}
