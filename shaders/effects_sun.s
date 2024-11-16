function normal(shader, t_base, t_second, t_detail)
  shader:begin("effects_sun","effects_sun")
        : blend             (true,blend.srcalpha,blend.one)
		: zb                (true,false)
		: sampler("s_base") : texture  (t_base): f_anisotropic() : srgb()
end