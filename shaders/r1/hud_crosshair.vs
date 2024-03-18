// #include "common_iostructs.h"
#include "common.h"

// uniform float4		screen_res;		// Screen resolution (x-Width,y-Height, zw - 1/resolution)

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
v2p_TL0uv main ( v_TL0uv_positiont I )
{
	v2p_TL0uv O;

	{
		I.P.xy += 0.5f;
//		O.hpos.x = I.P.x/1024 * 2 - 1;
//		O.hpos.y = (I.P.y/768 * 2 - 1)*-1;
		O.hpos.x = I.P.x * screen_res.z * 2 - 1;
		O.hpos.y = (I.P.y * screen_res.w * 2 - 1)*-1;
		O.hpos.zw = I.P.zw;
	}

	// O.Color = bgra_to_rgba(I.Color);	//	swizzle vertex colour
	O.Color = I.Color;	

 	return O;
}