#include "common.h"

struct vf
{
    // float4 hpos: POSITION;
    float2 tc: TEXCOORD0;
    float4 C: COLOR0;
    float4 hpos: SV_Position;
};

// uniform float4 array[200]: register(c10);

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

	// Final out
    o.hpos = mul (m_WVP, pos);
    o.C = c0;
    o.tc.xy = (v.misc * det_consts).xy;
    
    return o;
}
