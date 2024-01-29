#include "common.h"

struct vf
{
	vector4 P: POSITION	;
	vector4 C: COLOR0	;
};

uniform vector4 		tfactor;
vf main (vf i)
{
	vf 		o;

	o.P 		= mul			(m_WVP, i.P);			// xform, input in world coords
	o.C 		= tfactor*i.C;

	return o;
}
