function normal(shader, t_base, t_second, t_detail)

	shader:begin("effects_sun", "effects_sun")
			: blend(true, blend.one, blend.one)
			: zb(true, false)

	shader:sampler("s_base")
			: texture(t_base) 
			: srgb()

end