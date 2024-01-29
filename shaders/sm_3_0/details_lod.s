function l_special	(shader, t_base, t_second, t_detail)
	shader:begin	("gbuffer_stage_multiple_object_billboard","gbuffer_stage_multiple_object_billboard")
			: blend	(false, blend.one, blend.zero)
			: zb	(true,  true)
			: fog	(false)
	shader:sampler	("s_base")      :texture	(t_base)
	shader:sampler	("s_hemi")      :texture	(t_base .. "_nm")
end
