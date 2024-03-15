#ifndef HLSL_3_0_SYNTAX
#define HLSL_3_0_SYNTAX

	// 18.05.2020

	/*
			You can use:

			Init sampler template: uniform_sampler2D (texturename, samplernme);
			You Sampler have texturename

			uniform_sampler...		(...)
			uniform_sampler...Reg	(... number)
			uniform_sampler...RegS	(... shader, number)

			uniform_sampler...Comparison		(...)
			uniform_sampler...ComparisonReg		(... number)
			uniform_sampler...ComparisonRegS	(... shader, number)

			uniform_sampler...ARRAY		(...)
			uniform_sampler...ARRAYReg	(... number)
			uniform_sampler...ARRAYRegS	(... shader, number)

			You can create samplers from exists texture: 
			
			create_sampler...		(name, ...)

			create_sampler...Group	(name, size) 
			{
				set_element_samplerGroup(name0), 
				..., 
				set_element_samplerGroup(namex)
			}

			You can use: SamplerState smp_..., SamplerComparisonState smp_...

			In functions you can use samplers, template:

			float3 sample_zero_my_texture(in type_sampler2D(name), in float2 tc)
			{
				int level = 0;
				return tex2Dlod(name, float4(tc, 0, level));
			}

			float3 sample_zero_my_texture_group(in type_sampler2DGroup(name), unsigned int id, in float2 tc)
			{
				int level = 0;
				return tex2Dlod(get_element_samplerGroup(name, id), float4(tc, 0, level));
			}

			Argument: d_name, but tex2D(name...)

			How use function:

			float2 tc : TEXCOORDX;
			uniform_sampler2D (name, smp_my);
			sample_zero_my_texture(get_sampler_name_from_tex(name), tc)

			You can get texture and sampler from Sampler...

			Texture2D s_mynew_onlytex = get_dx11_style_texture_object_from_dx9_style_sampler_class(name);
			sampler s_mynew_onlysmp = get_dx11_style_sampler_object_from_dx9_style_sampler_class(name);

			//---------------------------

			float tc : TEXCOORDX;

			tex1Dsize(name)
			tex1Ddimensions(name)
			tex1D(name, tc)
			tex1Doffset(name, tc, offset)
			tex1Dgrad(name, tc, ddx, ddy)
			tex1Dgradoffset(name, tc, ddx, ddy, offset)
			tex1Dfetch(name, tc * screen resolution)
			tex1Dfetchoffset(name, tc * screen resolution, offset)
			tex1Dlod(name, float4(tc, 0, 0, level))
			tex1Dlodoffset(name, float4(tc, 0, 0, level), offset)
			tex1Dcmplod(name, float4(tc, 0, 0, level))
			tex1Dcmplodoffset(name, float4(tc, 0, 0, level), offset)
			tex1Dcmpzero(name, tc)
			tex1Dcmpzerooffset(name, tc, offset)
			tex1Dbias(name, tc, bias)

			//---------------------------

			float tc : TEXCOORDX;
			id - id of array element

			tex1DARRAY(name, float2(tc, id))
			tex1DARRAYproj(name, float2(tc, id))
			tex1DARRAYcmplod(name, float4(tc, id, 0, level))
			tex1DARRAYlod(name, float4(tc, id, 0, level))
			tex1DARRAYfetch(name, float2(tc * screen resolution, id))
			tex1DARRAYbias(name, float2(tc, id), bias)

			//---------------------------

			float2 tc : TEXCOORDX;

			tex2Dsize(name)
			tex2Ddimensions(name)
			tex2Dgather(name, tc)
			tex2Dgatheroffset(name, tc, offset)
			tex2D(name, tc)
			tex2Doffset(name, tc, offset)
			tex2Dproj(name, tc)
			tex2Dgrad(name, tc, ddx, ddy)
			tex2Dgradoffset(name, tc, ddx, ddy, offset)
			tex2Dfetch(name, tc * screen resolution)
			tex2Dfetchoffset(name, tc * screen resolution, offset)
			tex2Dlod(name, float4(tc, 0, level))
			tex2Dlodoffset(name, float4(tc, 0, level), offset)
			tex2Dcmplod(name, float4(tc, 0, level))
			tex2Dcmplodoffset(name, float4(tc, 0, level), offset)
			tex2Dcmpzero(name, tc)
			tex2Dcmpzerooffset(name, tc, offset)
			tex2Dbias(name, tc, bias)

			//---------------------------

			float2 tc : TEXCOORDX;
			id - id of array element

			tex2DARRAY(name, float3(tc, id))
			tex2DARRAYproj(name, float3(tc, id))
			tex2DARRAYfetch(name, float3(tc * screen resolution, id))
			tex2DARRAYbias(name, float3(tc, id), bias)

			//---------------------------

			float3 tc : TEXCOORDX;

			tex3Dsize(name)
			tex3Ddimensions(name, mipall)
			tex3D(name, tc)
			tex3Doffset(name, tc, offset)
			tex3Dgrad(name, tc, ddx, ddy)
			tex3Dgradoffset(name, tc, ddx, ddy, offset)
			tex3Dfetch(name, tc * screen resolution)
			tex3Dfetchoffset(name, tc * screen resolution, offset)
			tex3Dlod(name, float4(tc, level))
			tex3Dlodoffset(name, float4(tc, level), offset)
			tex3Dbias(name, tc, bias)

			//---------------------------

			float3 tc : TEXCOORDX;

			texCUBEsize(name)
			texCUBEdimensions(name)
			texCUBEgather(name, tc)
			texCUBEgatheroffset(name, tc, offset)
			texCUBE(name, tc)
			texCUBEoffset(name, tc, offset)
			texCUBEgrad(name, tc, ddx, ddy)
			texCUBEgradoffset(name, tc, ddx, ddy, offset)
			texCUBElod(name, float4(tc, level))
			texCUBElodoffset(name, float4(tc, level), offset)
			texCUBEcmplod(name, float4(tc, level))
			texCUBEcmplodoffset(name, float4(tc, level), offset)
			texCUBEcmpzero(name, tc)
			texCUBEcmpzerooffset(name, tc, offset)
			texCUBEbias(name, tc, bias)
			
			//---------------------------

			float3 tc : TEXCOORDX;
			id - id of array element

			texCUBEARRAY(name, float3(tc, id))
			texCUBEARRAYlod(name, float3(tc, id), level)
			texCUBEARRAYbias(name, float3(tc, id), bias)

	*/

	//------------------------- STRUCTOR -------------------------

	#define GENERATE_SAMPLER_TYPE( stype ) struct stype

	//------------------------- HACK BECAUSE I CANT USE sampler... -------------------------

	#define get_sampler_name_from_tex(name) d_##name

	// Array

	#define type_sampler1DArray(name)	\
		sampler1DArray 		\
		get_sampler_name_from_tex(name)
	#define type_sampler2DArray(name)	\
		sampler2DArray 		\
		get_sampler_name_from_tex(name)
	#define type_samplerCUBEArray(name)	\
		samplerCUBEArray 	\
		get_sampler_name_from_tex(name)

	// Ordinary

	#define type_sampler1D(name)	\
		sampler1D_		\
		get_sampler_name_from_tex(name)
	#define type_sampler2D(name)	\
		sampler2D_		\
		get_sampler_name_from_tex(name)
	#define type_sampler3D(name)	\
		sampler3D_		\
		get_sampler_name_from_tex(name)
	#define type_samplerCUBE(name)	\
		samplerCUBE_ 	\
		get_sampler_name_from_tex(name)

	#define type_sampler1DComparison(name)		\
		sampler1DComparison 	\
		get_sampler_name_from_tex(name)
	#define type_sampler2DComparison(name)		\
		sampler2DComparison 	\
		get_sampler_name_from_tex(name)
	#define type_sampler3DComparison(name)		\
		sampler3DComparison 	\
		get_sampler_name_from_tex(name)
	#define type_samplerCUBEComparison(name)	\
		samplerCUBEComparison 	\
		get_sampler_name_from_tex(name)

	// Group

	#define set_element_samplerGroup(element_name) get_sampler_name_from_tex(element_name)
	#define get_element_samplerGroup(group_name, number) group_name[number]

	#define type_sampler1DGroup(name, size)		\
		type_sampler1D(name)[size]
	#define type_sampler2DGroup(name, size)		\
		type_sampler2D(name)[size]
	#define type_sampler3DGroup(name, size)		\
		type_sampler3D(name)[size]
	#define type_samplerCUBEGroup(name, size)	\
		type_samplerCUBE(name)[size]

	//------------------------- GENERATE SAMPLERS -------------------------

		//-------------------------------------------------- 1D

			//------------------ ARRAY

			// sampler1DARRAY
			GENERATE_SAMPLER_TYPE( sampler1DARRAY )
			{
				SamplerState 	smp;
				Texture1DArray 	tex;
			};

			//------------------ NOT ARRAY

			// sampler1DComparison
			GENERATE_SAMPLER_TYPE( sampler1DComparison )
			{
				SamplerComparisonState 	smp;
				Texture1D 				tex;
			};

			// sampler1D_
			GENERATE_SAMPLER_TYPE( sampler1D_ )
			{
				SamplerState 	smp;
				Texture1D 		tex;
			};

		//-------------------------------------------------- 2D

			//------------------ ARRAY

			// sampler2DARRAY
			GENERATE_SAMPLER_TYPE( sampler2DARRAY )
			{
				SamplerState 	smp;
				Texture2DArray 	tex;
			};

			//------------------ NOT ARRAY

			// sampler2DComparison
			GENERATE_SAMPLER_TYPE( sampler2DComparison )
			{
				SamplerComparisonState 	smp;
				Texture2D 				tex;
			};

			// sampler2D_
			GENERATE_SAMPLER_TYPE( sampler2D_ )
			{
				SamplerState 	smp;
				Texture2D 		tex;
			};

		//-------------------------------------------------- 3D

			//------------------ NOT ARRAY

			// sampler3DComparison
			GENERATE_SAMPLER_TYPE( sampler3DComparison )
			{
				SamplerComparisonState 	smp;
				Texture3D 				tex;
			};

			// sampler3D_
			GENERATE_SAMPLER_TYPE( sampler3D_ )
			{
				SamplerState 	smp;
				Texture3D 		tex;
			};

		//-------------------------------------------------- CUBEMAP

			//------------------ ARRAY

			// samplerCUBEARRAY
			GENERATE_SAMPLER_TYPE( samplerCUBEARRAY )
			{
				SamplerState 		smp;
				TextureCubeArray 	tex;
			};

			//------------------ NOT ARRAY

			// samplerCUBEComparison
			GENERATE_SAMPLER_TYPE( samplerCUBEComparison )
			{
				SamplerComparisonState 	smp;
				TextureCube 			tex;
			};

			// samplerCUBE_
			GENERATE_SAMPLER_TYPE( samplerCUBE_ )
			{
				SamplerState 	smp;
				TextureCube 	tex;
			};

	//------------------------- GENERATE SAMPLERS FROM TEXTURES AND DX11SAMPLERS -------------------------

		#define create_sampler1D( name, x, y ) 		\
			static type_sampler1D(name) = { y,  x }

		#define create_sampler2D( name, x, y ) 		\
			static type_sampler2D(name) = { y,  x }

		#define create_sampler3D( name, x, y ) 		\
			static type_sampler3D(name) = { y,  x }

		#define create_samplerCUBE( name, x, y ) 	\
			static type_samplerCUBE(name) = { y,  x }

		#define create_sampler1DGroup( name, size ) 	\
			static const type_sampler1DGroup(name, size) =
		#define create_sampler2DGroup( name, size ) 	\
			static const type_sampler2DGroup(name, size) =
		#define create_sampler3DGroup( name, size ) 	\
			static const type_sampler3DGroup(name, size) =
		#define create_samplerCUBEGroup( name, size ) 	\
			static const type_samplerCUBEGroup(name, size) =

	//------------------------- GENERATE SAMPLERS UNIFORMS -------------------------

		#define RegT(num) t##num
		#define RegU(num) u##num

		//-------------------------------------------------- 1D

			#define uniform_sampler1DARRAY( x, y ) \
				uniform Texture1DArray x; \
				static type_sampler1DARRAY(x) = { y,  x }
			#define uniform_sampler1DARRAYReg( x, y, num ) \
				uniform Texture1DArray x : register(RegT(num)); \
				static type_sampler1DARRAY(x) = { y,  x }
			#define uniform_sampler1DARRAYRegS( x, y, shader, num ) \
				uniform Texture1DArray x : register(shader, RegT(num)); \
				static type_sampler1DARRAY(x) = { y,  x }

			#define uniform_sampler1DComparison( x, y ) \
				uniform Texture1D x; \
				static type_sampler1DComparison(x) = { y,  x }
			#define uniform_sampler1DComparisonReg( x, y, num ) \
				uniform Texture1D x : register(RegT(num)); \
				static type_sampler1DComparison(x) = { y,  x }
			#define uniform_sampler1DComparisonRegS( x, y, shader, num ) \
				uniform Texture1D x : register(shader, RegT(num)); \
				static type_sampler1DComparison(x) = { y,  x }

			#define uniform_sampler1D( x, y ) \
				uniform Texture1D x; \
				create_sampler1D(x, x, y)
			#define uniform_sampler1DReg( x, y, num ) \
				uniform Texture1D x : register(RegT(num)); \
				create_sampler1D(x, x, y)
			#define uniform_sampler1DRegS( x, y, shader, num ) \
				uniform Texture1D x : register(shader, RegT(num)); \
				create_sampler1D(x, x, y)

		//-------------------------------------------------- 2D

			#define uniform_sampler2DARRAY( x, y ) \
				uniform Texture2DArray x; \
				static type_sampler2DArray(x) = { y,  x }
			#define uniform_sampler2DARRAYReg( x, y, num ) \
				uniform Texture2DArray x : register(RegT(num)); \
				static type_sampler2DArray(x) = { y,  x }
			#define uniform_sampler2DARRAYRegS( x, y, shader, num ) \
				uniform Texture2DArray x : register(shader, RegT(num)); \
				static type_sampler2DArray(x) = { y,  x }

			#define uniform_sampler2DComparison( x, y ) \
				uniform Texture2D x; \
				static type_sampler2DComparison(x) = { y,  x }
			#define uniform_sampler2DComparisonReg( x, y, num ) \
				uniform Texture2D x : register(RegT(num)); \
				static type_sampler2DComparison(x) = { y,  x }
			#define uniform_sampler2DComparisonRegS( x, y, shader, num ) \
				uniform Texture2D x : register(shader, RegT(num)); \
				static type_sampler2DComparison(x) = { y,  x }

			#define uniform_sampler2D( x, y ) \
				uniform Texture2D x; \
				create_sampler2D(x, x, y)
			#define uniform_sampler2DReg( x, y, num ) \
				uniform Texture2D x : register(RegT(num)); \
				create_sampler2D(x, x, y)
			#define uniform_sampler2DRegS( x, y, shader, num ) \
				uniform Texture2D x : register(shader, RegT(num)); \
				create_sampler2D(x, x, y)

		//-------------------------------------------------- 3D

			// You need array of 3D samplers??? NO! YOU CAN'T IMPORT IT!

			#define uniform_sampler3DComparison( x, y ) \
				uniform Texture3D x; \
				static type_sampler3DComparison(x) = { y,  x }
			#define uniform_sampler3DComparisonReg( x, y, num ) \
				uniform Texture3D x : register(RegT(num)); \
				static type_sampler3DComparison(x) = { y,  x }
			#define uniform_sampler3DComparisonRegS( x, y, shader, num ) \
				uniform Texture3D x : register(shader, RegT(num)); \
				static type_sampler3DComparison(x) = { y,  x }

			#define uniform_sampler3D( x, y ) \
				uniform Texture3D x; \
				create_sampler3D(x, x, y)
			#define uniform_sampler3DReg( x, y, num ) \
				uniform Texture3D x : register(RegT(num)); \
				create_sampler3D(x, x, y)
			#define uniform_sampler3DRegS( x, y, shader, num ) \
				uniform Texture3D x : register(shader, RegT(num)); \
				create_sampler3D(x, x, y)

		//-------------------------------------------------- CUBEMAP

			#define uniform_samplerCUBEARRAY( x, y ) \
				uniform TextureCubeArray x; \
				static type_samplerCUBEArray(x) = { y,  x }
			#define uniform_samplerCUBEARRAYReg( x, y, num ) \
				uniform TextureCubeArray x : register(RegT(num)); \
				static type_samplerCUBEArray(x) = { y,  x }
			#define uniform_samplerCUBEARRAYRegS( x, y, shader, num ) \
				uniform TextureCubeArray x : register(shader, RegT(num)); \
				static type_samplerCUBEArray(x) = { y,  x }

			#define uniform_samplerCUBEComparison( x, y ) \
				uniform TextureCube x; \
				static type_samplerCUBEComparison(x) = { y,  x }
			#define uniform_samplerCUBEComparisonReg( x, y, num ) \
				uniform TextureCube x : register(RegT(num)); \
				static type_samplerCUBEComparison(x) = { y,  x }
			#define uniform_samplerCUBEComparisonRegS( x, y, shader, num ) \
				uniform TextureCube x : register(shader, RegT(num)); \
				static type_samplerCUBEComparison(x) = { y,  x }

			#define uniform_samplerCUBE( x, y ) \
				uniform TextureCube x; \
				create_samplerCUBE(x, x, y)
			#define uniform_samplerCUBEReg( x, y, num ) \
				uniform TextureCube x : register(RegT(num)); \
				create_samplerCUBE(x, x, y)
			#define uniform_samplerCUBERegS( x, y, shader, num ) \
				uniform TextureCube x : register(shader, RegT(num)); \
				create_samplerCUBE(x, x, y)

	//------------------------- SAMPLERS METHODS LINKS -------------------------

		//-------------------------------------------------- 1D

			#define tex1DARRAY				\
				_Sample				// template: tex1DARRAY				(name, tc)
			#define tex1DARRAYproj			\
				_Sample1A2DProj		// template: tex1DARRAYproj			(name, tc)
			#define tex1DARRAYcmplod		\
				_SampleCL2D			// template: tex1DARRAYcmplod		(name, float4(tc, 0, level))
			#define tex1DARRAYlod 			\
				_SampleLevel2D		// template: tex1DARRAYlod			(name, float4(tc, 0, level))
			#define tex1DARRAYfetch 		\
				_Load3				// template: tex1DARRAYfetch		(name, tc * screen resolution)
			#define tex1DARRAYbias			\
				_SampleBias			// template: tex1DARRAYbias			(name, tc, bias)
			#define tex1D					\
				_Sample				// template: tex1D					(name, tc)
			#define tex1Doffset				\
				_SampleOff			// template: tex1Doffset			(name, tc, offset)
			#define tex1Dgrad				\
				_SampleGrad			// template: tex1Dgrad				(name, tc, ddx, ddy)
			#define tex1Dgradoffset			\
				_SampleGradOff		// template: tex1Dgradoffset		(name, tc, ddx, ddy, offset)
			#define tex1Dfetch 				\
				_Load2				// template: tex1Dfetch				(name, tc * screen resolution)
			#define tex1Dfetchoffset 		\
				_Load2Off			// template: tex1Dfetchoffset		(name, tc * screen resolution, offset)
			#define tex1Dlod 				\
				_SampleLevel1D		// template: tex1Dlod				(name, float4(tc, 0, 0, level))
			#define tex1Dlodoffset 			\
				_SampleLevel1DOff	// template: tex1Dlodoffset			(name, float4(tc, 0, 0, level), offset)
			#define tex1Dcmplod				\
				_SampleCL1D			// template: tex1Dcmplod			(name, float4(tc, 0, 0, level))
			#define tex1Dcmplodoffset		\
				_SampleCL1DOff		// template: tex1Dcmplodoffset		(name, float4(tc, 0, 0, level), offset)
			#define tex1Dcmpzero			\
				_SampleCL0			// template: tex1Dcmpzero			(name, tc)
			#define tex1Dcmpzerooffset		\
				_SampleCL0Off		// template: tex1Dcmpzerooffset		(name, tc, offset)
			#define tex1Dbias				\
				_SampleBias			// template: tex1Dbias				(name, tc, bias)

		//-------------------------------------------------- 2D

			#define tex2DARRAY				\
				_Sample				// template: tex2DARRAY				(name, tc)
			#define tex2DARRAYproj			\
				_Sample2DARRAYProj	// template: tex2DARRAYproj			(name, tc)
			#define tex2DARRAYfetch			\
				_Load4				// template: tex2DARRAYfetch		(name, tc * screen resolution)
			#define tex2DARRAYbias			\
				_SampleBias			// template: tex2DARRAYbias			(name, tc, bias)
			#define tex2Dgather				\
				_Gather				// template: tex2Dgather			(name, tc)
			#define tex2Dgatheroffset		\
				_GatherOff			// template: tex2Dgatheroffset		(name, tc, offset)
			#define tex2D					\
				_Sample				// template: tex2D					(name, tc)
			#define tex2Doffset				\
				_SampleOff			// template: tex2Doffset			(name, tc, offset)
			#define tex2Dproj				\
				_Sample1A2DProj		// template: tex2Dproj				(name, tc)
			#define tex2Dgrad				\
				_SampleGrad			// template: tex2Dgrad				(name, tc, ddx, ddy)
			#define tex2Dgradoffset			\
				_SampleGradOff		// template: tex2Dgradoffset		(name, tc, ddx, ddy, offset)
			#define tex2Dfetch 				\
				_Load3				// template: tex2Dfetch				(name, tc * screen resolution)
			#define tex2Dfetchoffset 		\
				_Load3Off			// template: tex2Dfetchoffset		(name, tc * screen resolution, offset)
			#define tex2Dlod 				\
				_SampleLevel2D		// template: tex2Dlod				(name, float4(tc, 0, level))
			#define tex2Dlodoffset 			\
				_SampleLevel2DOff	// template: tex2Dlodoffset			(name, float4(tc, 0, level), offset)
			#define tex2Dcmplod				\
				_SampleCL2D			// template: tex2Dcmplod			(name, float4(tc, 0, level))
			#define tex2Dcmplodoffset		\
				_SampleCL2DOff		// template: tex2Dcmplodoffset		(name, float4(tc, 0, level), offset)
			#define tex2Dcmpzero			\
				_SampleCL0			// template: tex2Dcmpzero			(name, tc)
			#define tex2Dcmpzerooffset		\
				_SampleCL0Off		// template: tex2Dcmpzerooffset		(name, tc, offset)
			#define tex2Dbias				\
				_SampleBias			// template: tex2Dbias				(name, tc, bias)

		//-------------------------------------------------- 3D

			#define tex3D					\
				_Sample				// template: tex3D					(name, tc)
			#define tex3Doffset				\
				_SampleOff			// template: tex3Doffset			(name, tc, offset)
			#define tex3Dgrad				\
				_SampleGrad			// template: tex3Dgrad				(name, tc, ddx, ddy)
			#define tex3Dgradoffset			\
				_SampleGradOff		// template: tex3Dgradoffset		(name, tc, ddx, ddy, offset)
			#define tex3Dfetch 				\
				_Load4				// template: tex3Dfetch				(name, tc * screen resolution)
			#define tex3Dfetchoffset 		\
				_Load4Off			// template: tex3Dfetchoffset		(name, tc * screen resolution, offset)
			#define tex3Dlod 				\
				_SampleLevel_C		// template: tex3Dlod				(name, float4(tc, level))
			#define tex3Dlodoffset 			\
				_SampleLevel_COff	// template: tex3Dlodoffset			(name, float4(tc, level), offset)
			#define tex3Dbias				\
				_SampleBias			// template: tex3Dbias				(name, tc, bias)

		//-------------------------------------------------- CUBEMAP

			#define texCUBEARRAY			\
				_Sample				// template: texCUBEARRAY			(name, tc)
			#define texCUBEARRAYlod			\
				_SampleLevel_CA		// template: texCUBEARRAYlod		(name, tc, level)
			#define texCUBEARRAYbias		\
				_SampleBias			// template: texCUBEARRAYbias		(name, tc, bias)
			#define texCUBEgather			\
				_Gather				// template: texCUBEgather			(name, tc)
			#define texCUBEgatheroffset		\
				_GatherOff			// template: texCUBEgatheroffset	(name, tc, offset)
			#define texCUBE					\
				_Sample				// template: texCUBE				(name, tc)
			#define texCUBEoffset			\
				_SampleOff			// template: texCUBEoffset			(name, tc, offset)
			#define texCUBEgrad				\
				_SampleGrad			// template: texCUBEgrad			(name, tc, ddx, ddy)
			#define texCUBEgradoffset		\
				_SampleGradOff		// template: texCUBEgradoffset		(name, tc, ddx, ddy, offset)
			#define texCUBElod 				\
				_SampleLevel_C		// template: texCUBElod				(name, float4(tc, level))
			#define texCUBElodoffset 		\
				_SampleLevel_COff	// template: texCUBElodoffset		(name, float4(tc, level), offset)
			#define texCUBEcmplod			\
				_SampleCLCUBE		// template: texCUBEcmplod			(name, float4(tc, level))
			#define texCUBEcmplodoffset		\
				_SampleCLCUBEOff	// template: texCUBEcmplodoffset	(name, float4(tc, level), offset)
			#define texCUBEcmpzero			\
				_SampleCL0			// template: texCUBEcmpzero			(name, tc)
			#define texCUBEcmpzerooffset	\
				_SampleCL0Off		// template: texCUBEcmpzerooffset	(name, tc, offset)
			#define texCUBEbias				\
				_SampleBias			// template: texCUBEbias			(name, tc, bias)

	//------------------------- SAMPLERS METHODS -------------------------

		#define get_dx11_style_texture_object_from_dx9_style_sampler_class(name) get_sampler_name_from_tex(name).tex
		#define get_dx11_style_sampler_object_from_dx9_style_sampler_class(name) get_sampler_name_from_tex(name).smp

		// tex1Dlod
		#define _SampleLevel1D( name, tc_level ) 				\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).SampleLevel( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc_level.x, tc_level.w )

		// tex1Dlodoffset
		#define _SampleLevel1DOff( name, tc_level, offset ) 	\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).SampleLevel( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc_level.x, tc_level.w, offset )

		// tex1Dcmplod
		#define _SampleCL1D( name, tc_level ) 					\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).SampleCmpLevel( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc_level.x, tc_level.w )

		// tex1Dcmplodoffset
		#define _SampleCL1DOff( name, tc_level, offset ) 		\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).SampleCmpLevel( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc_level.x, tc_level.w, offset )

		// tex2DARRAYproj
		#define _Sample2DARRAYProj( name, tc ) 					\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).Sample( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc.xyz / tc.www )

		// tex2Dproj, tex1DARRAYproj
		#define _Sample1A2DProj( name, tc ) 					\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).Sample( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc.xy / tc.ww )

		// tex2Dlod, tex1DARRAYlod
		#define _SampleLevel2D( name, tc_level ) 				\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).SampleLevel( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc_level.xy, tc_level.w )

		// tex2Dlodoffset
		#define _SampleLevel2DOff( name, tc_level, offset ) 	\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).SampleLevel( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc_level.xy, tc_level.w, offset )

		// tex2Dcmplod, tex1DARRAYcmplod
		#define _SampleCL2D( name, tc_level ) 					\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).SampleCmpLevel( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc_level.xy, tc_level.w )

		// tex2Dcmplodoffset
		#define _SampleCL2DOff( name, tc_level, offset ) 		\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).SampleCmpLevel( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc_level.xy, tc_level.w, offset )

		// texCUBEARRAYlod
		#define _SampleLevel_CA( name, tc, level ) 				\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).SampleLevel( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc, level )

		// tex1D, tex2D, tex3D, texCUBE, tex1DARRAY, tex2DARRAY, texCUBEARRAY
		#define _Sample( name, tc ) 							\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).Sample( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc )

		// tex1Doffset, tex2Doffset, tex3Doffset, texCUBEoffset
		#define _SampleOff( name, tc, offset ) 					\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).Sample( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc, offset ) // offset can be int1 (tex1D), int2 (tex2D), int3 (tex3D)

		// tex1Dgrad, tex2Dgrad, tex3Dgrad, texCUBEgrad
		#define _SampleGrad( name, tc, _ddx, _ddy ) 			\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).SampleGrad( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc, _ddx, _ddy )

		// tex1Dgradoffset, tex2Dgradoffset, tex3Dgradoffset, texCUBEgradoffset
		#define _SampleGradOff( name, tc, _ddx, _ddy, offset ) 	\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).SampleGrad( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc, _ddx, _ddy, offset )

		// tex1Dbias, tex2Dbias, tex3Dbias, texCUBEbias, tex1DARRAYbias, tex2DARRAYbias, texCUBEARRAYbias
		#define _SampleBias( name, tc, bias ) 					\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).SampleBias( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc, bias )

		// tex1Dfetch
		#define _Load2( name, tc )								\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).Load( int2(tc, 0), 0 )

		// tex1Dfetchoffset
		#define _Load2Off( name, tc, texeloff )					\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).Load( int2(tc, 0), 0, texeloff )

		// tex2Dfetch, tex1DARRAYfetch
		#define _Load3( name, tc )								\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).Load( int3(tc, 0) ) // get_dx11_style_texture_object_from_dx9_style_sampler_class(name).Load( int3(tc, level) )

		// tex2Dfetchoffset
		#define _Load3Off( name, tc, texeloff )					\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).Load( int3(tc, 0), texeloff )

		// tex3Dfetch, tex2DARRAYfetch
		#define _Load4( name, tc )								\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).Load( int4(tc, 0), 0 )

		// tex3Dfetchoffset
		#define _Load4Off( name, tc, texeloff )					\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).Load( int4(tc, 0), 0, texeloff )

		// tex3Dlod, texCUBElod
		#define _SampleLevel_C( name, tc_level ) 				\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).SampleLevel( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc_level.xyz, tc_level.w )

		// tex3Dlodoffset, texCUBElodoffset
		#define _SampleLevel_COff( name, tc_level, offset ) 	\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).SampleLevel( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc_level.xyz, tc_level.w, offset )

		// tex1Dcmpzero, tex2Dcmpzero, texCUBEcmpzero
		#define _SampleCL0( name, tc ) 							\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).SampleCmpLevelZero( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc )

		// tex1Dcmpzerooffset, tex2Dcmpzerooffset, texCUBEcmpzerooffset
		#define _SampleCL0Off( name, tc, offset ) 				\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).SampleCmpLevelZero( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc, offset )

		// texCUBEcmplod
		#define _SampleCLCUBE( name, tc_level ) 				\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).SampleCmpLevel( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc_level.xyz, tc_level.w )

		// texCUBEcmplodoffset
		#define _SampleCLCUBEOff( name, tc_level, offset ) 		\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).SampleCmpLevel( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc_level.xyz, tc_level.w, offset )

		// tex2Dgather, texCUBEgather
		#define _Gather( name, tc ) 							\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).Gather( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc )

		// tex2Dgatheroffset, texCUBEgatheroffset
		#define _GatherOff( name, tc, offset ) 					\
			get_dx11_style_texture_object_from_dx9_style_sampler_class(name).Gather( get_dx11_style_sampler_object_from_dx9_style_sampler_class(name), tc, offset )

	//------------------------- TEXTURES IN SAMPLES FUNCTIONS LINKS -------------------------

		#define tex1Dsize( name )				\
			_GetSize1D( get_sampler_name_from_tex(name) )
		#define tex1Ddimensions( name )			\
			_GetDimensions1D( get_sampler_name_from_tex(name) )
		#define tex2Dsize( name )				\
			_GetSize2D( get_sampler_name_from_tex(name) )
		#define tex2Ddimensions( name )			\
			_GetDimensions2D( get_sampler_name_from_tex(name) )
		#define tex3Dsize( name )		 		\
			_GetSize3D( get_sampler_name_from_tex(name) )
		#define tex3Ddimensions( name, mip )	\
			_GetDimensions3D( get_sampler_name_from_tex(name), mip )
		#define texCUBEsize( name )				\
			_GetSizeCUBE( get_sampler_name_from_tex(name) )
		#define texCUBEdimensions( name )		\
			_GetDimensionsCUBE( get_sampler_name_from_tex(name) )

	//------------------------- TEXTURES IN SAMPLES FUNCTIONS -------------------------

		//-------------------------------------------------- 1D

			// sampler

			uint _GetSize1D( type_sampler1D(name))
			{
				uint w; 
				get_dx11_style_texture_object_from_dx9_style_sampler_class(name).GetDimensions( w ); 
				return w;
			}

			uint3 _GetDimensions1D( type_sampler1D(name))
			{
				uint mipcurr, w, mipall; 
				get_dx11_style_texture_object_from_dx9_style_sampler_class(name).GetDimensions( mipcurr, w, mipall ); 
				return uint3( mipcurr, w, mipall );
			}

			// SamplerComparisonState

			uint _GetSize1D( type_sampler1DComparison(name))
			{
				uint w; 
				get_dx11_style_texture_object_from_dx9_style_sampler_class(name).GetDimensions( w ); 
				return w;
			}

			uint3 _GetDimensions1D( type_sampler1DComparison(name))
			{
				uint mipcurr, w, mipall; 
				get_dx11_style_texture_object_from_dx9_style_sampler_class(name).GetDimensions( mipcurr, w, mipall ); 
				return uint3( mipcurr, w, mipall );
			}

		//-------------------------------------------------- 2D

			// sampler

			uint2 _GetSize2D( type_sampler2D(name))
			{
				uint w, h; 
				get_dx11_style_texture_object_from_dx9_style_sampler_class(name).GetDimensions( w, h ); 
				return uint2( w, h );
			}

			uint4 _GetDimensions2D( type_sampler2D(name))
			{
				uint mipcurr, w, h, mipall; 
				get_dx11_style_texture_object_from_dx9_style_sampler_class(name).GetDimensions( mipcurr, w, h, mipall ); 
				return uint4( mipcurr, w, h, mipall );
			}

			// SamplerComparisonState

			uint2 _GetSize2D( type_sampler2DComparison(name))
			{
				uint w, h; 
				get_dx11_style_texture_object_from_dx9_style_sampler_class(name).GetDimensions( w, h ); 
				return uint2( w, h );
			}

			uint4 _GetDimensions2D( type_sampler2DComparison(name))
			{
				uint mipcurr, w, h, mipall; 
				get_dx11_style_texture_object_from_dx9_style_sampler_class(name).GetDimensions( mipcurr, w, h, mipall ); 
				return uint4( mipcurr, w, h, mipall );
			}

		//-------------------------------------------------- 3D

			// sampler

			uint3 _GetSize3D( type_sampler3D(name))
			{
				uint w, h, d; 
				get_dx11_style_texture_object_from_dx9_style_sampler_class(name).GetDimensions( w, h, d ); 
				return uint3( w, h, d );
			}

			uint4 _GetDimensions3D( type_sampler3D(name), bool mipall_out )
			{
				uint mipcurr, w, h, d, mipall; 
				get_dx11_style_texture_object_from_dx9_style_sampler_class(name).GetDimensions( mipcurr, w, h, d, mipall ); 

				if( mipall_out ) 
					mipcurr = mipall; 

				return uint4( mipcurr, w, h, d );
			}

			// SamplerComparisonState

			uint3 _GetSize3D( type_sampler3DComparison(name))
			{
				uint w, h, d; 
				get_dx11_style_texture_object_from_dx9_style_sampler_class(name).GetDimensions( w, h, d ); 
				return uint3( w, h, d );
			}

			uint4 _GetDimensions3D( type_sampler3DComparison(name), bool mipall_out )
			{
				uint mipcurr, w, h, d, mipall; 
				get_dx11_style_texture_object_from_dx9_style_sampler_class(name).GetDimensions( mipcurr, w, h, d, mipall ); 

				if( mipall_out ) 
					mipcurr = mipall; 

				return uint4( mipcurr, w, h, d );
			}

		//-------------------------------------------------- CUBE

			// sampler
			uint2 _GetSizeCUBE( type_samplerCUBE(name))
			{
				uint w, h; 
				get_dx11_style_texture_object_from_dx9_style_sampler_class(name).GetDimensions( w, h ); 
				return uint2( w, h );
			}

			uint4 _GetDimensionsCUBE( type_samplerCUBE(name))
			{
				uint mipcurr, w, h, mipall; 
				get_dx11_style_texture_object_from_dx9_style_sampler_class(name).GetDimensions( mipcurr, w, h, mipall ); 
				return uint4( mipcurr, w, h, mipall );
			}

			// SamplerComparisonState

			uint2 _GetSizeCUBE( type_samplerCUBEComparison(name))
			{
				uint w, h; 
				get_dx11_style_texture_object_from_dx9_style_sampler_class(name).GetDimensions( w, h ); 
				return uint2( w, h );
			}

			uint4 _GetDimensionsCUBE( type_samplerCUBEComparison(name))
			{
				uint mipcurr, w, h, mipall; 
				get_dx11_style_texture_object_from_dx9_style_sampler_class(name).GetDimensions( mipcurr, w, h, mipall ); 
				return uint4( mipcurr, w, h, mipall );
			}

	//-------------------------  -------------------------

#endif