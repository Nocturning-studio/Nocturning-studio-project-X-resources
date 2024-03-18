#include "common_iostructs.h"

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
v2p_TL2uv main ( v_TL2uv I )
{
	v2p_TL2uv	O;

	O.hpos = I.P;
	O.Tex0 = I.Tex0;
	O.Tex1 = I.Tex1;
	//	Some shaders that use this stub don't need Color at all
	O.Color = I.Color;	//	swizzle vertex colour

 	return O;
}