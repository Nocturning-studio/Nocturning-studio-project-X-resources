////////////////////////////////////////////////////////////////////////////
//	Created		: 08.03.2024
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
struct v_model
{
	float4 P : POSITION;   // (float,float,float,1)
	float3 N : NORMAL;     // (nx,ny,nz)
	float3 T : TANGENT;    // (nx,ny,nz)
	float3 B : BINORMAL;   // (nx,ny,nz)
	float2 tc : TEXCOORD0; // (u,v)
};
////////////////////////////////////////////////////////////////
//	TL0uv
struct	v_TL0uv_positiont
{
	float4	P		: POSITIONT;
	float4	Color	: COLOR; 
};

struct	v_TL0uv
{
	float4	P		: POSITION;
	float4	Color	: COLOR; 
};

struct	v2p_TL0uv
{
	float4	Color	: COLOR;
	float4 	hpos	: SV_Position;	// Clip-space position 	(for rasterization)
};

struct	p_TL0uv
{
	float4	Color	: COLOR;
//	float4 	hpos	: SV_Position;	// Clip-space position 	(for rasterization)
};

////////////////////////////////////////////////////////////////
//	TL
struct	v_TL_positiont
{
	float4	P		: POSITIONT;
	float2	Tex0	: TEXCOORD0;
	float4	Color	: COLOR; 
};

struct	v_TL
{
	float4	P		: POSITION;
	float2	Tex0	: TEXCOORD0;
	float4	Color	: COLOR; 
};

struct	v2p_TL
{
	float2 	Tex0	: TEXCOORD0;
	float4	Color	: COLOR;
	float4 	hpos	: SV_Position;	// Clip-space position 	(for rasterization)
};

struct	p_TL
{
	float2 	Tex0	: TEXCOORD0;
	float4	Color	: COLOR;
//	float4 	hpos	: SV_Position;	// Clip-space position 	(for rasterization)
};

////////////////////////////////////////////////////////////////
//	TL2uv
struct	v_TL2uv
{
	float4	P		: POSITIONT;
	float2	Tex0	: TEXCOORD0;
	float2	Tex1	: TEXCOORD1;
	float4	Color	: COLOR; 
};

struct	v2p_TL2uv
{
	float2 	Tex0	: TEXCOORD0;
	float2	Tex1	: TEXCOORD1;
	float4	Color	: COLOR;
	float4 	hpos	: SV_Position;	// Clip-space position 	(for rasterization)
};

struct	p_TL2uv
{
	float2 	Tex0	: TEXCOORD0;
	float2	Tex1	: TEXCOORD1;
	float4	Color	: COLOR;
//	float4 	hpos	: SV_Position;	// Clip-space position 	(for rasterization)
};
////////////////////////////////////////////////////////////////////////////

