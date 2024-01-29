function normal		(shader, t_base, t_second, t_detail)
	shader:begin	("dumb", "dumb")
			: fog		(false)
			: zb 		(false,false)
			: blend		(true,blend.one,blend.one)
--			: aref 		(true,0)	--	enable to save bandwith?
			: sorting	(2, false)
	shader:sampler	("s_vollight")  :texture	("$user$generic2")
end