#include "common.h"
#include "skin.h"

struct vf
{
  half4 hpos  : POSITION;
  half2 tc0  : TEXCOORD0;    // base
  half4 c0  : COLOR0;      // color
};

vf   _main (v_model v)
{
  vf     o;

  o.hpos       = mul      (m_WVP, v.pos);      // xform, input in world coords
  o.tc0      = v.tc.xy;              // copy tc

  // calculate fade
  half3  dir_v   = normalize    (mul(m_WV,v.pos));
  half3  norm_v  = normalize    (mul(m_WV,v.norm));
  half   fade    = 0.9*abs      (dot(dir_v,norm_v));
  o.c0        = fade;

  return o;
}

/////////////////////////////////////////////////////////////////////////
#ifdef   SKIN_NONE
vf  main(v_model v)     { return _main(v);     }
#endif

#ifdef   SKIN_0
vf  main(v_model_skinned_0 v)   { return _main(skinning_0(v)); }
#endif

#ifdef  SKIN_1
vf  main(v_model_skinned_1 v)   { return _main(skinning_1(v)); }
#endif

#ifdef  SKIN_2
vf  main(v_model_skinned_2 v)   { return _main(skinning_2(v)); }
#endif