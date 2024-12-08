//////////////////////////////////////////////////////////////////////////////////////////
#include "common.xrh"
//////////////////////////////////////////////////////////////////////////////////////////
struct VertexData
{
    float4 Position: POSITION;
    float4 Color: COLOR0;
    float3 UV: TEXCOORD0;
};

struct Interpolators
{
    float4 HomogeniousPosition: POSITION;
    float4 Color: COLOR0;
    float3 UV: TEXCOORD0;
};
//////////////////////////////////////////////////////////////////////////////////////////
Interpolators main (VertexData Input)
{
    Interpolators Output;
    
    Output.HomogeniousPosition = mul (m_WVP, mul (1000.0f, Input.Position));
    Output.HomogeniousPosition.z = Output.HomogeniousPosition.w;
    Output.UV = Input.UV;
    Output.Color = Input.Color;
    
    return Output;
}
//////////////////////////////////////////////////////////////////////////////////////////
