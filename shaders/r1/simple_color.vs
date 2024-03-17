#include "common.h"

struct vf
{
	// float4 hpos	: POSITION	;
	float4 C 	: COLOR0	;
    float4 hpos: SV_Position;
};

uniform float4 		tfactor;
vf main (float4	P:POSITION)
{
	vf 		o;

	o.hpos 		= mul			(m_WVP, P);			// xform, input in world coords
	o.C 		= tfactor;

	return o;
}
