#include "common.h"

struct vf
{
	half4 hpos	: POSITION	;
	half4 C 	: COLOR0	;
};

uniform half4 		tfactor;
vf main (half4	P:POSITION)
{
	vf 		o;

	o.hpos 		= mul			(m_WVP, P);			// xform, input in world coords
	o.C 		= tfactor;

	return o;
}
