#include "libZenCore.h"

namespace EMgr { EGfx::ManagerRender GfxRender; }

namespace EGfx
{

ManagerRender::ManagerRender()
: mDX11DriverType(D3D_DRIVER_TYPE_NULL)
, mDX11FeatureLevel(D3D_FEATURE_LEVEL_11_0)
, mDX11pDevice(NULL)
, mDX11pContextImmediate(NULL)
{
}

bool ManagerRender::Load()
{	
	HRESULT hr	= S_OK;
	
	//-------------------------------------------------------------------------
	// Configure Texture Format mapping
	//-------------------------------------------------------------------------
	zenMem::Set(meFormatConversion, DXGI_FORMAT_UNKNOWN, sizeof(meFormatConversion) );	
	meFormatConversion[zenConst::keTexFormat_R8]			= DXGI_FORMAT_R8_UNORM;	
	meFormatConversion[zenConst::keTexFormat_RGB8]		= DXGI_FORMAT_UNKNOWN;
	meFormatConversion[zenConst::keTexFormat_RGBA8]		= DXGI_FORMAT_R8G8B8A8_UNORM;
	meFormatConversion[zenConst::keTexFormat_D24S8]		= DXGI_FORMAT_D24_UNORM_S8_UINT;
	meFormatConversion[zenConst::keTexFormat_D32]		= DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	meFormatConversion[zenConst::keTexFormat_BC1]		= DXGI_FORMAT_BC1_UNORM;	
	meFormatConversion[zenConst::keTexFormat_BC2]		= DXGI_FORMAT_BC2_UNORM;
	meFormatConversion[zenConst::keTexFormat_BC3]		= DXGI_FORMAT_BC3_UNORM;
	meFormatConversion[zenConst::keTexFormat_BC5]		= DXGI_FORMAT_BC5_UNORM;
	meFormatConversion[zenConst::keTexFormat_BC7]		= DXGI_FORMAT_BC7_UNORM;
	meFormatConversion[zenConst::keTexFormat_RGBA32f]	= DXGI_FORMAT_R32_FLOAT;

	UINT createDeviceFlags = 0;
	#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels	= ARRAYSIZE( featureLevels );
	mDX11DriverType			= D3D_DRIVER_TYPE_HARDWARE;	//Only support hardware dx11 support

	hr =  D3D11CreateDevice(
		NULL, 
		mDX11DriverType,	
		NULL, 
		createDeviceFlags, 
		featureLevels, 
		numFeatureLevels,
		D3D11_SDK_VERSION, 
		&mDX11pDevice, 
		&mDX11FeatureLevel, 
		&mDX11pContextImmediate
		);

	if( FAILED( hr ) )
		return FALSE;
	
	zenMem::Set(muPerStageTextureCount, 0, sizeof(muPerStageTextureCount));
	return TRUE;
}

bool ManagerRender::Unload()
{
	if( mDX11pContextImmediate )	mDX11pContextImmediate->ClearState();
	if( mDX11pContextImmediate )	mDX11pContextImmediate->Release();
	if( mDX11pDevice )				mDX11pDevice->Release();	
	return true;
}

void ManagerRender::FrameBegin(ERes::GfxWindowRef _FrameWindow)
{
	Super::FrameBegin(_FrameWindow);
}

void ManagerRender::FrameEnd()
{	
	mrWindowCurrent->Get().mDX11pSwapChain->Present( 1, 0 );
	EMgr::GfxState.SetView(NULL);
	EMgr::GfxState.PipelineUpdate();
	Super::FrameEnd();
}

void ManagerRender::Render(ERes::GfxMeshRef _rMesh)
{
	for(zUInt stripIdx(0), stripCount(_rMesh->Get().maGfxMeshStripRef.Count()); stripIdx<stripCount; ++stripIdx )
		Render( _rMesh->Get().maGfxMeshStripRef[stripIdx] );
}

void ManagerRender::Render(ERes::GfxMeshStripRef _rMeshStrip)
{	
	EMgr::GfxState.PipelineUpdate();

	UINT offset = 0;
	const ERes::GfxVertexRef rVertex					= _rMeshStrip->Get().mrInputStream->Get().mrVertex;
	const ERes::GfxIndexRef rIndex						= _rMeshStrip->Get().mrIndexBuffer;
	mDX11pContextImmediate->IASetInputLayout		( _rMeshStrip->Get().mrInputStream->Get().mpInputLayout );
	mDX11pContextImmediate->IASetPrimitiveTopology	( rIndex->Get().mePrimitiveType );
	mDX11pContextImmediate->IASetIndexBuffer		( rIndex->Get().mpIndiceBuffer, rIndex->Get().meIndiceFormat, 0 );
	mDX11pContextImmediate->IASetVertexBuffers		( 0, 1, rVertex->Get().maStreamBuffer.First(), rVertex->Get().maStreamStride.First(), &offset );

	const ERes::GfxShaderBindingRef		rShaderBind		= _rMeshStrip->Get().mrShaderBinding;
	const ERes::GfxShaderVertexRef		rShaderVertex	= rShaderBind->Get().maGfxShaderRef[EExp::keShaderStage_Vertex];
	const ERes::GfxShaderPixelRef		rShaderPixel	= rShaderBind->Get().maGfxShaderRef[EExp::keShaderStage_Pixel];
	zUInt uBufferCount									= _rMeshStrip->Get().maShaderParamRef.Count();
	mDX11pContextImmediate->VSSetShader( rShaderVertex->Get().mpVertexShader, NULL, 0 );				
	mDX11pContextImmediate->PSSetShader( rShaderPixel->Get().mpPixelShader, NULL, 0 );					
	for(zUInt bufferIdx=0; bufferIdx<uBufferCount; ++bufferIdx)
	{
		zU32 uShaderMask = rShaderBind->Get().maStagePerParamDef[bufferIdx];
		for(zUInt stageIdx=0; stageIdx<EExp::keShaderStage__Count; ++stageIdx)
			if( uShaderMask & (1<<stageIdx) )
				_rMeshStrip->Get().maShaderParamRef[bufferIdx]->Bind(static_cast<EExp::eShaderStage>(stageIdx));
	}

	//----------------------------------------------------------------------------
	// Assign texture/sampler input for each shader stage
	//----------------------------------------------------------------------------
	ID3D11ShaderResourceView*	aStageTextureViews[EExp::keShaderStage__Count][EExp::kuDX11_TexturePerStageMax];
	ID3D11SamplerState*			aStageSamplerState[EExp::keShaderStage__Count][EExp::kuDX11_TexturePerStageMax];
	bool						abSamplerChanged[EExp::keShaderStage__Count];
	bool						abTextureChanged[EExp::keShaderStage__Count];
	zU16							auPerStageTextureCount[EExp::keShaderStage__Count];
	zUInt						stageCount(_rMeshStrip->Get().maTextureRef.Count());
	
	for(zUInt stageIdx(0); stageIdx<stageCount; ++stageIdx)
	{		
		zUInt textureCount					= _rMeshStrip->Get().maTextureRef[stageIdx].Count();
		auPerStageTextureCount[stageIdx]	= 0;
		abSamplerChanged[stageIdx]			= false;
		abTextureChanged[stageIdx]			= false;
		for( zUInt textureIdx(0); textureIdx<textureCount; ++textureIdx )
		{
			ERes::GfxTexture2DRef rTexture					= _rMeshStrip->Get().maTextureRef[stageIdx][textureIdx];
			ERes::GfxSamplerRef rSampler					= _rMeshStrip->Get().maGfxSamplerRef[stageIdx][textureIdx];
			if( rTexture.IsValid() && rSampler.IsValid() ) //! @todo remove test, and make sure we always have default object at worst
			{
				abSamplerChanged[stageIdx]					|= maCurrentSampler[stageIdx][textureIdx] != rSampler;
				abTextureChanged[stageIdx]					|= maCurrentTexture[stageIdx][textureIdx] != rTexture;
				maCurrentSampler[stageIdx][textureIdx]		= rSampler;
				maCurrentTexture[stageIdx][textureIdx]		= rTexture;
				aStageTextureViews[stageIdx][textureIdx]	= rTexture->Get().mpTextureView;
				aStageSamplerState[stageIdx][textureIdx]	= rSampler->Get().mpSamplerState;
				auPerStageTextureCount[stageIdx]			= textureIdx+1;
			}
		}
	}

	// Assign vertex shader textures
	zU16 uTextureCount									= zenMath::Max<zU16>( muPerStageTextureCount[EExp::keShaderStage_Vertex], auPerStageTextureCount[EExp::keShaderStage_Vertex]);	
	muPerStageTextureCount[EExp::keShaderStage_Vertex]	= auPerStageTextureCount[EExp::keShaderStage_Vertex];
	if( abTextureChanged[EExp::keShaderStage_Vertex] )	
		DX11GetDeviceContext()->VSSetShaderResources( 0, uTextureCount, aStageTextureViews[EExp::keShaderStage_Vertex] );
	if( abSamplerChanged[EExp::keShaderStage_Vertex] )	
		DX11GetDeviceContext()->VSSetSamplers( 0, uTextureCount, aStageSamplerState[EExp::keShaderStage_Vertex] );	
	
	// Assign pixel shader texture
	uTextureCount										= zenMath::Max<zU16>( muPerStageTextureCount[EExp::keShaderStage_Pixel], auPerStageTextureCount[EExp::keShaderStage_Pixel]);	
	muPerStageTextureCount[EExp::keShaderStage_Pixel]	= auPerStageTextureCount[EExp::keShaderStage_Pixel];
	if( abTextureChanged[EExp::keShaderStage_Pixel] )	
		DX11GetDeviceContext()->PSSetShaderResources( 0, uTextureCount, aStageTextureViews[EExp::keShaderStage_Pixel] );
	if( abSamplerChanged[EExp::keShaderStage_Pixel] )	
		DX11GetDeviceContext()->PSSetSamplers( 0, uTextureCount, aStageSamplerState[EExp::keShaderStage_Pixel] );

	//----------------------------------------------------------------------------
	// Draw geometry
	//----------------------------------------------------------------------------
	mDX11pContextImmediate->DrawIndexed( _rMeshStrip->Get().muIndexCount, _rMeshStrip->Get().muIndexFirst, 0 );
}

void ManagerRender::UnbindTextures()
{
	ID3D11ShaderResourceView* StageTextureViews[EExp::kuDX11_TexturePerStageMax];
	zenMem::Set(StageTextureViews, 0, sizeof(StageTextureViews) );

	for(zUInt stageIdx(0); stageIdx<EExp::keShaderStage__Count; ++stageIdx)
		for(zUInt texIdx(0); texIdx<EExp::kuDX11_TexturePerStageMax; ++texIdx)
			maCurrentTexture[stageIdx][texIdx] = NULL;
	
	if( muPerStageTextureCount[EExp::keShaderStage_Vertex] )
		DX11GetDeviceContext()->VSSetShaderResources( 0, muPerStageTextureCount[EExp::keShaderStage_Vertex], StageTextureViews );
	if( muPerStageTextureCount[EExp::keShaderStage_Pixel] )
		DX11GetDeviceContext()->PSSetShaderResources( 0, muPerStageTextureCount[EExp::keShaderStage_Pixel], StageTextureViews );

	zenMem::Set(muPerStageTextureCount, 0, sizeof(muPerStageTextureCount) );
}

}