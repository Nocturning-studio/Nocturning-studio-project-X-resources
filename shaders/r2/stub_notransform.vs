#include "common.h"

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
v2p_TL main ( v_TL I )
{
	v2p_TL O;

	O.hpos = I.P;
	O.Tex0 = I.Tex0;
	//	Some shaders that use this stub don't need Color at all
	O.Color = I.Color;	//	swizzle vertex colour

 	return O;
}