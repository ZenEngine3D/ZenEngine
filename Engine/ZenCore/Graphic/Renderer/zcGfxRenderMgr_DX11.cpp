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
	UnbindResources();
	Super::FrameEnd();
}

void ManagerRender::Render(const zenRes::zGfxDrawcall& _rDrawcall)
{
}

void ManagerRender::UpdateGPUState(const zenRes::zGfxDrawcall& _rDrawcall, RenderContext& _Context)
{
	if( _Context.mrRenderpass != _rDrawcall->mrRenderPass )
	{			
		zcRes::GfxRenderPassProxyRef& rRenderpass	= _Context.mrRenderpass;
		_Context.mrRenderpass						= _rDrawcall->mrRenderPass;
		if( _Context.mrStateRaster != rRenderpass->mrProxRasterState )
		{
			_Context.mrStateRaster = rRenderpass->mrProxRasterState;
			mDX11pContextImmediate->RSSetState(_Context.mrStateRaster->mpRasterizerState);
		}
		if( _Context.mrStateBlend != rRenderpass->mrProxBlendState )
		{
			_Context.mrStateBlend = rRenderpass->mrProxBlendState;
			mDX11pContextImmediate->OMSetBlendState( _Context.mrStateBlend->mpBlendState, _Context.mrStateBlend->mafBlendFactor, _Context.mrStateBlend->muSampleMask );
		}
		if( mbResourceUnbound || _Context.mrStateDepthStencil != rRenderpass->mrProxDepthStencilState )
		{	
			_Context.mrStateDepthStencil = rRenderpass->mrProxDepthStencilState;
			mDX11pContextImmediate->OMSetDepthStencilState(_Context.mrStateDepthStencil->mpDepthStencilState, _Context.mrStateDepthStencil->muStencilValue);
		}
		if( mbResourceUnbound || _Context.mrStateView != rRenderpass->mrProxViewState )
		{
			UINT maxCount = zenMath::Max( _Context.mrStateView.IsValid() ? _Context.mrStateView->muColorCount : 0, _Context.mrRenderpass->mrProxViewState->muColorCount);
			_Context.mrStateView = rRenderpass->mrProxViewState;
			zcMgr::GfxRender.UnbindTextures();
			mDX11pContextImmediate->OMSetRenderTargets(maxCount, _Context.mrStateView->mpColorViews, _Context.mrStateView->mpDepthView );
			mDX11pContextImmediate->RSSetViewports( 1, &_Context.mrStateView->mViewport );
		}
	}
}

void ManagerRender::UpdateShaderState(const zenRes::zGfxDrawcall& _rDrawcall, RenderContext& _Context)
{
	UINT UnusedOffset = 0;
	const zcRes::GfxMeshStripProxyRef& rMeshStrip		= _rDrawcall->mrMeshStrip;
	const zcRes::GfxVertexProxyRef& rVertex				= rMeshStrip->mrInputStreamProxy->mrVertexProxy;
	const zcRes::GfxIndexProxyRef& rIndex				= rMeshStrip->mrIndexBufferProxy;
	const zcRes::GfxShaderBindingProxyRef rShaderBind	= rMeshStrip->mrShaderBindingProxy;
	if( _Context.mrInputStream != rMeshStrip->mrInputStreamProxy )
	{
		_Context.mrInputStream = rMeshStrip->mrInputStreamProxy;
		mDX11pContextImmediate->IASetInputLayout( rMeshStrip->mrInputStreamProxy->mpInputLayout );
	}
	if( _Context.mePrimitiveType != rIndex->mePrimitiveType )
	{
		_Context.mePrimitiveType = rIndex->mePrimitiveType;
		mDX11pContextImmediate->IASetPrimitiveTopology( rIndex->mePrimitiveType );
	}		
	if( _Context.mrShaderVertex != rShaderBind->mrProxShaderVertex )
	{
		_Context.mrShaderVertex = rShaderBind->mrProxShaderVertex;
		mDX11pContextImmediate->VSSetShader( _Context.mrShaderVertex->mpVertexShader, NULL, 0 );
	}
	if( _Context.mrShaderPixel != rShaderBind->mrProxShaderPixel )
	{
		_Context.mrShaderPixel = rShaderBind->mrProxShaderPixel;
		mDX11pContextImmediate->PSSetShader( _Context.mrShaderPixel->mpPixelShader, NULL, 0 );
	}
	
	//! @todo clean revise this
	for(zUInt bufferIdx=0, bufferCount(rMeshStrip->marShaderParamProxy.Count()); bufferIdx<bufferCount; ++bufferIdx)
	{
		zU32 uShaderMask = rShaderBind->maStagePerParamDef[bufferIdx];
		for(zUInt stageIdx=0; stageIdx<zenConst::keShaderStage__Count; ++stageIdx)
			if( uShaderMask & (1<<stageIdx) )
				rMeshStrip->marShaderParamProxy[bufferIdx]->Bind(static_cast<zenConst::eShaderStage>(stageIdx));
	}

	mDX11pContextImmediate->IASetIndexBuffer		( rIndex->mpIndiceBuffer, rIndex->meIndiceFormat, 0 );
	mDX11pContextImmediate->IASetVertexBuffers		( 0, 1, rVertex->maStreamBuffer.First(), rVertex->maStreamStride.First(), &UnusedOffset );

	//----------------------------------------------------------------------------
	// Assign texture/sampler input for each shader stage
	//----------------------------------------------------------------------------
	ID3D11ShaderResourceView*	aStageTextureViews[zenConst::keShaderStage__Count][zcExp::kuDX11_TexturePerStageMax];
	ID3D11SamplerState*			aStageSamplerState[zenConst::keShaderStage__Count][zcExp::kuDX11_TexturePerStageMax];
	bool						abSamplerChanged[zenConst::keShaderStage__Count];
	bool						abTextureChanged[zenConst::keShaderStage__Count];
	zU16						auPerStageTextureCount[zenConst::keShaderStage__Count];
	zUInt						stageCount(rMeshStrip->marTextureProxy.Count());
	
	//! @todo Clean : avoid this loop for each drawcall?
	for(zUInt stageIdx(0); stageIdx<stageCount; ++stageIdx)
	{		
		zUInt textureCount					= rMeshStrip->marTextureProxy[stageIdx].Count();
		auPerStageTextureCount[stageIdx]	= 0;
		abSamplerChanged[stageIdx]			= false;
		abTextureChanged[stageIdx]			= mbTextureUnbound;
		for( zUInt textureIdx(0); textureIdx<textureCount; ++textureIdx )
		{
			zcRes::GfxTexture2dProxyRef rTexture			= rMeshStrip->marTextureProxy[stageIdx][textureIdx];
			zcRes::GfxSamplerProxyRef rSampler				= rMeshStrip->marGfxSamplerProxy[stageIdx][textureIdx];
			if( rTexture.IsValid() && rSampler.IsValid() ) //! @todo Missing: remove test, and make sure we always have default object at worst
			{
				abSamplerChanged[stageIdx]						|= _Context.maCurrentSampler[stageIdx][textureIdx] != rSampler;
				abTextureChanged[stageIdx]						|= _Context.maCurrentTexture[stageIdx][textureIdx] != rTexture;
				_Context.maCurrentSampler[stageIdx][textureIdx]	= rSampler;
				_Context.maCurrentTexture[stageIdx][textureIdx]	= rTexture;
				aStageTextureViews[stageIdx][textureIdx]		= rTexture->mpTextureView;
				aStageSamplerState[stageIdx][textureIdx]		= rSampler->mpSamplerState;
				auPerStageTextureCount[stageIdx]				= textureIdx+1;
			}
		}
	}

	// Assign vertex shader textures	
	zU16 uTextureCount												= zenMath::Max<zU16>( _Context.muPerStageTextureCount[zenConst::keShaderStage_Vertex], auPerStageTextureCount[zenConst::keShaderStage_Vertex]);	
	_Context.muPerStageTextureCount[zenConst::keShaderStage_Vertex] = auPerStageTextureCount[zenConst::keShaderStage_Vertex];
	if( abTextureChanged[zenConst::keShaderStage_Vertex] )	
		DX11GetDeviceContext()->VSSetShaderResources( 0, uTextureCount, aStageTextureViews[zenConst::keShaderStage_Vertex] );
	if( abSamplerChanged[zenConst::keShaderStage_Vertex] )	
		DX11GetDeviceContext()->VSSetSamplers( 0, uTextureCount, aStageSamplerState[zenConst::keShaderStage_Vertex] );	
	
	// Assign pixel shader texture
	uTextureCount													= zenMath::Max<zU16>( _Context.muPerStageTextureCount[zenConst::keShaderStage_Pixel], auPerStageTextureCount[zenConst::keShaderStage_Pixel]);	
	_Context.muPerStageTextureCount[zenConst::keShaderStage_Pixel]	= auPerStageTextureCount[zenConst::keShaderStage_Pixel];
	if( abTextureChanged[zenConst::keShaderStage_Pixel] )	
		DX11GetDeviceContext()->PSSetShaderResources( 0, uTextureCount, aStageTextureViews[zenConst::keShaderStage_Pixel] );
	if( abSamplerChanged[zenConst::keShaderStage_Pixel] )	
		DX11GetDeviceContext()->PSSetSamplers( 0, uTextureCount, aStageSamplerState[zenConst::keShaderStage_Pixel] );
	
	uTextureCount													= zenMath::Max<zU16>( _Context.muPerStageTextureCount[zenConst::keShaderStage_Vertex], auPerStageTextureCount[zenConst::keShaderStage_Vertex]);	
	if( abTextureChanged[zenConst::keShaderStage_Vertex] )	
		DX11GetDeviceContext()->VSSetShaderResources( 0, uTextureCount, aStageTextureViews[zenConst::keShaderStage_Vertex] );
	if( abSamplerChanged[zenConst::keShaderStage_Vertex] )	
		DX11GetDeviceContext()->VSSetSamplers( 0, uTextureCount, aStageSamplerState[zenConst::keShaderStage_Vertex] );

	mbTextureUnbound	= false;
	mbResourceUnbound	= false;
}

void ManagerRender::Render(zArrayDynamic<zenRes::zGfxDrawcall>& _aDrawcalls)
{
	//_aDrawcalls.Sort<>(); //! @todo urgent : Sort element before render
	RenderContext			Context;
	zenRes::zGfxDrawcall*	pDrawcall = _aDrawcalls.First();
	for(zUInt i(0), count(_aDrawcalls.Count()); i<count; ++i, ++pDrawcall)
	{	
		if( (*pDrawcall).IsValid() && (*pDrawcall)->mrRenderPass.IsValid() )
		{	
			// Render Commands other than Draw/Compute
			if( (*pDrawcall)->mSortId.muGPUPipelineMode == Drawcall::keGpuPipe_PreDrawCommand || 
				(*pDrawcall)->mSortId.muGPUPipelineMode == Drawcall::keGpuPipe_PostDrawCommand )
			{
				(*pDrawcall)->Invoke();					
			}
			else if( (*pDrawcall)->mrMeshStrip.IsValid() )
			{
				const zcRes::GfxMeshStripProxyRef& rMeshStrip = (*pDrawcall)->mrMeshStrip;
				UpdateGPUState(*pDrawcall, Context);
				UpdateShaderState(*pDrawcall, Context);
				mDX11pContextImmediate->DrawIndexed( rMeshStrip->muIndexCount, rMeshStrip->muIndexFirst, 0 );
			}
		}
	}
	mrPreviousDrawcall = *_aDrawcalls.Last();
}


void ManagerRender::UnbindTextures()
{
	if( mbTextureUnbound == false )
	{
		ID3D11ShaderResourceView* StageTextureViews[zcExp::kuDX11_TexturePerStageMax];
		zenMem::Set(StageTextureViews, 0, sizeof(StageTextureViews) );	
		DX11GetDeviceContext()->VSSetShaderResources( 0, zcExp::kuDX11_TexturePerStageMax, StageTextureViews );
		DX11GetDeviceContext()->PSSetShaderResources( 0, zcExp::kuDX11_TexturePerStageMax, StageTextureViews );
		mbTextureUnbound = true;
	}
}

void ManagerRender::UnbindResources()
{
	if( mbResourceUnbound == false )
	{
		UnbindTextures();
		//mDX11pContextImmediate->OMSetRenderTargets(0, nullptr, nullptr );
		//mbResourceUnbound = true;
	}

}

}