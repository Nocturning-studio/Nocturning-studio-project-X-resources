////////////////////////////////////////////////////////////////////////////
//	Created		: 27.07.2022
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
#ifndef COMMON_FUNCTIONS_H_INCLUDED
#define COMMON_FUNCTIONS_H_INCLUDED
////////////////////////////////////////////////////////////////////////////
#include "common_math.h"
#include "common_samplers.h"
#include "common_uniforms.h"
////////////////////////////////////////////////////////////////////////////
// Shared common
vector calc_cyclic(vector x)
{
    vector phase = 1 / (2 * 3.141592653589f);
    vector sqrt2 = 1.4142136f;
    vector sqrt2m2 = 2.8284271f;
    vector f = sqrt2m2 * frac(x) - sqrt2; // [-sqrt2 .. +sqrt2]
    return f * f - 1.f;                   // [-1     .. +1]
}

vector2 calc_xz_wave(vector2 dir2D, vector frac)
{
    // Beizer
    vector2 ctrl_A = vector2(0.f, 0.f);
    vector2 ctrl_B = vector2(dir2D.x, dir2D.y);
    return lerp(ctrl_A, ctrl_B, frac);
}

vector4 watermove(vector4 P)
{
    vector3 wave1 = vector3(0.11f, 0.13f, 0.07f) * W_POSITION_SHIFT_SPEED;
    vector dh = sinapprox(timers.x + dot((vector3)P, wave1));
    P.y += dh * W_POSITION_SHIFT_HEIGHT;
    return P;
}

vector2 watermove_tc(vector2 base, vector2 P, vector amp)
{
    vector2 wave1 = vector2(0.2111f, 0.2333f) * amp;
    vector angle = timers.z + dot(P, wave1);
    vector du = sinapprox(angle);
    vector dv = cosapprox(angle);
    return (base + amp * vector2(du, dv));
}

vector3 waterrefl(out vector amount, vector3 P, vector3 N)
{
    vector3 v2point = normalize(P - eye_position);
    vector3 vreflect = reflect(v2point, N);
    vector fresnel = (.5f + .5f * dot(vreflect, v2point));
    amount = 1 - fresnel * fresnel; // 0=full env, 1=no env
    return vreflect;
}

vector4 wmark_shift(vector3 pos, vector3 norm)
{
    vector3 P = pos;
    vector3 N = norm;
    vector3 sd = eye_position - P;
    vector d = length(sd);
    vector w = min(d / RANGE, 1.f);
    vector s = lerp(MIN_SHIFT, MAX_SHIFT, d);
    P += N.xyz * NORMAL_SHIFT;
    P -= normalize(eye_direction + normalize(P - eye_position)) * s;
    return vector4(P, 1.f);
}
///////////////////////////////////////////////////////////////////////////////////////////
vector calc_fogging(vector4 w_pos)
{
    return dot(w_pos, fog_plane);
}

vector2 calc_detail(vector3 w_pos)
{
    vector dtl = distance(w_pos, eye_position) * dt_params.w;
    dtl = min(dtl * dtl, 1);
    vector dt_mul = 1 - dtl;  // dt*  [1 ..  0 ]
    vector dt_add = .5 * dtl; // dt+	[0 .. 0.5]
    return vector2(dt_mul, dt_add);
}

vector3 calc_reflection(vector3 pos_w, vector3 norm_w)
{
    return reflect(normalize(pos_w - eye_position), norm_w);
}

vector3 calc_sun_r1(vector3 norm_w)
{
    return L_sun_color * saturate(dot((norm_w), -L_sun_dir_w));
}

vector3 calc_model_hemi_r1(vector3 norm_w)
{
    return max(0, norm_w.y) * L_hemi_color;
}

vector3 calc_model_lq_lighting(vector3 norm_w)
{
    return L_material.x * calc_model_hemi_r1(norm_w) + L_ambient + L_material.y * calc_sun_r1(norm_w);
}
//////////////////////////////////////////////////////////////////////////////////////////

vector3 ACES(const vector3 x) 
{
    const float a = 2.51f;
    const float b = 0.03f;
    const float c = 2.43f;
    const float d = 0.59f;
    const float e = 0.14f;
    return (x * (a * x + b)) / (x * (c * x + d) + e);
}

vector3 v_hemi(vector3 n)
{
    return L_hemi_color * (.5f + .5f * n.y);
}

vector3 v_hemi_wrap(vector3 n, vector w)
{
    return L_hemi_color * (w + (1 - w) * n.y);
}

vector3 v_sun(vector3 n)
{
    return L_sun_color * dot(n, -L_sun_dir_w);
}

vector3 v_sun_wrap(vector3 n, vector w)
{
    return L_sun_color * (w + (1 - w) * dot(n, -L_sun_dir_w));
}

vector3 p_hemi(vector2 tc)
{
    vector3 t_lmh = tex2D(s_hemi, tc);
    return dot(t_lmh, 1.h / 3.h);
}

//	contrast function
vector Contrast(vector Input, vector ContrastPower)
{
    // piecewise contrast function
    bool IsAbovefloat = Input > 0.5;
    vector ToRaise = saturate(2 * (IsAbovefloat ? 1 - Input : Input));
    vector Output = 0.5 * pow(ToRaise, ContrastPower);
    Output = IsAbovefloat ? 1 - Output : Output;
    return Output;
}

vector4 proj_to_screen(vector4 proj)
{
    vector4 screen = proj;
    screen.x = (proj.x + proj.w);
    screen.y = (proj.w - proj.y);
    screen.xy *= 0.5;
    return screen;
}

vector3 uv_to_eye(vector2 uv, vector eye_z)
{
    uv = (uv * vector2(2.0, 2.0) - vector2(1.0, 1.0));
    // return vector3(uv * pos_decompression_params.xy * eye_z, eye_z);
    return vector3(uv * eye_z, eye_z);
}

vector get_hemi(vector4 lmh)
{
    return lmh.g;
}

vector get_sun(vector4 lmh)
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

vector3 unpack_normal(vector3 v)
{
    return 2 * v - 1;
}
vector3 unpack_bx2(vector3 v)
{
    return 2 * v - 1;
}
vector3 unpack_bx4(vector3 v)
{
    return 4 * v - 2;
}
vector2 unpack_tc_lmap(vector2 tc)
{
    return tc * (1.f / 32768.f);
} // [-1  .. +1 ]
vector2 unpack_tc_base(vector2 tc, vector du, vector dv)
{
    return (tc.xy + vector2(du, dv)) * (32.f / 32768.f);
}
////////////////////////////////////////////////////////////////////////////
#endif // #ifndef COMMON_FUNCTIONS_H_INCLUDED
////////////////////////////////////////////////////////////////////////////
