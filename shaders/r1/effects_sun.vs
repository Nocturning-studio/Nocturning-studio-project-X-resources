#include "common.h"

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
v2p_TL main ( v_TL I )
{
	v2p_TL O;

//	O.hpos = I.P;
	O.hpos = mul(m_VP, I.P);	// xform, input in world coords
	O.hpos.z = O.hpos.w;
	O.Tex0 = I.Tex0;
	O.Color = I.Color;		//	swizzle vertex colour

 	return O;
}