#include "common.h"

struct vi
{
	vector4 p: POSITION;
	vector4 dir: COLOR0; // dir0,dir1(w<->z)
	vector4 color: COLOR1; // rgb. intensity
};

struct vf
{
	vector4 hpos: POSITION;
	vector4 color: COLOR0;
  	vector2 tc0: TEXCOORD0;
  	vector2 tc1: TEXCOORD1;
};

vf main (vi v)
{
    vf o;

    o.hpos = mul (m_WVP, v.p); // xform, input in world coords
	
//	if (length(vector3(v.p.x,0,v.p.z))>CLOUD_FADE)	o.color.w = 0	;

	// generate tcs
	vector2 d0 = v.dir.xy * 2 - 1;
	vector2 d1 = v.dir.wz * 2 - 1;
	vector2 _0 = v.p.xz * CLOUD_TILE0 + d0 * timers.z * CLOUD_SPEED0;
	vector2 _1 = v.p.xz * CLOUD_TILE1 + d1 * timers.z * CLOUD_SPEED1;
    o.tc0 = _0; // copy tc
    o.tc1 = _1; // copy tc

    o.color = v.color; // copy color, low precision, cannot prescale even by 2
    o.color.w *= pow (v.p.y, 25);

    return o;
}
