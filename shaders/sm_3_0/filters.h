////////////////////////////////////////////////////////////////////////////
//	Created		: 18.11.2022
//	Author		: Deathman
//	NSProjectX - 2022
///////////////////////////////////////////////////////////////////////////
#ifndef SHADERS_R2_FILTERS_H_INCLUDED
#define SHADERS_R2_FILTERS_H_INCLUDED
///////////////////////////////////////////////////////////////////////////
#include "common.h"
///////////////////////////////////////////////////////////////////////////
static const float2 diagonal_coords[32] = {
    float2(1, 1),  float2(1, -1),  float2(-1, 1), float2(-1, -1), float2(2, 2),  float2(2, -2),
    float2(-2, 2), float2(-2, -2), float2(3, 3),  float2(3, -3),  float2(-3, 3), float2(-3, -3),
    float2(4, 4),  float2(4, -4),  float2(-4, 4), float2(-4, -4), float2(5, 5),  float2(5, -5),
    float2(-5, 5), float2(-5, -5), float2(6, 6),  float2(6, -6),  float2(-6, 6), float2(-6, -6),
    float2(7, 7),  float2(7, -7),  float2(-7, 7), float2(-7, -7), float2(8, 8),  float2(8, -8),
    float2(-8, 8), float2(-8, -8)};

// Optional diagonal filter (Numsamples - 4, 8, 12, 16, 24, 32)
float4 diagonal_filter(sampler2D in_sampler, int numsamples, float2 texcoords)
{
    float4 output = tex2Dlod0(in_sampler, texcoords);

    for (int i = 0; i < numsamples; i++)
    {
        float4 sample = tex2Dlod0(in_sampler, texcoords + screen_res.zw * diagonal_coords[i]);
        output += sample;
    }
    return output / (numsamples + 1.0h);
}

static const float2 box_coords[101] = {
    float2(-1, 1),  float2(0, 1),   float2(1, 1),   float2(-1, 0),  float2(0, 0),   float2(1, 0),
    float2(-1, -1), float2(0, -1),  float2(1, -1), //<-9 (3x3)

    float2(-2, 2),  float2(-1, 2),  float2(0, 2),   float2(1, 2),   float2(2, 2),   float2(-2, 1),
    float2(2, 1),   float2(-2, 0),  float2(2, 0),   float2(-2, -1), float2(2, -1),  float2(-2, -2),
    float2(-1, -2), float2(0, -2),  float2(1, -2),  float2(2, -2), //<-25 (5x5)

    float2(-3, 3),  float2(-2, 3),  float2(-1, 3),  float2(0, 3),   float2(1, 3),   float2(2, 3),
    float2(3, 3),   float2(-3, 1),  float2(3, 1),   float2(-3, 2),  float2(3, 2),   float2(-3, 0),
    float2(3, 0),   float2(-3, -2), float2(3, -2),  float2(-3, -1), float2(3, -1),  float2(-3, -3),
    float2(-2, -3), float2(-1, -3), float2(0, -3),  float2(1, -3),  float2(2, -3),  float2(3, -3), //<-49 (7x7)

    float2(4, -4),  float2(4, -3),  float2(4, -2),  float2(4, -1),  float2(4, 0),   float2(4, 1),
    float2(4, 2),   float2(4, 3),   float2(4, 4),   float2(4, 3),   float2(-4, 3),  float2(4, 2),
    float2(-4, 2),  float2(4, 1),   float2(-4, 1),  float2(4, 0),   float2(-4, 0),  float2(4, -1),
    float2(-4, -1), float2(4, -2),  float2(-4, -2), float2(4, -3),  float2(-4, -3), //<-73 (9x9)

    float2(-5, 5),  float2(-4, 5),  float2(-3, 5),  float2(-2, 5),  float2(-1, 5),  float2(0, 5),
    float2(1, 5),   float2(2, 5),   float2(3, 5),   float2(4, 5),   float2(5, 5),   float2(5, 4),
    float2(-5, 4),  float2(5, 3),   float2(-5, 3),  float2(5, 2),   float2(-5, 2),  float2(5, 1),
    float2(-5, 1),  float2(5, 0),   float2(-5, 0),  float2(5, -1),  float2(-5, -1), float2(5, -2),
    float2(-5, -2), float2(5, -3),  float2(-5, -3), float2(5, -4),  float2(-5, -4) //<-101 (11x11)
};

// Optional box filter
float4 box_filter(sampler2D in_sampler, int numsamples, float2 texcoords)
{
    float4 output = NULL;

    for (int i = 0; i < numsamples; i++)
    {
        float4 sample = tex2Dlod0(in_sampler, texcoords + screen_res.zw * box_coords[i]);
        output += sample;
    }
    return output / numsamples;
}

float4 box_filter_ao(sampler2D in_sampler, int numsamples, float2 texcoords)
{
    float4 output = NULL;
    float2 PixelSize = ao_resolution.zw;

    for (int i = 0; i < numsamples; i++)
    {
        float4 sample = tex2Dlod0(in_sampler, texcoords + PixelSize * box_coords[i]);
        output += sample;
    }
    return output / numsamples;
}

// 2x1 filter
float4 linear_filter(sampler2D in_sampler, float2 texcoords)
{

    float2 pixel = screen_res.zw;

    float4 s0 = tex2Dlod0(in_sampler, texcoords + pixel * float2(1.5f, 0.5f));  //+1,  0
    float4 s1 = tex2Dlod0(in_sampler, texcoords + pixel * float2(-0.5f, 0.5f)); //-1,  0

    return (s0 + s1) / 2.0h;
}

// 2x2 filter
float4 bilinear_filter(sampler2D in_sampler, float2 texcoords)
{
    float2 PixelSize = screen_res.zw;

    float4 s0 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(1.5f, -0.5f));  //+1, -1
    float4 s1 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(1.5f, 1.5f));   //+1, +1
    float4 s2 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(-0.5f, -0.5f)); //-1, -1
    float4 s3 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(-0.5f, 1.5f));  //-1, +1

    return (s0 + s1 + s2 + s3) / 4.0h;
}

float4 bilinear_filter_ao(sampler2D in_sampler, float2 texcoords)
{
    float2 PixelSize = ao_resolution.zw;

    float4 s0 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(1.5f, -0.5f));  //+1, -1
    float4 s1 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(1.5f, 1.5f));   //+1, +1
    float4 s2 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(-0.5f, -0.5f)); //-1, -1
    float4 s3 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(-0.5f, 1.5f));  //-1, +1

    return (s0 + s1 + s2 + s3) / 4.0h;
}

// 3x3 filter
float4 trilinear_filter(sampler2D in_sampler, float2 texcoords)
{
    float2 PixelSize = screen_res.zw;

    float4 s0 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(2.5f, -1.5f));  //+2, -2
    float4 s1 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(2.5f, 2.5f));   //+2, +2
    float4 s2 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(-1.5f, -1.5f)); //-2, -2
    float4 s3 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(-1.5f, 2.5f));  //-2, +2

    float4 s4 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(0.5f, -2.5f)); // 0, -2
    float4 s5 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(0.5f, 2.5f));  // 0, +2
    float4 s6 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(-2.5f, 0.5f)); //-2,  0
    float4 s7 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(2.5f, 0.5f));  //+2,  0

    return (s0 + s1 + s2 + s3 + s4 + s5 + s6 + s7) / 8.0h;
}

float4 trilinear_filter_ao(sampler2D in_sampler, float2 texcoords)
{
    float2 PixelSize = ao_resolution.zw;

    float4 s0 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(2.5f, -1.5f));  //+2, -2
    float4 s1 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(2.5f, 2.5f));   //+2, +2
    float4 s2 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(-1.5f, -1.5f)); //-2, -2
    float4 s3 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(-1.5f, 2.5f));  //-2, +2

    float4 s4 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(0.5f, -2.5f)); // 0, -2
    float4 s5 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(0.5f, 2.5f));  // 0, +2
    float4 s6 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(-2.5f, 0.5f)); //-2,  0
    float4 s7 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(2.5f, 0.5f));  //+2,  0

    return (s0 + s1 + s2 + s3 + s4 + s5 + s6 + s7) / 8.0h;
}

float depth_based_diagonal_filter_4x4(sampler2D in_sampler, float2 centerTC)
{
    // low weight center sample - will be used on edges
    float fSumWeight = 0.025f;
    float2 centerData = tex2D(in_sampler, centerTC);

    float fOcclusion = centerData.r * fSumWeight;
    float centerDepth = tex2D(s_gbuffer_position, centerTC).z;

    float2 arrOffsets[4] = {float2(1, -1), float2(-1, -1), float2(1, 1), float2(-1, 1)};

    for (int i = 0; i < 4; i++)
    {
        float2 sampleTC = centerTC + screen_res.zw * arrOffsets[i];
        float2 sampleData = tex2Dlod0(in_sampler, sampleTC);

        float fDepth = tex2Dlod0(s_gbuffer_position, sampleTC).z;
        float fDiff = 8 * abs(fDepth - centerDepth) / min(fDepth, centerDepth);
        float fWeight = saturate(1 - fDiff);

        fOcclusion += sampleData.r * fWeight;

        fSumWeight += fWeight;
    }

    fOcclusion /= fSumWeight;

    return fOcclusion;
}

// Depth based diagonal filter from X-Ray Engine 2.0
float depth_based_diagonal_ao_filter(sampler2D in_sampler, float2 uv)
{
    float center_depth = tex2D(s_gbuffer_position, uv).z;
    float ao_sum_xy = 0.0h;
    float ao_sum_yx = 0.0h;
    float total_weight_xy = 0.0h;
    float total_weight_yx = 0.0h;

    int x;
    for (x = -4; x <= 4; x++)
    {
        int y = x;
        float2 sample_uv = uv + float2(x, y) / screen_res;
        float sample_ao = tex2D(in_sampler, sample_uv).x;
        float sample_depth = tex2D(s_gbuffer_position, sample_uv).z;

        float depth_result = 1.0h - saturate(pow((sample_depth - center_depth), 2.0h) / (0.001f * center_depth)) + 0.2f;

        total_weight_xy += depth_result;

        ao_sum_xy += sample_ao * depth_result;
    }

    for (x = -4; x <= 4; x++)
    {
        int y = -x;
        float2 sample_uv = uv + float2(x, y) / screen_res;
        float sample_ao = tex2D(in_sampler, sample_uv).x;
        float sample_depth = tex2D(s_gbuffer_position, sample_uv).z;

        float depth_result = 1.0h - saturate(pow((sample_depth - center_depth), 2.0h) / (0.001f * center_depth)) + 0.2f;

        total_weight_yx += depth_result;

        ao_sum_yx += sample_ao * depth_result;
    }
    return min(ao_sum_xy / total_weight_xy, ao_sum_yx / total_weight_yx);
}

static const float2 strided_coords[35] = {
    float2(-1, 1), float2(-1, 0),  float2(1, -1),

    float2(-2, 2), float2(1, 2),   float2(2, 1),  float2(-2, -1), float2(-1, -2), float2(2, -2),

    float2(-3, 3), float2(0, 3),   float2(3, 3),  float2(-3, 2),  float2(3, 0),   float2(-3, -1), float2(0, -3),
    float2(3, -3),

    float2(4, -4), float2(4, -1),  float2(4, 2),  float2(4, 3),   float2(-4, 2),  float2(4, 0),   float2(-4, -3),

    float2(-5, 5), float2(-2, 5),  float2(1, 5),  float2(4, 5),   float2(5, 5),   float2(5, 3),   float2(-5, 2),
    float2(5, 0),  float2(-5, -1), float2(5, -3), float2(-5, -4)};

float4 strided_filter(sampler2D in_sampler, int numsamples, float2 texcoords)
{
    float4 output = NULL;

    for (int i = 0; i < numsamples; i++)
    {
        float4 sample = tex2Dlod0(in_sampler, texcoords + screen_res.zw * strided_coords[i]);
        output += sample;
    }
    return output / numsamples;
}

float4 strided_filter_ao(sampler2D in_sampler, int numsamples, float2 texcoords)
{
    float4 output = NULL;
    float2 PixelSize = ao_resolution.zw;

    for (int i = 0; i < numsamples; i++)
    {
        float4 sample = tex2Dlod0(in_sampler, texcoords + PixelSize * strided_coords[i]);
        output += sample;
    }
    return output / numsamples;
}
#endif
