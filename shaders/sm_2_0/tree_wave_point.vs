#include "common.h"

struct av 
{
	half4 	pos	: POSITION;	// (half,half,half,1)
	half4 	nc	: NORMAL;	// (half,half,half,clr)
	half4 	misc	: TEXCOORD0;	// (u(Q),v(Q),frac,???)
};

uniform half3x4	m_xform;
uniform half4 		consts;		// {1/quant,1/quant,???,???}
uniform half4 		wave; 		// cx,cy,cz,tm
uniform half4 		wind; 		// direction2D
uniform half4		c_bias;		// + color
uniform half4		c_scale;	// * color
uniform half2 		c_sun;		// x=*, y=+

vf_point main (av v)
{
	vf_point	o;

	// Transform to world coords
	half3 	pos	= mul	(m_xform, v.pos);

	// 
	half 	base 	= m_xform._24;			// take base height from matrix
	half 	dp	= calc_cyclic  (wave.w+dot(pos,(half3)wave));
	half 	H 	= pos.y - base;			// height of vertex (scaled, rotated, etc.)
	half 	frac 	= v.misc.z*consts.x;		// fractional (or rigidity)
	half 	inten 	= H * dp;			// intensity
	half2 	result	= calc_xz_wave	(wind.xz*inten, frac);
	half4 	f_pos 	= half4(pos.x+result.x, pos.y, pos.z+result.y, 1);
	half3 	f_N 	= normalize 	(mul (m_xform,  unpack_normal(v.nc)));

	// Final xform
	o.hpos		= mul		(m_VP, f_pos);
	o.tc0		= (v.misc * consts).xy;
	o.color		= calc_point 	(o.tc1,o.tc2,f_pos,f_N);

	return o;
}
