-- normal pp
-- DX11: disabled
--[[
function normal	(shader, t_base, t_second, t_detail)
	shader:begin("stub_notransform_postpr","postprocess")
			: fog(false)
			: zb (false,false)
	shader:sampler("s_image") :texture(t_rt) : clamp() : f_linear ()
	shader:sampler("s_depth") :texture("$user$depth") : clamp() : f_linear ()
	shader:sampler("s_noise") :texture(t_noise) : f_linear ()
	shader:sampler("s_vignette") :texture("vfx\\vfx_vignette") : f_linear ()
end
]]