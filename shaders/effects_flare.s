function normal(shader, t_base, t_second, t_detail)

	shader:begin("effects_lens_flare", "effects_lens_flare")
			: blend(true, blend.srcalpha, blend.one)
			: zb(true, false)

	shader:sampler("s_base")
			: texture(t_base) 
			: srgb()

end