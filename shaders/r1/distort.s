-- distort
function normal		(shader, t_base, t_second, t_detail)
	shader:begin	("distort","distort")
			: fog	(false)
			: zb 	(false,false)
	shader:sampler	("t_distort")  	:texture(t_distort)	: clamp() : f_linear ()
	shader:sampler	("t_base")	:texture(t_rt)		: clamp() : f_linear ()
end

