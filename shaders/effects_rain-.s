function normal(shader, t_base, t_second, t_detail)

	shader:begin("effects_rain", "effects_rain")
		: blend(true, blend.srcalpha, blend.one)
		: aref(true, 0)
		: sorting(0, false)
		: zb(true, false)

	shader:sampler("s_base") 
        : texture(t_base) 
        : srgb()

end