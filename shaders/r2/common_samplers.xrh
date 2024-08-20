//////////////////////////////////////////////////////////////////////////////////////////
//	Created		: 27.07.2022
//	Author		: Deathman
//  Nocturning studio for NS Project X
//////////////////////////////////////////////////////////////////////////////////////////
// Geometry phase / deferring
uniform sampler2D s_base;
uniform sampler2D s_bump;
uniform sampler2D s_bumpX;       // (Error for bump)
uniform sampler2D s_detail;
uniform sampler2D s_detailBump;
uniform sampler2D s_detailBumpX; // (Error for bump detail)
uniform sampler2D s_hemi;
uniform sampler2D s_mask;
uniform sampler2D s_baked_ao;        // Baked AO - From texture
uniform sampler2D s_detail_baked_ao; // Baked detail texture AO - From texture
uniform sampler2D s_custom_normal;
uniform sampler2D s_custom_roughness;
uniform sampler2D s_custom_metallness;
uniform sampler2D s_custom_subsurface;
uniform sampler2D s_custom_emissive;
uniform sampler2D s_custom_displacement;
//////////////////////////////////////////////////////////////////////////////////////////
// For terrain mask
uniform sampler2D s_dt_r;
uniform sampler2D s_dt_g;
uniform sampler2D s_dt_b;
uniform sampler2D s_dt_a;
uniform sampler2D s_dn_r;
uniform sampler2D s_dn_g;
uniform sampler2D s_dn_b;
uniform sampler2D s_dn_a;
uniform sampler2D s_dn_rX;
uniform sampler2D s_dn_gX;
uniform sampler2D s_dn_bX;
uniform sampler2D s_dn_aX;
//////////////////////////////////////////////////////////////////////////////////////////
// Jittering
uniform sampler2D jitter0;
uniform sampler2D jitter1;
uniform sampler2D jitter2;
uniform sampler2D jitter3;
uniform sampler2D s_blue_noise;   // Blue noise (textures\\noise\\blue_noise_texture)
uniform sampler2D s_perlin_noise; // Perlin noise (textures\\noise\\perlin_noise_texture)
//////////////////////////////////////////////////////////////////////////////////////////
// Lighting/shadowing phase
uniform sampler s_smap;               // 2D/cube shadowmap
uniform sampler s_lmap;               // 2D/cube projector lightmap
uniform sampler3D s_material;
//////////////////////////////////////////////////////////////////////////////////////////
// Combine phase
uniform sampler2D s_accumulator;
uniform sampler2D s_diffuse_accumulator;
uniform sampler2D s_specular_accumulator;
uniform sampler2D s_bloom;
uniform sampler2D s_distort;
uniform sampler s_image;            // used in various post-processing
uniform sampler2D s_autoexposure;   // actually MidleGray / exp(Lw + eps)
uniform sampler2D s_vollight;

// AO
uniform sampler2D s_ao;              // Raw real time ao
uniform sampler2D s_ao_base;
uniform sampler2D s_ao_blurred1;
uniform sampler2D s_ao_blurred2;

// Debug
uniform sampler2D s_debug_mask;
uniform sampler2D s_debug_diagonal_mask;
uniform sampler2D s_debug_triple_mask;
uniform sampler2D s_debug_quadro_mask;
//////////////////////////////////////////////////////////////////////////////////////////
// Sky
uniform samplerCUBE env_s0;
uniform samplerCUBE env_s1;
uniform samplerCUBE sky_s0;
uniform samplerCUBE sky_s1;
//////////////////////////////////////////////////////////////////////////////////////////
uniform sampler2D s_reflections;
//////////////////////////////////////////////////////////////////////////////////////////