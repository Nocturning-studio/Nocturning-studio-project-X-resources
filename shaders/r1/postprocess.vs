////////////////////////////////////////////////////////////////////////////
//	Created		: 16.03.2024
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
#include "common.h"
////////////////////////////////////////////////////////////////////////////
struct VertexData
{
    float4 Position: POSITIONT;
    float2 DualityTexCoords0: TEXCOORD0;
    float2 DualityTexCoords1: TEXCOORD1;
    float2 NoiseTexCoords: TEXCOORD2;
    float4 Color: COLOR0;
    float4 Gray: COLOR1;
};

struct Interpolators
{
    float4 HomogeniousPosition: SV_Position;
    float2 DualityTexCoords0: TEXCOORD0;
    float2 DualityTexCoords1: TEXCOORD1;
    float2 NoiseTexCoords: TEXCOORD2;
    float4 Color: COLOR0;
    float4 Gray: COLOR1;
};
////////////////////////////////////////////////////////////////////////////
Interpolators main (VertexData Input)
{
    Interpolators Output;

    Input.Position.xy += float2 (0.5f, 0.5f);
    
    Output.HomogeniousPosition.x = Input.Position.x * screen_res.z * 2.0f - 1.0f;
    Output.HomogeniousPosition.y = (Input.Position.y * screen_res.w * 2.0f - 1.0f) * -1.0f;
    Output.HomogeniousPosition.zw = Input.Position.zw;
    
    Output.DualityTexCoords0 = Input.DualityTexCoords0;
    Output.DualityTexCoords1 = Input.DualityTexCoords1;
    Output.NoiseTexCoords = Input.NoiseTexCoords;
    
    Output.Color = Input.Color;
    Output.Gray = Input.Gray;
    
    return Output;
}
////////////////////////////////////////////////////////////////////////////
