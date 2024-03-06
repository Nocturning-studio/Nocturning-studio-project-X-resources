//////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
//////////////////////////////////////////////////////////////////////////////////////////
struct VertexData
{
    float4 Position: POSITION;
    float4 Color: COLOR0;
    float3 UV0: TEXCOORD0;
    float3 UV1: TEXCOORD1;
};

struct Interpolators
{
    float4 HomogeniousPosition: POSITION;
    float4 Color: COLOR0;
    float3 UV0: TEXCOORD0;
    float3 UV1: TEXCOORD1;
};
//////////////////////////////////////////////////////////////////////////////////////////
Interpolators main (VertexData Input)
{
    Interpolators Output;
    
    Output.HomogeniousPosition = mul (m_WVP, mul (1000.0h, Input.Position));
    Output.HomogeniousPosition.z = Output.HomogeniousPosition.w;
    Output.UV0 = Input.UV0;
    Output.UV1 = Input.UV1;
    Output.Color = Input.Color;
    
    return Output;
}
//////////////////////////////////////////////////////////////////////////////////////////
