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
static const vector2 diagonal_coords[32] = {
    vector2(1, 1),  vector2(1, -1),  vector2(-1, 1), vector2(-1, -1), vector2(2, 2),  vector2(2, -2),
    vector2(-2, 2), vector2(-2, -2), vector2(3, 3),  vector2(3, -3),  vector2(-3, 3), vector2(-3, -3),
    vector2(4, 4),  vector2(4, -4),  vector2(-4, 4), vector2(-4, -4), vector2(5, 5),  vector2(5, -5),
    vector2(-5, 5), vector2(-5, -5), vector2(6, 6),  vector2(6, -6),  vector2(-6, 6), vector2(-6, -6),
    vector2(7, 7),  vector2(7, -7),  vector2(-7, 7), vector2(-7, -7), vector2(8, 8),  vector2(8, -8),
    vector2(-8, 8), vector2(-8, -8)};

// Optional diagonal filter (Numsamples - 4, 8, 12, 16, 24, 32)
vector4 diagonal_filter(sampler2D in_sampler, int numsamples, vector2 texcoords)
{
    vector4 output = tex2Dlod0(in_sampler, texcoords);

    for (int i = 0; i < numsamples; i++)
    {
        vector4 sample = tex2Dlod0(in_sampler, texcoords + screen_res.zw * diagonal_coords[i]);
        output += sample;
    }
    return output / (numsamples + 1.0h);
}

static const vector2 box_coords[101] = {
    vector2(-1, 1),  vector2(0, 1),   vector2(1, 1),   vector2(-1, 0),  vector2(0, 0),   vector2(1, 0),
    vector2(-1, -1), vector2(0, -1),  vector2(1, -1), //<-9 (3x3)

    vector2(-2, 2),  vector2(-1, 2),  vector2(0, 2),   vector2(1, 2),   vector2(2, 2),   vector2(-2, 1),
    vector2(2, 1),   vector2(-2, 0),  vector2(2, 0),   vector2(-2, -1), vector2(2, -1),  vector2(-2, -2),
    vector2(-1, -2), vector2(0, -2),  vector2(1, -2),  vector2(2, -2), //<-25 (5x5)

    vector2(-3, 3),  vector2(-2, 3),  vector2(-1, 3),  vector2(0, 3),   vector2(1, 3),   vector2(2, 3),
    vector2(3, 3),   vector2(-3, 1),  vector2(3, 1),   vector2(-3, 2),  vector2(3, 2),   vector2(-3, 0),
    vector2(3, 0),   vector2(-3, -2), vector2(3, -2),  vector2(-3, -1), vector2(3, -1),  vector2(-3, -3),
    vector2(-2, -3), vector2(-1, -3), vector2(0, -3),  vector2(1, -3),  vector2(2, -3),  vector2(3, -3), //<-49 (7x7)

    vector2(4, -4),  vector2(4, -3),  vector2(4, -2),  vector2(4, -1),  vector2(4, 0),   vector2(4, 1),
    vector2(4, 2),   vector2(4, 3),   vector2(4, 4),   vector2(4, 3),   vector2(-4, 3),  vector2(4, 2),
    vector2(-4, 2),  vector2(4, 1),   vector2(-4, 1),  vector2(4, 0),   vector2(-4, 0),  vector2(4, -1),
    vector2(-4, -1), vector2(4, -2),  vector2(-4, -2), vector2(4, -3),  vector2(-4, -3), //<-73 (9x9)

    vector2(-5, 5),  vector2(-4, 5),  vector2(-3, 5),  vector2(-2, 5),  vector2(-1, 5),  vector2(0, 5),
    vector2(1, 5),   vector2(2, 5),   vector2(3, 5),   vector2(4, 5),   vector2(5, 5),   vector2(5, 4),
    vector2(-5, 4),  vector2(5, 3),   vector2(-5, 3),  vector2(5, 2),   vector2(-5, 2),  vector2(5, 1),
    vector2(-5, 1),  vector2(5, 0),   vector2(-5, 0),  vector2(5, -1),  vector2(-5, -1), vector2(5, -2),
    vector2(-5, -2), vector2(5, -3),  vector2(-5, -3), vector2(5, -4),  vector2(-5, -4) //<-101 (11x11)
};

// Optional box filter
vector4 box_filter(sampler2D in_sampler, int numsamples, vector2 texcoords)
{
    vector4 output = NULL;

    for (int i = 0; i < numsamples; i++)
    {
        vector4 sample = tex2Dlod0(in_sampler, texcoords + screen_res.zw * box_coords[i]);
        output += sample;
    }
    return output / numsamples;
}

vector4 box_filter_ao(sampler2D in_sampler, int numsamples, vector2 texcoords)
{
    vector4 output = NULL;
    vector2 PixelSize = ao_resolution.zw;

    for (int i = 0; i < numsamples; i++)
    {
        vector4 sample = tex2Dlod0(in_sampler, texcoords + PixelSize * box_coords[i]);
        output += sample;
    }
    return output / numsamples;
}

// 2x1 filter
vector4 linear_filter(sampler2D in_sampler, vector2 texcoords)
{

    vector2 pixel = screen_res.zw;

    vector4 s0 = tex2Dlod0(in_sampler, texcoords + pixel * vector2(1.5f, 0.5f));  //+1,  0
    vector4 s1 = tex2Dlod0(in_sampler, texcoords + pixel * vector2(-0.5f, 0.5f)); //-1,  0

    return (s0 + s1) / 2.0h;
}

// 2x2 filter
vector4 bilinear_filter(sampler2D in_sampler, vector2 texcoords)
{
    vector2 PixelSize = screen_res.zw;

    vector4 s0 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(1.5f, -0.5f));  //+1, -1
    vector4 s1 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(1.5f, 1.5f));   //+1, +1
    vector4 s2 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(-0.5f, -0.5f)); //-1, -1
    vector4 s3 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(-0.5f, 1.5f));  //-1, +1

    return (s0 + s1 + s2 + s3) / 4.0h;
}

vector4 bilinear_filter_ao(sampler2D in_sampler, vector2 texcoords)
{
    vector2 PixelSize = ao_resolution.zw;

    vector4 s0 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(1.5f, -0.5f));  //+1, -1
    vector4 s1 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(1.5f, 1.5f));   //+1, +1
    vector4 s2 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(-0.5f, -0.5f)); //-1, -1
    vector4 s3 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(-0.5f, 1.5f));  //-1, +1

    return (s0 + s1 + s2 + s3) / 4.0h;
}

// 3x3 filter
vector4 trilinear_filter(sampler2D in_sampler, vector2 texcoords)
{
    vector2 PixelSize = screen_res.zw;

    vector4 s0 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(2.5f, -1.5f));  //+2, -2
    vector4 s1 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(2.5f, 2.5f));   //+2, +2
    vector4 s2 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(-1.5f, -1.5f)); //-2, -2
    vector4 s3 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(-1.5f, 2.5f));  //-2, +2

    vector4 s4 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(0.5f, -2.5f)); // 0, -2
    vector4 s5 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(0.5f, 2.5f));  // 0, +2
    vector4 s6 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(-2.5f, 0.5f)); //-2,  0
    vector4 s7 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(2.5f, 0.5f));  //+2,  0

    return (s0 + s1 + s2 + s3 + s4 + s5 + s6 + s7) / 8.0h;
}

vector4 trilinear_filter_ao(sampler2D in_sampler, vector2 texcoords)
{
    vector2 PixelSize = ao_resolution.zw;

    vector4 s0 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(2.5f, -1.5f));  //+2, -2
    vector4 s1 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(2.5f, 2.5f));   //+2, +2
    vector4 s2 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(-1.5f, -1.5f)); //-2, -2
    vector4 s3 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(-1.5f, 2.5f));  //-2, +2

    vector4 s4 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(0.5f, -2.5f)); // 0, -2
    vector4 s5 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(0.5f, 2.5f));  // 0, +2
    vector4 s6 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(-2.5f, 0.5f)); //-2,  0
    vector4 s7 = tex2Dlod0(in_sampler, texcoords + PixelSize * vector2(2.5f, 0.5f));  //+2,  0

    return (s0 + s1 + s2 + s3 + s4 + s5 + s6 + s7) / 8.0h;
}

vector depth_based_diagonal_filter_4x4(sampler2D in_sampler, vector2 centerTC)
{
    // low weight center sample - will be used on edges
    vector fSumWeight = 0.025f;
    vector2 centerData = tex2D(in_sampler, centerTC);

    vector fOcclusion = centerData.r * fSumWeight;
    vector centerDepth = tex2D(s_gbuffer_position, centerTC).z;

    vector2 arrOffsets[4] = {vector2(1, -1), vector2(-1, -1), vector2(1, 1), vector2(-1, 1)};

    for (int i = 0; i < 4; i++)
    {
        vector2 sampleTC = centerTC + screen_res.zw * arrOffsets[i];
        vector2 sampleData = tex2Dlod0(in_sampler, sampleTC);

        vector fDepth = tex2Dlod0(s_gbuffer_position, sampleTC).z;
        vector fDiff = 8 * abs(fDepth - centerDepth) / min(fDepth, centerDepth);
        vector fWeight = saturate(1 - fDiff);

        fOcclusion += sampleData.r * fWeight;

        fSumWeight += fWeight;
    }

    fOcclusion /= fSumWeight;

    return fOcclusion;
}

// Depth based diagonal filter from X-Ray Engine 2.0
vector depth_based_diagonal_ao_filter(sampler2D in_sampler, vector2 uv)
{
    vector center_depth = tex2D(s_gbuffer_position, uv).z;
    vector ao_sum_xy = 0.0h;
    vector ao_sum_yx = 0.0h;
    vector total_weight_xy = 0.0h;
    vector total_weight_yx = 0.0h;

    int x;
    for (x = -4; x <= 4; x++)
    {
        int y = x;
        vector2 sample_uv = uv + vector2(x, y) / screen_res;
        vector sample_ao = tex2D(in_sampler, sample_uv).x;
        vector sample_depth = tex2D(s_gbuffer_position, sample_uv).z;

        vector depth_result = 1.0h - saturate(pow((sample_depth - center_depth), 2.0h) / (0.001f * center_depth)) + 0.2f;

        total_weight_xy += depth_result;

        ao_sum_xy += sample_ao * depth_result;
    }

    for (x = -4; x <= 4; x++)
    {
        int y = -x;
        vector2 sample_uv = uv + vector2(x, y) / screen_res;
        vector sample_ao = tex2D(in_sampler, sample_uv).x;
        vector sample_depth = tex2D(s_gbuffer_position, sample_uv).z;

        vector depth_result = 1.0h - saturate(pow((sample_depth - center_depth), 2.0h) / (0.001f * center_depth)) + 0.2f;

        total_weight_yx += depth_result;

        ao_sum_yx += sample_ao * depth_result;
    }
    return min(ao_sum_xy / total_weight_xy, ao_sum_yx / total_weight_yx);
}

static const vector2 strided_coords[35] = {
    vector2(-1, 1), vector2(-1, 0),  vector2(1, -1),

    vector2(-2, 2), vector2(1, 2),   vector2(2, 1),  vector2(-2, -1), vector2(-1, -2), vector2(2, -2),

    vector2(-3, 3), vector2(0, 3),   vector2(3, 3),  vector2(-3, 2),  vector2(3, 0),   vector2(-3, -1), vector2(0, -3),
    vector2(3, -3),

    vector2(4, -4), vector2(4, -1),  vector2(4, 2),  vector2(4, 3),   vector2(-4, 2),  vector2(4, 0),   vector2(-4, -3),

    vector2(-5, 5), vector2(-2, 5),  vector2(1, 5),  vector2(4, 5),   vector2(5, 5),   vector2(5, 3),   vector2(-5, 2),
    vector2(5, 0),  vector2(-5, -1), vector2(5, -3), vector2(-5, -4)};

vector4 strided_filter(sampler2D in_sampler, int numsamples, vector2 texcoords)
{
    vector4 output = NULL;

    for (int i = 0; i < numsamples; i++)
    {
        vector4 sample = tex2Dlod0(in_sampler, texcoords + screen_res.zw * strided_coords[i]);
        output += sample;
    }
    return output / numsamples;
}

vector4 strided_filter_ao(sampler2D in_sampler, int numsamples, vector2 texcoords)
{
    vector4 output = NULL;
    vector2 PixelSize = ao_resolution.zw;

    for (int i = 0; i < numsamples; i++)
    {
        vector4 sample = tex2Dlod0(in_sampler, texcoords + PixelSize * strided_coords[i]);
        output += sample;
    }
    return output / numsamples;
}
#endif
