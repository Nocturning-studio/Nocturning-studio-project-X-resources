#include "common.h"

struct vf
{
	half4 P: POSITION	;
	half4 C: COLOR0	;
};

uniform half4 		tfactor;
vf main (vf i)
{
	vf 		o;

	o.P 		= mul			(m_WVP, i.P);			// xform, input in world coords
	o.C 		= tfactor*i.C;

	return o;
}
