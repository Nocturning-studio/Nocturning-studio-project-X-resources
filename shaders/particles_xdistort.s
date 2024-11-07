function l_special	(shader, t_base, t_second, t_detail)
	shader:begin	("particle",	"particle_distort")
			: sorting	(3, false)
			: blend		(true,blend.srcalpha,blend.invsrcalpha)
			: zb 		(true,false)
			: fog		(false)
			: distort 	(true)
	shader:sampler	("s_base")      :texture	(t_base)
	shader:sampler	("s_distort")   :texture	(t_base)	-- "pfx\\pfx_distortion"
	shader:sampler  ("s_gbuffer_2")  :texture  ("$user$gbuffer_2")
end
