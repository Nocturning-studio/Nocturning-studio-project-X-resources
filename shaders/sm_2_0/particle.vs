#include "common.h"

struct vv
{
	half4 P	: POSITION;
	half2 tc	: TEXCOORD0;
	half4 c	: COLOR0;
};
struct vf
{
	half4 hpos	: POSITION;
	half2 tc	: TEXCOORD0;
	half4 c	: COLOR0;
	half  fog	: FOG;
};

vf main (vv v)
{
	vf 		o;

	o.hpos 		= mul	(m_WVP, v.P);		// xform, input in world coords
	o.tc		= v.tc;				// copy tc
	o.c		= v.c;				// copy color
	o.fog 		= calc_fogging (v.P);		// fog, input in world coords

	return o;
}
