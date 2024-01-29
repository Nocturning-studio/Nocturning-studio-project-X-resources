#include "common.h"

struct vv
{
	vector4 P	: POSITION;
	vector2 tc	: TEXCOORD0;
	vector4 c	: COLOR0;
};

struct vf
{
	vector4 hpos	: POSITION;
	vector2 tc	: TEXCOORD0;
	vector4 c	: COLOR0;

//	Igor: for additional depth dest
#ifdef	USE_SOFT_PARTICLES
	vector4 tctexgen	: TEXCOORD1;
#endif	//	USE_SOFT_PARTICLES
	
	vector3 position: TEXCOORD2;
};

vf main (vv v)
{
	vf 		o;

	o.hpos 		= mul	(m_WVP, v.P);		// xform, input in world coords
//	o.hpos 		= mul	(m_VP, v.P);		// xform, input in world coords
    o.position = mul (m_WV, v.P);
	o.tc		= v.tc;				// copy tc
	o.c		= v.c;				// copy color

//	Igor: for additional depth dest
#ifdef	USE_SOFT_PARTICLES
	o.tctexgen 	= mul( mVPTexgen, v.P);
	o.tctexgen.z	= o.hpos.z;
#endif	//	USE_SOFT_PARTICLES

	return o;
}
