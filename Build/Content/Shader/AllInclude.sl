#include "AllDefinitions.sl"
	
	
#if SHADER_DX11 || SHADER_DX12
	#pragma pack_matrix( row_major )	
	
	cbuffer cbVertexInput : register( b2 )
	{    
		uint  VtxInput_Offset;
	};
	
	struct STexture2D
	{
		Texture2D		mTexture;
		SamplerState	mSampler;
	};

	struct SVertex_ColorUV
	{
		uint 	Color;
		float2 	UV;		
	};
	
	float4 UNorm4ToFloat4( in uint value )
	{
		return float4(	((value) 		& 0xFF) / 255.0,
						((value>>8) 	& 0xFF) / 255.0,
						((value>>16)	& 0xFF) / 255.0,
						((value>>24) 	& 0xFF) / 255.0);	
	}

 	#define Texture_2D(_texture_)				STexture2D _texture_;
 	#define TextureSample2D(_texture_, _uv_)	_texture_.mTexture.Sample( _texture_.mSampler, _uv_ )

#endif

