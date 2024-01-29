#include "common.h"

struct vv
{
	vector4 P: POSITION;
	vector2 tc: TEXCOORD0;
	vector4 c: COLOR0;
};
struct vf
{
	vector4 hpos: POSITION;
	vector2 tc: TEXCOORD0;
	vector4 c: COLOR0;
};

vf main (vv v)
{
    vf o;

    o.hpos = mul (m_WVP, v.P); // xform, input in world coords
    o.hpos.z = abs (o.hpos.z);
    o.hpos.w = abs (o.hpos.w);
    o.tc = v.tc; // copy tc
    o.c = v.c; // copy color

    return o;
}
