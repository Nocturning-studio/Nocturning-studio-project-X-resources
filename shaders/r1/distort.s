-- distort
function normal		(shader, t_base, t_second, t_detail)
	shader:begin	("null","distort")
			: fog	(false)
			: zb 	(false,false)
	shader:sampler	("s_distort")  	:texture(t_distort)	: clamp() : f_linear ()
	shader:sampler	("s_base")	:texture(t_rt)		: clamp() : f_linear ()
end

