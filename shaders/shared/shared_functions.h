////////////////////////////////////////////////////////////////////////////
//	Created		: 08.03.2024
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
float4 bgra_to_rgba(float4 c)
{
	return c.bgra;
}

float3 bgra_to_rgba(float3 c)
{
	return c.bgr;
}

float3 unpack_normal(float3 v) 
{ 
	return 2.0f * v - 1.0f;
}

float3 pack_bx2(float3 v) 
{ 
	return v * 0.5f + 0.5f;
}

float3 unpack_bx2(float3 v) 
{ 
	return 2.0f * v - 1.0f;
}

float3 unpack_bx4(float3 v) 
{ 
	return 4.0f * v - 2.0f;
}

float2 unpack_tc_base(float2 tc, float du, float dv) 
{
	return (tc.xy + float2(du, dv)) * (32.0f / 32768.0f);
}

float2 unpack_tc_lmap(float2 tc) 
{ 
	return tc * (1.0f / 32768.0f); // [-1  .. +1 ]
} 

float calc_cyclic(float x)
{
    float phase = 1.0f / (2.0f * 3.141592653589f);
    float sqrt2 = 1.4142136f;
    float sqrt2m2 = 2.8284271f;
    float f = sqrt2m2 * frac(x) - sqrt2; // [-sqrt2 .. +sqrt2]
    return f * f - 1.0f;                   // [-1     .. +1]
}

float2 calc_xz_wave(float2 dir2D, float frac)
{
    // Beizer
    float2 ctrl_A = float2(0.0f, 0.0f);
    float2 ctrl_B = float2(dir2D.x, dir2D.y);
    return lerp(ctrl_A, ctrl_B, frac);
}

float4 watermove(float4 P)
{
    float3 wave1 = float3(0.11f, 0.13f, 0.07f) * W_POSITION_SHIFT_SPEED;
    float dh = sinapprox(timers.x + dot((float3)P, wave1));
    P.y += dh * W_POSITION_SHIFT_HEIGHT;
    return P;
}

float2 watermove_tc(float2 base, float2 P, float amp)
{
    float2 wave1 = float2(0.2111f, 0.2333f) * amp;
    float angle = timers.z + dot(P, wave1);
    float du = sinapprox(angle);
    float dv = cosapprox(angle);
    return (base + amp * float2(du, dv));
}

float3 waterrefl(out float amount, float3 P, float3 N)
{
    float3 v2point = normalize(P - eye_position);
    float3 vreflect = reflect(v2point, N);
    float fresnel = (0.5f + 0.5f * dot(vreflect, v2point));
    amount = 1.0f - fresnel * fresnel; // 0=full env, 1=no env
    return vreflect;
}

float4 wmark_shift(float3 pos, float3 norm)
{
    float3 P = pos;
    float3 N = norm;
    float3 sd = eye_position - P;
    float d = length(sd);
    float w = min(d / RANGE, 1.0f);
    float s = lerp(MIN_SHIFT, MAX_SHIFT, d);
    P += N.xyz * NORMAL_SHIFT;
    P -= normalize(eye_direction + normalize(P - eye_position)) * s;
    return float4(P, 1.0f);
}

float2 calc_detail(float3 w_pos)
{
    float dtl = distance(w_pos, eye_position) * dt_params.w;
    dtl = min(dtl * dtl, 1.0f);
    float dt_mul = 1.0f - dtl;  // dt*  [1 ..  0 ]
    float dt_add = 0.5f * dtl; // dt+	[0 .. 0.5]
    return float2(dt_mul, dt_add);
}

float3 calc_reflection(float3 pos_w, float3 norm_w)
{
    return reflect(normalize(pos_w - eye_position), norm_w);
}
////////////////////////////////////////////////////////////////////////////

