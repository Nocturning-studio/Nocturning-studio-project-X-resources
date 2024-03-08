////////////////////////////////////////////////////////////////////////////
//	Created		: 27.07.2022
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
#ifndef COMMON_FUNCTIONS_H_INCLUDED
#define COMMON_FUNCTIONS_H_INCLUDED
////////////////////////////////////////////////////////////////////////////
// Shared common
float calc_cyclic(float x)
{
    float phase = 1 / (2 * 3.141592653589f);
    float sqrt2 = 1.4142136f;
    float sqrt2m2 = 2.8284271f;
    float f = sqrt2m2 * frac(x) - sqrt2; // [-sqrt2 .. +sqrt2]
    return f * f - 1.f;                   // [-1     .. +1]
}

float2 calc_xz_wave(float2 dir2D, float frac)
{
    // Beizer
    float2 ctrl_A = float2(0.f, 0.f);
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
    float fresnel = (.5f + .5f * dot(vreflect, v2point));
    amount = 1 - fresnel * fresnel; // 0=full env, 1=no env
    return vreflect;
}

float4 wmark_shift(float3 pos, float3 norm)
{
    float3 P = pos;
    float3 N = norm;
    float3 sd = eye_position - P;
    float d = length(sd);
    float w = min(d / RANGE, 1.f);
    float s = lerp(MIN_SHIFT, MAX_SHIFT, d);
    P += N.xyz * NORMAL_SHIFT;
    P -= normalize(eye_direction + normalize(P - eye_position)) * s;
    return float4(P, 1.f);
}

float2 calc_detail(float3 w_pos)
{
    float dtl = distance(w_pos, eye_position) * dt_params.w;
    dtl = min(dtl * dtl, 1);
    float dt_mul = 1 - dtl;  // dt*  [1 ..  0 ]
    float dt_add = .5 * dtl; // dt+	[0 .. 0.5]
    return float2(dt_mul, dt_add);
}

float3 calc_reflection(float3 pos_w, float3 norm_w)
{
    return reflect(normalize(pos_w - eye_position), norm_w);
}

float3 calc_sun_r1(float3 norm_w)
{
    return L_sun_color * saturate(dot((norm_w), -L_sun_dir_w));
}

float3 calc_model_hemi_r1(float3 norm_w)
{
    return max(0, norm_w.y) * L_hemi_color;
}

float3 calc_model_lq_lighting(float3 norm_w)
{
    return L_material.x * calc_model_hemi_r1(norm_w) + L_ambient + L_material.y * calc_sun_r1(norm_w);
}
//////////////////////////////////////////////////////////////////////////////////////////

float3 ACES(const float3 x) 
{
    const float a = 2.51f;
    const float b = 0.03f;
    const float c = 2.43f;
    const float d = 0.59f;
    const float e = 0.14f;
    return (x * (a * x + b)) / (x * (c * x + d) + e);
}

float3 v_hemi(float3 n)
{
    return L_hemi_color * (.5f + .5f * n.y);
}

float3 v_hemi_wrap(float3 n, float w)
{
    return L_hemi_color * (w + (1 - w) * n.y);
}

float3 v_sun(float3 n)
{
    return L_sun_color * dot(n, -L_sun_dir_w);
}

float3 v_sun_wrap(float3 n, float w)
{
    return L_sun_color * (w + (1 - w) * dot(n, -L_sun_dir_w));
}

float3 p_hemi(float2 tc)
{
    float3 t_lmh = tex2D(s_hemi, tc);
    return dot(t_lmh, 1.h / 3.h);
}

//	contrast function
float Contrast(float Input, float ContrastPower)
{
    // piecewise contrast function
    bool IsAbovefloat = Input > 0.5;
    float ToRaise = saturate(2 * (IsAbovefloat ? 1 - Input : Input));
    float Output = 0.5 * pow(ToRaise, ContrastPower);
    Output = IsAbovefloat ? 1 - Output : Output;
    return Output;
}

float3 uv_to_eye(float2 uv, float eye_z)
{
    uv = (uv * float2(2.0, 2.0) - float2(1.0, 1.0));
    // return float3(uv * pos_decompression_params.xy * eye_z, eye_z);
    return float3(uv * eye_z, eye_z);
}

float get_hemi(float4 lmh)
{
    return lmh.g;
}

float get_sun(float4 lmh)
{
    return lmh.a;
}

float rand_1_05(in float2 uv)
{
    float2 noise = (frac(sin(dot(uv, float2(12.9898, 78.233) * 2.0)) * 43758.5453));
    return abs(noise.x + noise.y) * 0.5;
}

float2 rand_2_10(in float2 uv)
{
    float noiseX = (frac(sin(dot(uv, float2(12.9898, 78.233) * 2.0)) * 43758.5453));
    float noiseY = sqrt(1 - noiseX * noiseX);
    return float2(noiseX, noiseY);
}

float2 rand_2_0004(in float2 uv)
{
    float noiseX = (frac(sin(dot(uv, float2(12.9898, 78.233))) * 43758.5453));
    float noiseY = (frac(sin(dot(uv, float2(12.9898, 78.233) * 2.0)) * 43758.5453));
    return float2(noiseX, noiseY) * 0.004;
}
//////////////////////////////////////////////////////////////////////////////////////////
float4 calc_proj(float3 P)
{
    return mul(m_P, float4(P, 1.0h));
}
//////////////////////////////////////////////////////////////////////////////////////////
float2 projection_to_screen(float4 proj)
{
    float2 screen = proj.xy / proj.w;
    return (screen + float2(1.0h, -1.0h)) * float2(0.5h, -0.5h);
}
////////////////////////////////////////////////////////////////////////////
#endif // #ifndef COMMON_FUNCTIONS_H_INCLUDED
////////////////////////////////////////////////////////////////////////////
