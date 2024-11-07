#include "common.xrh"

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
v_shadow_direct_aref 	main	( v_tree  	I )
{
	v_shadow_direct_aref 	O;

	// Transform to world coords
	float3 	pos	= mul		(m_xform , I.P);

	float4 	f_pos 	= float4(pos.x, pos.y, pos.z, 1);

	O.hpos 	= mul		(m_VP,	f_pos	);
	O.tc0 	= (I.tc * consts).xy;
 	return	O;
}

