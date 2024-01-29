#include "common.h"

struct vi
{
	half4	p	: POSITION	;
	half4	dir 	: COLOR0	;	// dir0,dir1(w<->z)
	half4	color	: COLOR1	;	// rgb. intensity
};

struct 	vf
{
	half4 	hpos	: POSITION	;
	half4	color	: COLOR0	;	// rgb. intensity
  	half2	tc0	: TEXCOORD0	;
  	half2	tc1	: TEXCOORD1	;
};

vf main (vi v)
{
	vf 		o;

	o.hpos 		= mul		(m_WVP, v.p);	// xform, input in world coords
	o.color		= v.color;			// copy color
	
	if (length(half3(v.p.x,0,v.p.z))>CLOUD_FADE)	o.color.w = 0	;

	// generate tcs
	half2 d0	= v.dir.xy*2-1;
	half2 d1	= v.dir.wz*2-1;
	o.tc0		= v.p.xz * CLOUD_TILE0 + d0*timers.z*CLOUD_SPEED0;
	o.tc1		= v.p.xz * CLOUD_TILE1 + d1*timers.z*CLOUD_SPEED1;

	return o;
}
