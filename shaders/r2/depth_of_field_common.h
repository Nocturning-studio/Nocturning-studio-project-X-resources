////////////////////////////////////////////////////////////////////////////
//	Created		: 21.12.2022
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "gbuffer.h"
////////////////////////////////////////////////////////////////////////////
float DOFFactor(float2 TexCoords)
{
    float depth = GetDepth(TexCoords);
    float dist_to_focus = depth - dof_params.y;
    float blur_far = saturate(dist_to_focus / (dof_params.z - dof_params.y));
    float blur_near = saturate(dist_to_focus / (dof_params.x - dof_params.y));
    float blur = blur_near + blur_far;
    blur = pow(blur, 2.0f);
    return blur;
}
////////////////////////////////////////////////////////////////////////////