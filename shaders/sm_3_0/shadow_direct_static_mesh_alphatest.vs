#include "common.h"

struct 	a2v
{
	vector4 P:	 	POSITION;	// Object-space position
 	vector4 tc0:		TEXCOORD0;	// Texture coordinates
};

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
v_shadow_direct_aref 	main	( v_static  	I )
{
	v_shadow_direct_aref 		O;
	O.hpos 	= mul				(m_WVP,	I.P		);
	O.tc0 	= unpack_tc_base	(I.tc,I.T.w,I.B.w	);	// copy tc

 	return	O;
}

