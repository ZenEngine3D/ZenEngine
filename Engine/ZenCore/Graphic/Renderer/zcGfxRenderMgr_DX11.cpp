#include "zcCore.h"

namespace zcMgr { zcGfx::ManagerRender GfxRender; }

namespace zcGfx
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
	meFormatConversion[zenConst::keTexFormat_R8]		= DXGI_FORMAT_R8_UNORM;	
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

void ManagerRender::FrameBegin(zcRes::GfxWindowRef _FrameWindow)
{
	Super::FrameBegin(_FrameWindow);
	mrPreviousDrawcall = NULL;
}

void ManagerRender::FrameEnd()
{	
	mrWindowCurrent->GetProxy()->mDX11pSwapChain->Present( 1, 0 );
	Super::FrameEnd();
}

void ManagerRender::Render(const zenRes::zGfxDrawcall& _rDrawcall)
{
}

void ManagerRender::Render( zArrayDynamic<zenRes::zGfxDrawcall>& _aDrawcalls)
{
	//_aDrawcalls.Sort<>(); //SF Sort element before render
	zenRes::zGfxDrawcall* pDrawcall							= _aDrawcalls.First();
	bool IsValidLastDraw									= mrPreviousDrawcall.IsValid() && mrPreviousDrawcall->mrRenderPass.IsValid();
	zcRes::GfxStateBlendProxyRef		rStateBlend			= IsValidLastDraw ? mrPreviousDrawcall->mrRenderPass->mrProxBlendState : NULL;
	zcRes::GfxStateDepthStencilProxyRef	rStateDepthStencil	= IsValidLastDraw ? mrPreviousDrawcall->mrRenderPass->mrProxDepthStencilState : NULL;
	zcRes::GfxStateRasterizerProxyRef	rStateRaster		= IsValidLastDraw ? mrPreviousDrawcall->mrRenderPass->mrProxRasterState : NULL;
	zcRes::GfxViewProxyRef				rStateView			= IsValidLastDraw ? mrPreviousDrawcall->mrRenderPass->mrProxViewState : NULL;

	for(zUInt i(0), count(_aDrawcalls.Count()); i<count; ++i, ++pDrawcall)
	{	
		if( (*pDrawcall).IsValid() && (*pDrawcall)->mrRenderPass.IsValid() && (*pDrawcall)->mrMeshStrip.IsValid() )
		{
			// GPU State setting
			if( rStateRaster != (*pDrawcall)->mrRenderPass->mrProxRasterState )
			{
				rStateRaster = (*pDrawcall)->mrRenderPass->mrProxRasterState;
				mDX11pContextImmediate->RSSetState(rStateRaster->mpRasterizerState);
			}
			//! @todo Urgent fix blend
			/*
			if( rStateBlend != (*pDrawcall)->mrRenderPass->mrProxBlendState )
			{
				rStateBlend = (*pDrawcall)->mrRenderPass->mrProxBlendState;
				mDX11pContextImmediate->OMSetBlendState( rStateBlend->mpBlendState, rStateBlend->mafBlendFactor, rStateBlend->muSampleMask );
			}*/
			if( rStateDepthStencil != (*pDrawcall)->mrRenderPass->mrProxDepthStencilState )
			{	
				rStateDepthStencil = (*pDrawcall)->mrRenderPass->mrProxDepthStencilState;
				mDX11pContextImmediate->OMSetDepthStencilState(rStateDepthStencil->mpDepthStencilState, rStateDepthStencil->muStencilValue);
			}
			if( rStateView != (*pDrawcall)->mrRenderPass->mrProxViewState )
			{
				UINT maxCount = zenMath::Max( rStateView.IsValid() ? rStateView->muColorCount : 0, (*pDrawcall)->mrRenderPass->mrProxViewState->muColorCount);
				rStateView = (*pDrawcall)->mrRenderPass->mrProxViewState;
				zcMgr::GfxRender.UnbindTextures();
				mDX11pContextImmediate->OMSetRenderTargets(maxCount, rStateView->mpColorViews, rStateView->mpDepthView );
				mDX11pContextImmediate->RSSetViewports( 1, &rStateView->mViewport );
			}
	
			// Shader Inputs Settings

			//Drawcall
			Render( (*pDrawcall)->mrMeshStrip );
		}
	}
	mrPreviousDrawcall = *_aDrawcalls.Last();
}

void ManagerRender::Render(zcRes::GfxMeshProxyRef _rMesh)
{
	for(zUInt stripIdx(0), stripCount(_rMesh->marProxGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx )
		Render( _rMesh->marProxGfxMeshStrip[stripIdx] );
}

void ManagerRender::Render(zcRes::GfxMeshStripProxyRef _rMeshStrip)
{	
	UINT offset = 0;
	const zcRes::GfxVertexProxyRef rVertex			= _rMeshStrip->mrInputStreamProxy->mrVertexProxy;
	const zcRes::GfxIndexProxyRef rIndex			= _rMeshStrip->mrIndexBufferProxy;
	mDX11pContextImmediate->IASetInputLayout		( _rMeshStrip->mrInputStreamProxy->mpInputLayout );
	mDX11pContextImmediate->IASetPrimitiveTopology	( rIndex->mePrimitiveType );
	mDX11pContextImmediate->IASetIndexBuffer		( rIndex->mpIndiceBuffer, rIndex->meIndiceFormat, 0 );
	mDX11pContextImmediate->IASetVertexBuffers		( 0, 1, rVertex->maStreamBuffer.First(), rVertex->maStreamStride.First(), &offset );

	const zcRes::GfxShaderBindingProxyRef	rShaderBind		= _rMeshStrip->mrShaderBindingProxy;
	const zcRes::GfxShaderVertexProxyRef	rShaderVertex	= rShaderBind->mrProxShaderVertex;
	const zcRes::GfxShaderPixelProxyRef		rShaderPixel	= rShaderBind->mrProxShaderPixel;
	zUInt uBufferCount										= _rMeshStrip->marShaderParamProxy.Count();
	mDX11pContextImmediate->VSSetShader( rShaderVertex->mpVertexShader, NULL, 0 );
	mDX11pContextImmediate->PSSetShader( rShaderPixel->mpPixelShader, NULL, 0 );

	for(zUInt bufferIdx=0; bufferIdx<uBufferCount; ++bufferIdx)
	{
		zU32 uShaderMask = rShaderBind->maStagePerParamDef[bufferIdx];
		for(zUInt stageIdx=0; stageIdx<zenConst::keShaderStage__Count; ++stageIdx)
			if( uShaderMask & (1<<stageIdx) )
				_rMeshStrip->marShaderParamProxy[bufferIdx]->Bind(static_cast<zenConst::eShaderStage>(stageIdx));
	}

	//----------------------------------------------------------------------------
	// Assign texture/sampler input for each shader stage
	//----------------------------------------------------------------------------
	ID3D11ShaderResourceView*	aStageTextureViews[zenConst::keShaderStage__Count][zcExp::kuDX11_TexturePerStageMax];
	ID3D11SamplerState*			aStageSamplerState[zenConst::keShaderStage__Count][zcExp::kuDX11_TexturePerStageMax];
	bool						abSamplerChanged[zenConst::keShaderStage__Count];
	bool						abTextureChanged[zenConst::keShaderStage__Count];
	zU16						auPerStageTextureCount[zenConst::keShaderStage__Count];
	zUInt						stageCount(_rMeshStrip->marTextureProxy.Count());
	
	for(zUInt stageIdx(0); stageIdx<stageCount; ++stageIdx)
	{		
		zUInt textureCount					= _rMeshStrip->marTextureProxy[stageIdx].Count();
		auPerStageTextureCount[stageIdx]	= 0;
		abSamplerChanged[stageIdx]			= false;
		abTextureChanged[stageIdx]			= false;
		for( zUInt textureIdx(0); textureIdx<textureCount; ++textureIdx )
		{
			zcRes::GfxTexture2dProxyRef rTexture			= _rMeshStrip->marTextureProxy[stageIdx][textureIdx];
			zcRes::GfxSamplerProxyRef rSampler				= _rMeshStrip->marGfxSamplerProxy[stageIdx][textureIdx];
			if( rTexture.IsValid() && rSampler.IsValid() ) //! @todo Missing: remove test, and make sure we always have default object at worst
			{
				abSamplerChanged[stageIdx]					|= maCurrentSampler[stageIdx][textureIdx] != rSampler;
				abTextureChanged[stageIdx]					|= maCurrentTexture[stageIdx][textureIdx] != rTexture;
				maCurrentSampler[stageIdx][textureIdx]		= rSampler;
				maCurrentTexture[stageIdx][textureIdx]		= rTexture;
				aStageTextureViews[stageIdx][textureIdx]	= rTexture->mpTextureView;
				aStageSamplerState[stageIdx][textureIdx]	= rSampler->mpSamplerState;
				auPerStageTextureCount[stageIdx]			= textureIdx+1;
			}
		}
	}

	// Assign vertex shader textures
	zU16 uTextureCount									= zenMath::Max<zU16>( muPerStageTextureCount[zenConst::keShaderStage_Vertex], auPerStageTextureCount[zenConst::keShaderStage_Vertex]);	
	muPerStageTextureCount[zenConst::keShaderStage_Vertex]	= auPerStageTextureCount[zenConst::keShaderStage_Vertex];
	if( abTextureChanged[zenConst::keShaderStage_Vertex] )	
		DX11GetDeviceContext()->VSSetShaderResources( 0, uTextureCount, aStageTextureViews[zenConst::keShaderStage_Vertex] );
	if( abSamplerChanged[zenConst::keShaderStage_Vertex] )	
		DX11GetDeviceContext()->VSSetSamplers( 0, uTextureCount, aStageSamplerState[zenConst::keShaderStage_Vertex] );	
	
	// Assign pixel shader texture
	uTextureCount										= zenMath::Max<zU16>( muPerStageTextureCount[zenConst::keShaderStage_Pixel], auPerStageTextureCount[zenConst::keShaderStage_Pixel]);	
	muPerStageTextureCount[zenConst::keShaderStage_Pixel]	= auPerStageTextureCount[zenConst::keShaderStage_Pixel];
	if( abTextureChanged[zenConst::keShaderStage_Pixel] )	
		DX11GetDeviceContext()->PSSetShaderResources( 0, uTextureCount, aStageTextureViews[zenConst::keShaderStage_Pixel] );
	if( abSamplerChanged[zenConst::keShaderStage_Pixel] )	
		DX11GetDeviceContext()->PSSetSamplers( 0, uTextureCount, aStageSamplerState[zenConst::keShaderStage_Pixel] );

	//----------------------------------------------------------------------------
	// Draw geometry
	//----------------------------------------------------------------------------
	mDX11pContextImmediate->DrawIndexed( _rMeshStrip->muIndexCount, _rMeshStrip->muIndexFirst, 0 );
}

void ManagerRender::UnbindTextures()
{
	ID3D11ShaderResourceView* StageTextureViews[zcExp::kuDX11_TexturePerStageMax];
	zenMem::Set(StageTextureViews, 0, sizeof(StageTextureViews) );

	for(zUInt stageIdx(0); stageIdx<zenConst::keShaderStage__Count; ++stageIdx)
		for(zUInt texIdx(0); texIdx<zcExp::kuDX11_TexturePerStageMax; ++texIdx)
			maCurrentTexture[stageIdx][texIdx] = NULL;
	
	if( muPerStageTextureCount[zenConst::keShaderStage_Vertex] )
		DX11GetDeviceContext()->VSSetShaderResources( 0, muPerStageTextureCount[zenConst::keShaderStage_Vertex], StageTextureViews );
	if( muPerStageTextureCount[zenConst::keShaderStage_Pixel] )
		DX11GetDeviceContext()->PSSetShaderResources( 0, muPerStageTextureCount[zenConst::keShaderStage_Pixel], StageTextureViews );

	zenMem::Set(muPerStageTextureCount, 0, sizeof(muPerStageTextureCount) );
}

}