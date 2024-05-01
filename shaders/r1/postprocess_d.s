-- distort
function normal	(shader, t_base, t_second, t_detail)
	shader:begin("stub_notransform_postpr","postprocess_d")
			: fog(false)
			: zb (false,false)
	shader:sampler("s_distort") :texture(t_distort) : clamp() : f_linear ()
	shader:sampler("s_image") :texture(t_rt) : clamp() : f_linear ()
	shader:sampler("s_depth") :texture("$user$depth") : clamp() : f_linear ()
	shader:sampler("s_noise") :texture(t_noise) : f_linear ()
	shader:sampler("s_vignette") :texture("vfx\\vfx_vignette") : f_linear ()
end
