#include "common.h"

struct vf
{
	half4 hpos	: POSITION;
	half4 c	: COLOR0;
	half  fog	: FOG;
};

vf main (v_vert v)
{
	vf 		o;

	o.hpos 		= mul			(m_VP, v.P);			// xform, input in world coords
	o.c 		= v.color;
	o.fog 		= calc_fogging 		(v.P);				// fog, input in world coords

	return o;
}
