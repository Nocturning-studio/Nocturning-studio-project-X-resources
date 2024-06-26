////////////////////////////////////////////////////////////////////////////
//	Created		: 27.07.2022
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
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
    return mul(m_P, float4(P, 1.0f));
}
//////////////////////////////////////////////////////////////////////////////////////////
float2 projection_to_screen(float4 proj)
{
    float2 screen = proj.xy / proj.w;
    return (screen + float2(1.0f, -1.0f)) * float2(0.5f, -0.5f);
}
////////////////////////////////////////////////////////////////////////////
//TODO Deathman to all - replace with more faster noise
float random(float2 st)
{
    return frac(sin(dot(st.xy, float2(12.9898, 78.233))) * 43758.5453123);
}
////////////////////////////////////////////////////////////////////////////