function normal		(shader, t_base, t_second, t_detail)
	shader:begin	("wmark",	"wmark")
			: sorting	(1, false)
			: blend		(true,blend.destcolor,blend.srccolor)
			: aref 		(true,0)
			: zb 		(true,false)
			: fog		(false)
			: wmark		(true)
	shader:sampler	("s_base") :texture	(t_base)
	shader:sampler  ("s_gbuffer_position") :texture  ("$user$gbuffer_position")
end