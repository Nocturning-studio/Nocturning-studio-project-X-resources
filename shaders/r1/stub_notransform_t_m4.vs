#include "common_iostructs.h"

uniform float4		screen_res;		// Screen resolution (x-Width,y-Height, zw - 1/resolution)

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
v2p_TL main ( v_TL_positiont I )
{
	v2p_TL O;

//	O.hpos = P;

	{
		I.P.xy += 0.5f;
//		O.hpos.x = I.P.x/1024 * 2 - 1;
//		O.hpos.y = (I.P.y/768 * 2 - 1)*-1;
		O.hpos.x = I.P.x * screen_res.z * 2 - 1;
		O.hpos.y = (I.P.y * screen_res.w * 2 - 1)*-1;
		O.hpos.zw = I.P.zw;
	}

	O.Tex0 = I.Tex0;
	O.Color = float4(I.Color.rgb*4, 1.0f);	//	swizzle vertex colour

 	return O;
}