////////////////////////////////////////////////////////////////////////////
//	Created		: 18.11.2022
//	Author		: Deathman
//	NSProjectX - 2022
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
float4 diagonal_filter(type_sampler2D(in_sampler), int numsamples, float2 texcoords)
{
    float4 output = tex2Dlod0(in_sampler, texcoords);

    for (int i = 0; i < numsamples; i++)
    {
        float4 sample = tex2Dlod0(in_sampler, texcoords + screen_res.zw * diagonal_coords[i]);
        output += sample;
    }
    return output / (numsamples + 1.0f);
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
float4 box_filter(type_sampler2D(in_sampler), float2 PixelSize, int numsamples, float2 texcoords)
{
    float4 output = NULL;

    for (int i = 0; i < numsamples; i++)
    {
        float4 sample = tex2Dlod0(in_sampler, texcoords + PixelSize * box_coords[i]);
        output += sample;
    }
    return output / numsamples;
}

// 4x4 filter
float4 bilinear_filter(type_sampler2D(in_sampler), float2 PixelSize, float2 texcoords)
{
    float4 s0 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(1.5f, -0.5f));  //+1, -1
    float4 s1 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(1.5f, 1.5f));   //+1, +1
    float4 s2 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(-0.5f, -0.5f)); //-1, -1
    float4 s3 = tex2Dlod0(in_sampler, texcoords + PixelSize * float2(-0.5f, 1.5f));  //-1, +1

    return (s0 + s1 + s2 + s3) / 4.0f;
}

static const float2 strided_coords[35] = {
    float2(-1, 1), float2(-1, 0),  float2(1, -1),

    float2(-2, 2), float2(1, 2),   float2(2, 1),  float2(-2, -1), float2(-1, -2), float2(2, -2),

    float2(-3, 3), float2(0, 3),   float2(3, 3),  float2(-3, 2),  float2(3, 0),   float2(-3, -1), float2(0, -3),
    float2(3, -3),

    float2(4, -4), float2(4, -1),  float2(4, 2),  float2(4, 3),   float2(-4, 2),  float2(4, 0),   float2(-4, -3),

    float2(-5, 5), float2(-2, 5),  float2(1, 5),  float2(4, 5),   float2(5, 5),   float2(5, 3),   float2(-5, 2),
    float2(5, 0),  float2(-5, -1), float2(5, -3), float2(-5, -4)};

float4 strided_filter(type_sampler2D(in_sampler), float2 PixelSize, int numsamples, float2 texcoords)
{
    float4 output = NULL;

    for (int i = 0; i < numsamples; i++)
    {
        float4 sample = tex2Dlod0(in_sampler, texcoords + PixelSize * strided_coords[i]);
        output += sample;
    }
    return output / numsamples;
}
