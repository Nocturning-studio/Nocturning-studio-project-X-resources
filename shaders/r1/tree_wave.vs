#include "common.h"

struct av 
{
	half4 	pos	: POSITION;	// (half,half,half,1)
	half4 	nc	: NORMAL;	// (half,half,half,clr)
	half4 	misc	: TEXCOORD0;	// (u(Q),v(Q),frac,???)
};

struct vf
{
	half4 HPOS	: POSITION;
	half4 COL0	: COLOR0;
	half2 TEX0	: TEXCOORD0;
	half  fog	: FOG;
};

vf main (av v)
{
	vf 		o;

	// Transform to world coords
	half3 	pos	= mul	(m_xform, v.pos);

	// 
	half 	base 	= m_xform._24;			// take base height from matrix
	half 	dp	= calc_cyclic  (wave.w+dot(pos,(half3)wave));
	half 	H 	= pos.y - base;			// height of vertex (scaled, rotated, etc.)
	half 	frac 	= v.misc.z*consts.x;		// fractional (or rigidity)
	half 	inten 	= H * dp;			// intensity
	half2 	result	= calc_xz_wave	(wind.xz*inten, frac);
	half4 	f_pos 	= half4	(pos.x+result.x, pos.y, pos.z+result.y, 1);

	// Calc fog
	o.fog 		= CalcVertexFogness(f_pos);

	// Final xform
	o.HPOS		= mul		(m_VP, f_pos);

	// Lighting
	half3 	N 	= normalize 	(mul (m_xform,  unpack_normal(v.nc)));
	half 	L_base 	= v.nc.w;								// base hemisphere
	half4 	L_unpack= c_scale*L_base+c_bias;						// unpacked and decompressed
	half3 	L_rgb 	= L_unpack.xyz;								// precalculated RGB lighting
	half3 	L_hemi 	= v_hemi_wrap(N,.75f)* L_unpack.w;					// hemisphere
	half3 	L_sun 	= v_sun_wrap (N,.25f)* (L_base*c_sun.x+c_sun.y);			// sun
	half3 	L_final	= L_rgb + L_hemi + L_sun + L_ambient;
	o.COL0		= half4 (L_final,1);

	// final xform, color, tc
	o.TEX0.xy	= (v.misc * consts).xy;

	return o;
}
