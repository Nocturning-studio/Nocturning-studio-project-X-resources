#include "common.h"

struct vf
{
	// float4 hpos	: POSITION;
	float4 c	: COLOR0;
	float  fog	: FOG;
    float4 hpos: SV_Position;
};

vf main (v_vert v)
{
	vf 		o;

	o.hpos 		= mul			(m_VP, v.P);			// xform, input in world coords
	o.c 		= v.color;
	o.fog 		= CalcVertexFogness(v.P);				// fog, input in world coords

	return o;
}
