#include "common.h"

struct vf
{
	half4 hpos	: POSITION;
	half4 C	: COLOR0;
	half2 tc	: TEXCOORD0;
};

uniform float4 		consts; // {1/quant,1/quant,diffusescale,ambient}
uniform float4 		wave; 	// cx,cy,cz,tm
uniform float4 		dir2D; 
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

	// 
	half 	base 	= m1.w;
	half 	dp	= calc_cyclic   (dot(pos,wave));
	half 	H 	= pos.y - base;			// height of vertex (scaled)
	half 	frac 	= v.misc.z*consts.x;		// fractional
	half 	inten 	= H * dp;
	half2 	result	= calc_xz_wave	(dir2D.xz*inten,frac);
	pos		= half4(pos.x+result.x, pos.y, pos.z+result.y, 1);
	o.hpos		= mul	(m_WVP,pos);

	// Fake lighting
	half 	dpc 	= max 	(0.f, dp);
	o.C		= c0 * (consts.w+consts.z*dpc*frac);

	// final xform, color, tc
	o.tc.xy	= (v.misc * consts).xy;

	return o;
}
