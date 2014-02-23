#include "AllDefinitions.sl"

#if SHADER_DX11
	#pragma pack_matrix( row_major )	
	struct STexture2D
	{
		Texture2D		mTexture;
		SamplerState	mSampler;
	};

 	#define Texture_2D(_texture_)				STexture2D _texture_;
 	#define TextureSample2D(_texture_, _uv_)	_texture_.mTexture.Sample( _texture_.mSampler, _uv_ )

#elif SHADER_DX10
	#pragma pack_matrix( row_major )	
	#define Texture_2D(_texture_)				Texture2D _texture_; SamplerState samp##_texture_;
	#define TextureSample2D(_texture_, _uv_)	_texture_.Sample( samp##_texture_, _uv_ )
#endif

