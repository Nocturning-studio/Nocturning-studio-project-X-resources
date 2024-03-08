/*********************************************************************************/
// Data - 21.06.2023
// Deathman Experemental
// Only non commertial using
/*********************************************************************************/
// https://blog.selfshadow.com/publications/blending-in-detail/
/*********************************************************************************/
float3 Linear_Normal_Map_Blending(float3 NormalMap_1, float3 NormalMap_2)
{
    float3 r = normalize(NormalMap_1 + NormalMap_2);
    return r * 0.5 + 0.5;
}

float3 Overlay_Normal_Map_Blending(float3 NormalMap_1, float3 NormalMap_2)
{
    float3 r = NormalMap_1 < 0.5 ? 2 * NormalMap_1 * NormalMap_2 : 1 - 2 * (1 - NormalMap_1) * (1 - NormalMap_2);
    r = normalize(r * 2 - 1);
    return r * 0.5 + 0.5;
}

float3 Partial_Derivative_Normal_Map_Blending(float3 NormalMap_1, float3 NormalMap_2)
{
    float3 r = normalize(
        float3(NormalMap_1.xy * NormalMap_2.z + NormalMap_2.xy * NormalMap_1.z, NormalMap_1.z * NormalMap_2.z));
    return r * 0.5 + 0.5;
}

float3 Whiteout_Normal_Map_Blending(float3 NormalMap_1, float3 NormalMap_2)
{
    float3 r = normalize(float3(NormalMap_1.xy + NormalMap_2.xy, NormalMap_1.z * NormalMap_2.z));
    return r * 0.5 + 0.5;
}

float3 UDN_Normal_Map_Blending(float3 NormalMap_1, float3 NormalMap_2)
{
    float3 r = normalize(float3(NormalMap_1.xy + NormalMap_2.xy, NormalMap_1.z));
    return r * 0.5 + 0.5;
}

float3 Reorienting_Normal_Map_Blending(float3 NormalMap_1, float3 NormalMap_2)
{
    float a = 1 / (1 + NormalMap_1.z);
    float b = -NormalMap_1.x * NormalMap_1.y * a;

    // Form a basis
    float3 b1 = float3(1 - NormalMap_1.x * NormalMap_1.x * a, b, -NormalMap_1.x);
    float3 b2 = float3(b, 1 - NormalMap_1.y * NormalMap_1.y * a, -NormalMap_1.y);
    float3 b3 = NormalMap_1;

    if (NormalMap_1.z < -0.9999999) // Handle the singularity
    {
        b1 = float3(0, -1, 0);
        b2 = float3(-1, 0, 0);
    }

    // Rotate n2 via the basis
    float3 r = NormalMap_2.x * b1 + NormalMap_2.y * b2 + NormalMap_2.z * b3;

    return r * 0.5 + 0.5;
}

float3 Unity_Normal_Map_Blending(float3 NormalMap_1, float3 NormalMap_2)
{
    float3x3 nBasis =
        float3x3(float3(NormalMap_1.z, NormalMap_1.y, -NormalMap_1.x), // +90 degree rotation around y axis
                  float3(NormalMap_1.x, NormalMap_1.z, -NormalMap_1.y), // -90 degree rotation around x axis
                  float3(NormalMap_1.x, NormalMap_1.y, NormalMap_1.z));

    float3 r = normalize(NormalMap_2.x * nBasis[0] + NormalMap_2.y * nBasis[1] + NormalMap_2.z * nBasis[2]);
    return r * 0.5 + 0.5;
}

// LVutner
float3 Blend_Normal_Maps(float3 NormalMap_1, float3 NormalMap_2)
{
    NormalMap_1 += float3(0, 0, 1);
    NormalMap_2 *= float3(-1, -1, 1);
    return NormalMap_1 * dot(NormalMap_1, NormalMap_2) / NormalMap_1.z - NormalMap_2;
}
/*********************************************************************************/