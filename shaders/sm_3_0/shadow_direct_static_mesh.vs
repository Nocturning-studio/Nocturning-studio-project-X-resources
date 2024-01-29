#include "common.h"

struct 	a2v
{
	vector4 P:	 	POSITION;	// Object-space position
 	vector4 tc0:		TEXCOORD0;	// Texture coordinates
};

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
v_shadow_direct 	main	( a2v  	I )
{
	v_shadow_direct	O;

	O.hpos 	= mul		(m_WVP,	I.P	);

 	return	O;
}

