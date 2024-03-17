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
    float3 UV0: TEXCOORD0;
    float3 UV1: TEXCOORD1;
    float4 hpos: SV_Position;
    float4 Color: COLOR0;
};
//////////////////////////////////////////////////////////////////////////////////////////
Interpolators main (VertexData Input)
{
    Interpolators Output;
    
    Output.hpos = mul (m_WVP, mul (1000.0f, Input.Position));
    Output.hpos.z = Output.hpos.w;
    Output.UV0 = Input.UV0;
    Output.UV1 = Input.UV1;
    Output.Color = Input.Color;
    
    return Output;
}
//////////////////////////////////////////////////////////////////////////////////////////
