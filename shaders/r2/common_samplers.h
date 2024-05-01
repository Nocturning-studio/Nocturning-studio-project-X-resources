//////////////////////////////////////////////////////////////////////////////////////////
//	Created		: 27.07.2022
//	Author		: Deathman
//  Nocturning studio for NS Project X
//////////////////////////////////////////////////////////////////////////////////////////
// Geometry phase / deferring
uniform_sampler2D(s_base		, smp_base);
uniform_sampler2D(s_bump		, smp_base);
uniform_sampler2D(s_bumpX		, smp_base); // (Error for bump)
uniform_sampler2D(s_detail		, smp_base);
uniform_sampler2D(s_detailBump	, smp_base);
uniform_sampler2D(s_detailBumpX	, smp_base); // (Error for bump detail)
uniform_sampler2D(s_hemi		, smp_linear);
uniform_sampler2D(s_mask		, smp_linear);
//////////////////////////////////////////////////////////////////////////////////////////
// For terrain mask
uniform_sampler2D(s_dt_r	, smp_base);
uniform_sampler2D(s_dt_g	, smp_base);
uniform_sampler2D(s_dt_b	, smp_base);
uniform_sampler2D(s_dt_a	, smp_base);
uniform_sampler2D(s_dn_r	, smp_base);
uniform_sampler2D(s_dn_g	, smp_base);
uniform_sampler2D(s_dn_b	, smp_base);
uniform_sampler2D(s_dn_a	, smp_base);
uniform_sampler2D(s_dn_rX	, smp_base);
uniform_sampler2D(s_dn_gX	, smp_base);
uniform_sampler2D(s_dn_bX	, smp_base);
uniform_sampler2D(s_dn_aX	, smp_base);
//////////////////////////////////////////////////////////////////////////////////////////
// Jittering
uniform_sampler2D(jitter0			, smp_jitter);
uniform_sampler2D(jitter1			, smp_jitter);
uniform_sampler2D(jitter2			, smp_jitter);
uniform_sampler2D(jitter3			, smp_jitter);
uniform_sampler2D(s_blue_noise		, smp_jitter); // Blue noise (textures\\noise\\blue_noise_texture)
uniform_sampler2D(s_perlin_noise	, smp_jitter); // Perlin noise (textures\\noise\\perlin_noise_texture)
//////////////////////////////////////////////////////////////////////////////////////////
// Lighting/shadowing phase
uniform_sampler2D(s_smap		, smp_smap); // 2D/cube shadowmap
uniform_sampler2D(s_lmap		, smp_linear); // 2D/cube projector lightmap
uniform_sampler3D(s_material	, smp_material);
//////////////////////////////////////////////////////////////////////////////////////////
// Combine phase
uniform_sampler2D(s_accumulator				, smp_rtlinear);
uniform_sampler2D(s_diffuse_accumulator		, smp_rtlinear);
uniform_sampler2D(s_specular_accumulator	, smp_rtlinear);
uniform_sampler2D(s_bloom					, smp_rtlinear);
uniform_sampler2D(s_distort					, smp_rtlinear);
uniform_sampler2D(s_image					, smp_rtlinear); // used in various post-processing
uniform_sampler2D(s_autoexposure			, smp_rtlinear); // actually MidleGray / exp(Lw + eps)
uniform_sampler2D(s_vollight				, smp_rtlinear);

// AO
uniform_sampler2D(s_baked_ao		, smp_base); // Baked AO - From texture
uniform_sampler2D(s_detail_baked_ao	, smp_base); // Baked detail texture AO - From texture
uniform_sampler2D(s_ao				, smp_rtlinear); // Raw real time ao
uniform_sampler2D(s_ao_base			, smp_rtlinear);
uniform_sampler2D(s_ao_blurred1		, smp_rtlinear);
uniform_sampler2D(s_ao_blurred2		, smp_rtlinear);

// Debug
uniform_sampler2D(s_debug_mask				, smp_rtlinear);
uniform_sampler2D(s_debug_diagonal_mask		, smp_rtlinear);
uniform_sampler2D(s_debug_triple_mask		, smp_rtlinear);
uniform_sampler2D(s_debug_quadro_mask		, smp_rtlinear);

// Overlay
uniform_sampler2D(s_photo_grid		, smp_rtlinear);
uniform_sampler2D(s_cinema_borders	, smp_rtlinear);
uniform_sampler2D(s_watermark		, smp_rtlinear);

// Visual posteffects
uniform_sampler2D(s_vignette, smp_rtlinear);
//////////////////////////////////////////////////////////////////////////////////////////
// Sky
uniform_samplerCUBE(env_s0, smp_base);
uniform_samplerCUBE(env_s1, smp_base);
uniform_samplerCUBE(sky_s0, smp_base);
uniform_samplerCUBE(sky_s1, smp_base);
//////////////////////////////////////////////////////////////////////////////////////////