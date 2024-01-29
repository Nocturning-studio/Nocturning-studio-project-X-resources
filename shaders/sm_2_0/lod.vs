#include "common.h"

struct vv
{
	half3 pos0	: POSITION0	;
	half3 pos1	: POSITION1	;
	half3 n0	: NORMAL0	;
	half3 n1	: NORMAL1	;
	half2 tc0	: TEXCOORD0	;
	half2 tc1	: TEXCOORD1	;
	half4 rgbh0	: TEXCOORD2;	// rgb.h
	half4 rgbh1	: TEXCOORD3;	// rgb.h
	half4 sun_af	: COLOR0;	// x=sun_0, y=sun_1, z=alpha, w=factor
};
struct vf
{
	half4 	hpos	: POSITION;
 	half2 	tc0	: TEXCOORD0;	// base0
 	half2 	tc1	: TEXCOORD1;	// base1
 	half2 	tc2	: TEXCOORD2;	// hemi0
 	half2 	tc3	: TEXCOORD3;	// hemi1
	half4 	c	: COLOR0;	// color.alpha
	half4 	f	: COLOR1;	// factor
	half  	fog	: FOG	;
};

#define L_SCALE 	(1.55)
#define L_SUN_HACK 	(.7)
vf main (vv v)
{
	vf 		o;

	// lerp pos
	half 	factor 	= v.sun_af.w	;
	half4 	pos 	= half4	(lerp(v.pos0,v.pos1,factor),1);
	o.hpos 		= mul		(m_VP, pos);				// xform, input in world coords

	// replicate TCs
	o.tc0		= v.tc0;						
	o.tc1		= v.tc1;						
	o.tc2		= v.tc0;
	o.tc3		= v.tc1;

	// calc normal & lighting
	half3 	normal 	= 	normalize	(lerp(v.n0,v.n1,factor));
		normal.y+= 	1;
		normal 	= 	normalize 	(normal);
	half4 	rgbh 	= lerp	(v.rgbh0,v.rgbh1,factor)		*L_SCALE;
	half 	sun 	= lerp 	(v.sun_af.x, v.sun_af.y, factor)	*L_SCALE;
	half 	sun_c 	= 1 + L_SUN_HACK * dot(normal, L_sun_dir_w);		// [1+-delta], normal already inverted

	half3 	L_rgb 	= rgbh.rgb;						// precalculated RGB lighting
	half3 	L_hemi 	= L_hemi_color * rgbh.w;				// hemisphere
	half3 	L_sun 	= L_sun_color  * sun * sun_c;				// sun
	half3 	L_final	= L_rgb + L_hemi + L_sun + L_ambient;

	o.c		= half4	(L_final,v.sun_af.z);
	o.f 		= factor	;
	o.fog 		= calc_fogging 	(pos);					// fog, input in world coords

	return o	;
}
