#include "common.h"

struct v_vert
{
	vector4 pos: POSITION; // (vector,vector,vector,1)
	vector4 color: COLOR0; // (r,g,b,dir-occlusion)
};
struct vf
{
	vector4 hpos: POSITION;
	vector4 c: COLOR0;
    vector fog: FOG;
};

vf main (v_vert v)
{
    vf o;

    o.hpos = mul (m_VP, v.pos); // xform, input in world coords
    o.c = v.color;
    o.fog = calc_fogging (v.pos); // fog, input in world coords
    o.fog = saturate (o.fog);
    o.c.rgb = lerp (fog_color, o.c, o.fog);
    o.c.a = o.fog;

    return o;
}

