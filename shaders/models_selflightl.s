function normal		(shader, t_base, t_second, t_detail)
	shader:begin	("gbuffer_stage_dynamic_mesh_selflight","gbuffer_stage_dynamic_mesh_selflight")
			: fog		(false)
			: emissive 	(true)
	shader:sampler	("s_base")      :texture	(t_base): f_anisotropic() : srgb()
end

function l_special	(shader, t_base, t_second, t_detail)
	shader:begin	("shadow_direct_dynamic_mesh",	"selflight")
			: zb 		(true,false)
			: fog		(false)
			: emissive 	(true)
	shader:sampler	("s_base")      :texture	(t_base): f_anisotropic() : srgb()
end
