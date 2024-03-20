#include "common.h"

struct vf
{
    // float4 hpos: POSITION;
    float2 tc: TEXCOORD0;
    float4 C: COLOR0;
    float4 hpos: SV_Position;
};

//uniform float4 dir2D;
//uniform float4 array[200]: register(c10);

cbuffer CBufferDetails
{
	float4 det_array[4096 - 3];
	float4 det_consts;
	float4 det_wave;
	float4 det_dir2D;
}

vf main (v_detail v)
{
    vf o;

	// index
    int i = v.misc.w;
    float4 m0 = det_array[i + 0];
    float4 m1 = det_array[i + 1];
    float4 m2 = det_array[i + 2];
    float4 c0 = det_array[i + 3];

	// Transform to world coords
    float4 pos;
    pos.x = dot (m0, v.pos);
    pos.y = dot (m1, v.pos);
    pos.z = dot (m2, v.pos);
    pos.w = 1;

	// 
    float base = m1.w;
    float dp = calc_cyclic (dot (pos, det_wave));
    float H = pos.y - base; // height of vertex (scaled)
    float frac = v.misc.z * det_consts.x; // fractional
    float inten = H * dp;
    float2 result = calc_xz_wave (det_dir2D.xz * inten, frac);
    pos = float4 (pos.x + result.x, pos.y, pos.z + result.y, 1);
    o.hpos = mul (m_WVP, pos);

	// Fake lighting
    float dpc = max (0.f, dp);
    // o.C = c0 * (det_consts.w + det_consts.z * dpc * frac);
    o.C = c0;

	// final xform, color, tc
    o.tc.xy = (v.misc * det_consts).xy;

    return o;
}