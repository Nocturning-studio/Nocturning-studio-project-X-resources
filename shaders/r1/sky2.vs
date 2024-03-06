#include "common.h"

struct vi
{
	half4	p	: POSITION;
	half4	c	: COLOR0;
	half3	tc0	: TEXCOORD0;
	half3	tc1	: TEXCOORD1;
};

struct vf
{
	half4 	hpos	: POSITION;
	half4	c	: COLOR0;
	half3	tc0	: TEXCOORD0;
	half3	tc1	: TEXCOORD1;
};

vf main (vi v)
{
	vf 		o;

	float4	tpos	= mul(1000.0f, v.p);
        o.hpos          = mul(m_WVP, tpos);						// xform, input in world coords, 1000 - magic number
	o.hpos.z	= o.hpos.w;
	o.c		= v.c;				// copy color
	o.tc0		= v.tc0;			// copy tc
	o.tc1		= v.tc1;			// copy tc

	return o;
}
