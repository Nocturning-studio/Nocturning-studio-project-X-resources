#include "common.h"

struct vi
{
        vector4 p: POSITION;
        vector4 c: COLOR0;
        vector3 tc0: TEXCOORD0;
        vector3 tc1: TEXCOORD1;
};

struct vf
{
        vector4 hpos: POSITION;
        vector4 c: COLOR0;
        vector3 tc0: TEXCOORD0;
        vector3 tc1: TEXCOORD1;
};

vf main (vi v)
{
    vf o;

    vector4 tpos = mul (1000.0h, v.p);
    o.hpos = mul (m_WVP, tpos);
    o.hpos.z = o.hpos.w;
    o.tc0 = v.tc0;
    o.tc1 = v.tc1;
    o.c = v.c;
    
    return o;
}