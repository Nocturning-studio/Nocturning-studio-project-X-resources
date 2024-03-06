#include "common.h"

struct vf
{
	half4 hpos	: POSITION;
	half2 tc0	: TEXCOORD0;
	half  fog	: FOG;
};

vf main (v_vert v)
{
	vf 		o;

	o.hpos 		= mul			(m_VP, v.P);			// xform, input in world coords
	o.tc0		= unpack_tc_base	(v.uv,v.T.w,v.B.w);		// copy tc
	o.fog 		= CalcVertexFogness(v.P);				// fog, input in world coords

	return o;
}
