local tex_base                = "water\\water_water"
local tex_nmap                = "water\\water_normal"
local tex_dist                = "water\\water_dudv"
local tex_env0                = "$user$sky0"
local tex_env1                = "$user$sky1"
local tex_leaves              = "water\\water_foam"
local tex_leaves              = "water\\water_caustic"

function normal                (shader, t_base, t_second, t_detail)
  shader:begin                ("water_soft","water_soft")
        : sorting        (2, false)
        : blend                (true,blend.srcalpha,blend.invsrcalpha)
              : zb                (true,false)
             : distort        (true)
              : fog                (true)
  shader:sampler        ("s_base")       :texture  (tex_base): f_anisotropic() : srgb()
  shader:sampler        ("s_nmap")       :texture  (tex_nmap): f_anisotropic()
  shader:sampler        ("s_env0")       :texture  (tex_env0)   : clamp(): srgb()
  shader:sampler        ("s_env1")       :texture  (tex_env1)   : clamp(): srgb()
  shader:sampler        ("s_image")       :texture  ("$user$generic0") : f_none ()
  shader:sampler        ("s_gbuffer_2")       :texture  ("$user$gbuffer_2") : f_none ()
  shader:sampler        ("s_light_accumulator")       :texture  ("$user$accumulator") : f_none ()
  shader:sampler        ("s_shadow_accumulator")       :texture  ("$user$shadow_accumulator") : f_none ()


  shader:sampler        ("s_leaves")     :texture  (tex_leaves) : wrap()	:f_anisotropic(): srgb()
  shader:sampler        ("s_caustic")     :texture  (tex_caustic) : wrap()	:f_anisotropic(): srgb()
end

function l_special        (shader, t_base, t_second, t_detail)
  shader:begin                ("waterd_soft","waterd_soft")
        : sorting        (2, true)
        : blend                (true,blend.srcalpha,blend.invsrcalpha)
        : zb                (true,false)
        : fog                (false)
        : distort        (true)
  shader:sampler        ("s_base")       :texture  (tex_base): f_anisotropic() : srgb()
  shader:sampler        ("s_distort")    :texture  (tex_dist)
  shader:sampler        ("s_image")       :texture  ("$user$generic0") : f_none ()
  shader:sampler        ("s_gbuffer_2")       :texture  ("$user$gbuffer_2") : f_none ()
end
