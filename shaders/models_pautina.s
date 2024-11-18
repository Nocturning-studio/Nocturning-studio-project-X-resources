function normal    (shader, t_base, t_second, t_detail)
  shader:begin  ("model_distort4ghost","particle")    -- particle_alphaonly
      : sorting  (3, true)
      : blend    (true,blend.srccolor,blend.invsrcalpha)
      : aref     (true,0)
      : zb     (true,false)
      : fog    (false)
      : distort   (false)
  shader:sampler  ("s_base")      :texture  (t_base): f_anisotropic() : srgb()
end

function l_special  (shader, t_base, t_second, t_detail)
  shader:begin  ("model_distort4ghost","particle_distort")
      : sorting   (3, true)
      : blend    (true,blend.srcalpha,blend.invsrcalpha)
      : zb        (true,false)
      : fog       (false)
      : distort   (true)
  shader:sampler  ("s_base")      :texture  (t_base): f_anisotropic() : srgb()
  shader:sampler  ("s_distort")   :texture  ("pfx\\pfx_dist_glass")
end

function normal    (shader, t_base, t_second, t_detail)
  shader:begin  ("models_light_panel","models_light_panel")
      : fog      (false)
      : zb       (true,false)
      : blend   (true,blend.srccolor,blend.one)
      : aref    (true,0)
      : sorting  (2, true)
  shader:sampler  ("s_base")      :texture  (t_base): f_anisotropic() : srgb()
end
