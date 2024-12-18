#include "common.xrh"

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

	float3 N = unpack_normal (v.Nh.xyz);
    o.hpos = mul (m_VP, v.P);
    o.tc0 = unpack_tc_base (v.tc, v.T.w, v.B.w);
    o.position = mul(m_WV, v.P);

	float3 L_rgb = v.color.xyz;
	float3 L_hemi = v_hemi (N) * v.Nh.w;
	float3 L_sun = v_sun (N) * v.color.w;
	float3 L_final = L_rgb + L_hemi + L_sun + L_ambient.rgb;

    o.c0 = L_final;

    return o;
}
