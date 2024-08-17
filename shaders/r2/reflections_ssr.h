////////////////////////////////////////////////////////////////////////////
//	Created		: 03.07.2024
//	Author		: Mihan323
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "gbuffer.h"
////////////////////////////////////////////////////////////////////////////
#define SSR_STEPS 50   //  50  	 // 25 	
#define SSR_STEP_SIZE 2.0  //  2.0     // 1.2
#define SSR_STEP_INCREASE 1.12 //  1.12    // 1.2
#define SSR_REFINE_STEPS 8 		
#define SSR_FAR_PLANE 150 		
#define SSR_THICKNESS 3	
////////////////////////////////////////////////////////////////////////////
float3 remap_sky(float3 vreflect)
{
	float3 vreflectabs = abs(vreflect);
	float vreflectmax = max(vreflectabs.x, max(vreflectabs.y, vreflectabs.z));
	vreflect /= vreflectmax;
	if (vreflect.y < 0.99f)
		vreflect.y = vreflect.y * 2.0f - 1.0f;
	return vreflect;
}

bool is_in_range(float val, float min, float max)
{
	return val >= min && val <= max;
}
	
bool is_in_quad(float2 p, float2 lt = float2(0, 0), float2 rb = float2(1, 1))
{
	return is_in_range(p.x, lt.x, rb.x) && is_in_range(p.y, lt.y, rb.y);
}
	
float4 proj_to_screen(float4 proj)
{
	float4 screen = proj;
	screen.x = (proj.x + proj.w);
	screen.y = (proj.w - proj.y);
	screen.xy *= 0.5;
	return screen;
}
	
float3 calc_vreflect(float3 P, float3 N)
{
	float3 eye = normalize(P);
	float3 vreflect = normalize(reflect(eye, N));
	float4 p_pixel_pos = mul(m_P, float4(P, 1.0f));
	float4 s_pixel_pos = proj_to_screen(p_pixel_pos);
	s_pixel_pos.xy /= s_pixel_pos.w;
	s_pixel_pos.z = P.z;
	float3 reflect_vec = vreflect;
	float3 V_m_point = P + vreflect;
	float4 P_m_point = mul(m_P, float4(V_m_point, 1.0f));
	float4 S_m_point = proj_to_screen(P_m_point);
	S_m_point.xy /= S_m_point.w;
	S_m_point.z = V_m_point.z;
	s_pixel_pos.xy *= s_pixel_pos.z;
	S_m_point.xy *= S_m_point.z;
	return S_m_point - s_pixel_pos;
}
	
void mirror_tc(inout float2 tc)
{
	if(tc.x < 0)
		tc = float2(-tc.x, tc.y - tc.x * 0.5);
	if(tc.x > 1)
		tc = float2(1 - (tc.x - 1), tc.y + (tc.x - 1) * 0.5);
}
	
bool calc_intersection(int steps, inout float scale, inout float thickness, float increment,
	float3 pos, float3 vreflect, inout float3 pos_step, out float3 tc_step)
{
	bool intersected = false;

	for(int i = 0; i < steps; i++)
	{
		pos_step += vreflect * scale;
		tc_step.xy = pos_step.xy / pos_step.z;
		mirror_tc(tc_step.xy);

		if(is_in_range(tc_step.y, 0, 1))
		{
			tc_step.z = GetDepth(tc_step.xy);
			float depth = pos_step.z - tc_step.z;
			if(depth > 0 && depth <= thickness && tc_step.z >= pos.z) 
			{
				intersected = true;
				i = steps;
			}
			else
			{
				scale *= increment;
				thickness *= increment;
			}
		}
		else
		{
			i = steps;
		}
	}
	return intersected;
}
	
// tc - screen space texcoords
// P - view space position
// N - view space normal
// iSample - msaa sample id
float4 calc_ssr(float2 tc, float3 P, float3 N)
{
	float3 pos = float3(tc, 1) * P.z;
	float3 pos_step = pos;
	float3 vreflect = calc_vreflect(P, N);
		
	float depth_scale = clamp(sqrt(P.z), 0.5, 5.0);
	float scale = SSR_STEP_SIZE * depth_scale / SSR_STEPS;
	float thickness = scale * SSR_THICKNESS;
		
	// Ищем пересечение грубым шагом
	float3 tc_step; 
	int intersected = calc_intersection(SSR_STEPS, scale, thickness, SSR_STEP_INCREASE, pos, vreflect, pos_step, tc_step);
		
#ifdef SSR_REFINE_STEPS
	if(intersected) // Если нашли, уточним его более мелким шагом
	{
		pos_step -= vreflect * scale;
		scale /= SSR_REFINE_STEPS;
		// thickness = 10000;
		intersected = calc_intersection(SSR_REFINE_STEPS, scale, thickness, 1.0, pos, vreflect, pos_step, tc_step);
	}
#endif
#ifdef SSR_FAR_PLANE
	if(!intersected) // Проецируем задний план
	{
		float scale = SSR_FAR_PLANE;
		float thickness = scale;
		intersected = calc_intersection(1, scale, thickness, 1.0, pos, vreflect, pos_step, tc_step);
	}
#endif

	if (!intersected || !is_in_quad(tc_step))
	{
		return 0;
	}
	else
	{
		float3 ssr = 0;
		float factor = 0;
		if (intersected && is_in_quad(tc_step.xy))
		{
			factor = 1 - smoothstep(0.1, 0.0, tc_step.y);
			ssr = tex2Dlod0(s_image, tc_step.xy);
		}

		return float4(ssr, factor);
	}
}
////////////////////////////////////////////////////////////////////////////