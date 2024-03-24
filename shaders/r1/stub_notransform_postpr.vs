#include "common_iostructs.h"

uniform float4		screen_res;		// Screen resolution (x-Width,y-Height, zw - 1/resolution)

struct	v_postpr
{
	float4	P		: POSITIONT;
	float2 	Tex0	: TEXCOORD0;	// base1 (duality)	
	float2	Tex1	: TEXCOORD1;	// base2 (duality)
	float2	Tex2	: TEXCOORD2;	// base  (noise)
	float4	Color	: COLOR0;		// multiplier, color.w = noise_amount
	float4	Gray	: COLOR1;		// (.3,.3,.3.,amount)
};
struct	v2p_postpr
{
	float2 	Tex0	: TEXCOORD0;	// base1 (duality)	
	float2	Tex1	: TEXCOORD1;	// base2 (duality)
	float2	Tex2	: TEXCOORD2;	// base  (noise)
	float4	Color	: COLOR0;		// multiplier, color.w = noise_amount
	float4	Gray	: COLOR1;		// (.3,.3,.3.,amount)
	float4 	hpos	: SV_Position;	// Clip-space position 	(for rasterization)
};

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
v2p_postpr main ( v_postpr I )
{
	v2p_postpr	O;

//	O.hpos	= I.P;

	{
		I.P.xy += 0.5f;
//		O.hpos.x = I.P.x/1024 * 2 - 1;
//		O.hpos.y = (I.P.y/768 * 2 - 1)*-1;
		O.hpos.x = I.P.x * screen_res.z * 2 - 1;
		O.hpos.y = (I.P.y * screen_res.w * 2 - 1)*-1;
		O.hpos.zw = I.P.zw;
	}


	O.Tex0	= I.Tex0;
	O.Tex1	= I.Tex1;
	O.Tex2	= I.Tex2;
	
	O.Color = I.Color;	//	swizzle vertex colour
	O.Gray	= I.Gray;	//	swizzle vertex colour

 	return O;
}