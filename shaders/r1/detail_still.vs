#include "common.h"

struct vf
{
	half4 hpos	: POSITION;
	half4 C	: COLOR0;
	half2 tc	: TEXCOORD0;
};

uniform float4 		array	[200] : register(c10);

vf main (v_detail v)
{
	vf 		o;

	// index
	int 	i 	= v.misc.w;
	half4  m0 	= array[i+0];
	half4  m1 	= array[i+1];
	half4  m2 	= array[i+2];
	half4  c0 	= array[i+3];

	// Transform to world coords
	half4 	pos;
 	pos.x 		= dot	(m0, v.pos);
 	pos.y 		= dot	(m1, v.pos);
 	pos.z 		= dot	(m2, v.pos);
	pos.w 		= 1;

	// Final out
	o.hpos		= mul	(m_WVP,pos);
	o.C		= c0;
	o.tc.xy		= (v.misc * consts).xy;

	return o;
}
