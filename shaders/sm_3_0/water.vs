#include "common.h"

struct v_vert
{
        vector4 P: POSITION; // (vector,vector,vector,1)
        vector4 N: NORMAL; // (nx,ny,nz,hemi occlusion)
        vector4 T: TANGENT;
        vector4 B: BINORMAL;
        vector4 color: COLOR0; // (r,g,b,dir-occlusion)
        vector2 uv: TEXCOORD0; // (u0,v0)
};

struct vf
{
        vector4 hpos: POSITION;
        vector2 tbase: TEXCOORD0; // base
        vector2 tnorm0: TEXCOORD1; // nm0
        vector2 tnorm1: TEXCOORD2; // nm1
        vector3 M1: TEXCOORD3;
        vector3 M2: TEXCOORD4;
        vector3 M3: TEXCOORD5;
        vector3 v2point: TEXCOORD6;
#ifdef	USE_SOFT_WATER
#ifdef	NEED_SOFT_WATER
	    vector4 tctexgen: TEXCOORD7;
#endif	//	USE_SOFT_WATER
#endif	//	NEED_SOFT_WATER	
    	vector3 position: TEXCOORD8;
        vector4 c0: COLOR0;
};

vf main (v_vert v)
{
    vf o;

    vector4 P = watermove (v.P);
    vector3 NN = unpack_normal (v.N);

    o.v2point = P - eye_position;
    o.tbase = unpack_tc_base (v.uv, v.T.w, v.B.w);
    o.tnorm0 = watermove_tc (o.tbase * W_DISTORT_BASE_TILE_0, P.xz, W_DISTORT_AMP_0);
    o.tnorm1 = watermove_tc (o.tbase * W_DISTORT_BASE_TILE_1, P.xz, W_DISTORT_AMP_1);


        // Calculate the 3x3 transform from tangent space to eye-space
        // TangentToEyeSpace = object2eye * tangent2object
        //                     = object2eye * transpose(object2tangent) (since the inverse of a rotation is its transpose)
        vector3 N = unpack_normal (v.N); // just scale (assume normal in the -.5f, .5f)
        vector3 T = unpack_normal (v.T); //
        vector3 B = unpack_normal (v.B); //
        matrix3x3 xform = mul ((matrix3x3)m_W, matrix3x3 (
                                                T.x, B.x, N.x,
                                                T.y, B.y, N.y,
                                                T.z, B.z, N.z
                                ));
        // The pixel shader operates on the bump-map in [0..1] range
        // Remap this range in the matrix, anyway we are pixel-shader limited :)
        // ...... [ 2  0  0  0]
        // ...... [ 0  2  0  0]
        // ...... [ 0  0  2  0]
        // ...... [-1 -1 -1  1]
        // issue: strange, but it's slower :(
        // issue: interpolators? dp4? VS limited? black magic?

        // Feed this transform to pixel shader
    o.M1 = xform[0];
    o.M2 = xform[1];
    o.M3 = xform[2];

        vector3 L_rgb = v.color.xyz; // precalculated RGB lighting
        vector3 L_hemi = v_hemi (N) * v.N.w; // hemisphere
        vector3 L_sun = v_sun (N) * v.color.w; // sun
        vector3 L_final = L_rgb + L_hemi + L_sun + L_ambient;
                // L_final        = v.N.w        + L_ambient;

    o.hpos = mul (m_VP, P); // xform, input in world coords
    o.position = mul (m_WV, P);

    o.c0 = vector4 (L_final, 0.0h);

//	Igor: for additional depth dest
#ifdef	USE_SOFT_WATER
#ifdef	NEED_SOFT_WATER
	o.tctexgen = mul( m_texgen, P);
	vector3	Pe	= mul		(m_V,  P);
	o.tctexgen.z = Pe.z;
#endif	//	USE_SOFT_WATER
#endif	//	NEED_SOFT_WATER
    return o;
}
