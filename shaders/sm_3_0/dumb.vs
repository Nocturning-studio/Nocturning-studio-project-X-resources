#include "common.h"

struct 	v2p
{
	vector4 	hpos: 		POSITION;	// Clip-space position 	(for rasterization)
};

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
v2p 	main	( vector4 P:	POSITION )
{
	v2p 		O;
	O.hpos 		= mul	(m_WVP,	  	P	);
 	return	O;
}

