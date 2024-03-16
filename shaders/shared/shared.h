////////////////////////////////////////////////////////////////////////////
//	Created		: 08.03.2024
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
//Common
#include <common_legacy.h>

// original samplers
sampler smp_nofilter; // point
sampler smp_rtlinear; // bilinear
sampler smp_linear; // trilinear (wrap)
sampler smp_base; // anisotropic (wrap)

#include <shared_math.h>
#include <shared_constants.h>
#include <shared_defines.h>
#include <shared_uniforms.h>
#include <shared_iostructs.h>
#include <shared_functions.h>

//Effects
#include <fog.h>
#include <depth_of_field_common.h>
#include <chromatic_abberations.h>
#include <tonemapping.h>

//Z-Buffer
#include <zbuffer.h>
////////////////////////////////////////////////////////////////////////////
