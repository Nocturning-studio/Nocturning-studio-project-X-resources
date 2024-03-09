////////////////////////////////////////////////////////////////////////////
//	Created		: 08.03.2024
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
float3 	unpack_normal(float3 v) { return 2 * v - 1; }
float3 	unpack_bx2(float3 v) { return 2 * v - 1; }
float3 	unpack_bx4(float3 v) { return 4 * v - 2; }

float2 	unpack_tc_base(float2 tc, float du, float dv) {
	return (tc.xy + float2(du, dv)) * (32.f / 32768.f);
}

float2 	unpack_tc_lmap(float2 tc) { return tc * (1.f / 32768.f); } // [-1  .. +1 ]
////////////////////////////////////////////////////////////////////////////

