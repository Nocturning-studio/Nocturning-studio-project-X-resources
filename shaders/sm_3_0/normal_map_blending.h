/*********************************************************************************/
// Data - 21.06.2023
// Deathman Experemental
// Only non commertial using
/*********************************************************************************/
#ifndef NORMAL_MAP_BLENDING_H_INCLUDED
#define NORMAL_MAP_BLENDING_H_INCLUDED
/*********************************************************************************/
// https://blog.selfshadow.com/publications/blending-in-detail/
/*********************************************************************************/
vector3 Linear_Normal_Map_Blending(vector3 NormalMap_1, vector3 NormalMap_2)
{
    vector3 r = normalize(NormalMap_1 + NormalMap_2);
    return r * 0.5 + 0.5;
}

vector3 Overlay_Normal_Map_Blending(vector3 NormalMap_1, vector3 NormalMap_2)
{
    vector3 r = NormalMap_1 < 0.5 ? 2 * NormalMap_1 * NormalMap_2 : 1 - 2 * (1 - NormalMap_1) * (1 - NormalMap_2);
    r = normalize(r * 2 - 1);
    return r * 0.5 + 0.5;
}

vector3 Partial_Derivative_Normal_Map_Blending(vector3 NormalMap_1, vector3 NormalMap_2)
{
    vector3 r = normalize(
        vector3(NormalMap_1.xy * NormalMap_2.z + NormalMap_2.xy * NormalMap_1.z, NormalMap_1.z * NormalMap_2.z));
    return r * 0.5 + 0.5;
}

vector3 Whiteout_Normal_Map_Blending(vector3 NormalMap_1, vector3 NormalMap_2)
{
    vector3 r = normalize(vector3(NormalMap_1.xy + NormalMap_2.xy, NormalMap_1.z * NormalMap_2.z));
    return r * 0.5 + 0.5;
}

vector3 UDN_Normal_Map_Blending(vector3 NormalMap_1, vector3 NormalMap_2)
{
    vector3 r = normalize(vector3(NormalMap_1.xy + NormalMap_2.xy, NormalMap_1.z));
    return r * 0.5 + 0.5;
}

vector3 Reorienting_Normal_Map_Blending(vector3 NormalMap_1, vector3 NormalMap_2)
{
    vector a = 1 / (1 + NormalMap_1.z);
    vector b = -NormalMap_1.x * NormalMap_1.y * a;

    // Form a basis
    vector3 b1 = vector3(1 - NormalMap_1.x * NormalMap_1.x * a, b, -NormalMap_1.x);
    vector3 b2 = vector3(b, 1 - NormalMap_1.y * NormalMap_1.y * a, -NormalMap_1.y);
    vector3 b3 = NormalMap_1;

    if (NormalMap_1.z < -0.9999999) // Handle the singularity
    {
        b1 = vector3(0, -1, 0);
        b2 = vector3(-1, 0, 0);
    }

    // Rotate n2 via the basis
    vector3 r = NormalMap_2.x * b1 + NormalMap_2.y * b2 + NormalMap_2.z * b3;

    return r * 0.5 + 0.5;
}

vector3 Unity_Normal_Map_Blending(vector3 NormalMap_1, vector3 NormalMap_2)
{
    matrix3x3 nBasis =
        matrix3x3(vector3(NormalMap_1.z, NormalMap_1.y, -NormalMap_1.x), // +90 degree rotation around y axis
                  vector3(NormalMap_1.x, NormalMap_1.z, -NormalMap_1.y), // -90 degree rotation around x axis
                  vector3(NormalMap_1.x, NormalMap_1.y, NormalMap_1.z));

    vector3 r = normalize(NormalMap_2.x * nBasis[0] + NormalMap_2.y * nBasis[1] + NormalMap_2.z * nBasis[2]);
    return r * 0.5 + 0.5;
}

// LVutner
vector3 Blend_Normal_Maps(vector3 NormalMap_1, vector3 NormalMap_2)
{
    NormalMap_1 += vector3(0, 0, 1);
    NormalMap_2 *= vector3(-1, -1, 1);
    return NormalMap_1 * dot(NormalMap_1, NormalMap_2) / NormalMap_1.z - NormalMap_2;
}
/*********************************************************************************/
#endif // NORMAL_MAP_BLENDING_H_INCLUDED
/*********************************************************************************/
