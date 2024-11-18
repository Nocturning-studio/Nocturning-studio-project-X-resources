////////////////////////////////////////////////////////////////////////////
//	Created		: 09.03.2024
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
uniform float4 dof_params; // x - near, y - focus, z - far, w - sky distance
uniform float3 dof_kernel; // x, y - resolution pre-scaled, z - just kernel size
////////////////////////////////////////////////////////////////////////////
float DOFFactor(float Depth)
{
    float dist_to_focus = Depth - dof_params.y;
    float blur_far = saturate(dist_to_focus / (dof_params.z - dof_params.y));
    float blur_near = saturate(dist_to_focus / (dof_params.x - dof_params.y));
    float blur = blur_near + blur_far;
    blur = pow(blur, EXP);
    return blur;
}
////////////////////////////////////////////////////////////////////////////
