-- normal pp
function normal	(shader, t_base, t_second, t_detail)
	shader:begin("postprocess","postprocess")
			: fog(false)
			: zb (false,false)
	shader:sampler("t_base") :texture(t_rt) : clamp() : f_linear ()
	shader:sampler("t_depth") :texture("$user$depth") : clamp() : f_linear ()
	shader:sampler("t_noise") :texture(t_noise) : f_linear ()
	shader:sampler("t_vignette") :texture("vfx\\vfx_vignette") : f_linear ()
end
