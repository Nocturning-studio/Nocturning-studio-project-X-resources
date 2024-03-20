#include "common.h"

struct av 
{
	float4 	pos	: POSITION;	// (float,float,float,1)
	float4 	nc	: NORMAL;	// (float,float,float,clr)
	int4 	misc	: TEXCOORD0;	// (u(Q),v(Q),frac,???)
};

struct vf
{
	// float4 hpos	: POSITION;
	float2 TEX0	: TEXCOORD0;
	float4 COL0	: COLOR0;
	float  fog	: COLOR2;
    float4 hpos: SV_Position;
};

vf main (av v)
{
	vf 		o;

	v.nc = bgra_to_rgba(v.nc);

	// Transform to world coords
	float3 	pos	= mul	(m_xform, v.pos);

	// 
	float 	base 	= m_xform._24;			// take base height from matrix
	float 	dp	= calc_cyclic  (wave.w+dot(pos,(float3)wave));
	float 	H 	= pos.y - base;			// height of vertex (scaled, rotated, etc.)
	float 	frac 	= v.misc.z*consts.x;		// fractional (or rigidity)
	float 	inten 	= H * dp;			// intensity
	float2 	result	= calc_xz_wave	(wind.xz*inten, frac);
	float4 	f_pos 	= float4	(pos.x+result.x, pos.y, pos.z+result.y, 1);

	// Calc fog
	o.fog 		= CalcVertexFogness(f_pos);

	// Final xform
	o.hpos		= mul		(m_VP, f_pos);

	// Lighting
	float3 	N 	= normalize 	(mul (m_xform,  unpack_normal(v.nc)));
	float 	L_base 	= v.nc.w;								// base hemisphere
	float4 	L_unpack= c_scale*L_base+c_bias;						// unpacked and decompressed
	float3 	L_rgb 	= L_unpack.xyz;								// precalculated RGB lighting
	float3 	L_hemi 	= v_hemi_wrap(N,.75f)* L_unpack.w;					// hemisphere
	float3 	L_sun 	= v_sun_wrap (N,.25f)* (L_base*c_sun.x+c_sun.y);			// sun
	float3 	L_final	= L_rgb + L_hemi + L_sun + L_ambient;
	o.COL0		= float4 (L_final,1);

	// final xform, color, tc
	o.TEX0.xy	= (v.misc * consts).xy;

	return o;
}
