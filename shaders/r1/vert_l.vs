#include "common.h"

struct vf
{
	// float4 hpos	: POSITION;
	float4 c0	: COLOR0;		// c0=all lighting
    float4 hpos: SV_Position;
};

vf main (v_vert v)
{
	vf 		o;

	// from DX9 to DX11 format
	// v.N = bgra_to_rgba(v.N);
	// v.T = bgra_to_rgba(v.T);
	// v.B = bgra_to_rgba(v.B);
	
	float3 	N 	= unpack_normal		(v.N);
	float3 	L_rgb 	= v.color.xyz;						// precalculated RGB lighting
	float3 	L_hemi 	= v_hemi(N)*v.N.w;					// hemisphere
	float 	L_sun 	= v.color.w;						// sun occl only
	float3 	L_final	= L_rgb + L_hemi + L_ambient;

	o.hpos 		= mul			(m_VP, v.P);			// xform, input in world coords
	o.c0 		= float4 		(L_final.x,L_final.y,L_final.z,L_sun);
	return o;
}
