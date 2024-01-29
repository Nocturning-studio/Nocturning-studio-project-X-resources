#include "common.h"

struct vf
{
	vector4 hpos	: POSITION	;
	vector4 C 	: COLOR0	;
};

uniform vector4 		tfactor;
vf main (vector4	P:POSITION)
{
	vf 		o;

	o.hpos 		= mul			(m_WVP, P);			// xform, input in world coords
	o.C 		= tfactor;

	return o;
}
