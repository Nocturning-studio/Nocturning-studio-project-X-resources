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
	half2 tc0	: TEXCOORD0;
	half2 tc1	: TEXCOORD1;		// detail
	half4 c0	: COLOR0;		// c0=all lighting
	half4 c1	: COLOR1;		// ps_1_1 read ports
	half  fog	: FOG;
};

uniform float3x4	m_xform;
uniform float4 		consts;		// {1/quant,1/quant,???,???}
uniform float4 		wave; 		// cx,cy,cz,tm
uniform float4 		wind; 		// direction2D
uniform float4		c_bias;		// + color
uniform float4		c_scale;	// * color
uniform float2 		c_sun;		// x=*, y=+

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
	half4 	f_pos 	= half4(pos.x+result.x, pos.y, pos.z+result.y, 1);

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

	// final xform, color, tc
	o.tc0.xy	= (v.misc * consts).xy;
	o.tc1		= o.tc0*dt_params;					// dt tc
	half2	dt 	= calc_detail		(f_pos);			// 
	o.c0		= half4 		(L_final,dt.x);			//
	o.c1		= dt.y;							//

	return o;
}
