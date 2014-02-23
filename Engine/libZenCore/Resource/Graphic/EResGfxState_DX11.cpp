#include "libZenCore.h"

namespace ERes
{
	//////////////////////////////////////////////////////////////////////////
	//GfxSampler_DX11
	//////////////////////////////////////////////////////////////////////////
	GfxSampler_DX11::GfxSampler_DX11()
	: ERes::Resource()
	{		
		mInstanceInfo.mpSamplerState = NULL;
	}

	GfxSampler_DX11::~GfxSampler_DX11()
	{	
		if( mInstanceInfo.mpSamplerState )
			mInstanceInfo.mpSamplerState->Release();
	}

	bool GfxSampler_DX11::ResourceInit()
	{
		HRESULT hr = EMgr::GfxRender.DX11GetDevice()->CreateSamplerState( &mInstanceInfo.mSerial.mSamplerDesc, &mInstanceInfo.mpSamplerState );
		return SUCCEEDED(hr);
	}

	//////////////////////////////////////////////////////////////////////////
	//GfxBlend_DX11
	//////////////////////////////////////////////////////////////////////////
	GfxBlend_DX11::GfxBlend_DX11()
		: ERes::Resource()
	{		
		mInstanceInfo.mpBlendState = NULL;
	}

	GfxBlend_DX11::~GfxBlend_DX11()
	{	
		if( mInstanceInfo.mpBlendState )
			mInstanceInfo.mpBlendState->Release();
	}

	bool GfxBlend_DX11::ResourceInit()
	{
		HRESULT hr = EMgr::GfxRender.DX11GetDevice()->CreateBlendState( &mInstanceInfo.mSerial.mBlendDesc, &mInstanceInfo.mpBlendState );
		return SUCCEEDED(hr);
	}

	//////////////////////////////////////////////////////////////////////////
	//GfxDepthStencil_DX11
	//////////////////////////////////////////////////////////////////////////
	GfxDepthStencil_DX11::GfxDepthStencil_DX11()
		: ERes::Resource()
	{		
		mInstanceInfo.mpDepthStencilState = NULL;
	}

	GfxDepthStencil_DX11::~GfxDepthStencil_DX11()
	{	
		if( mInstanceInfo.mpDepthStencilState )
			mInstanceInfo.mpDepthStencilState->Release();
	}

	bool GfxDepthStencil_DX11::ResourceInit()
	{
		HRESULT hr = EMgr::GfxRender.DX11GetDevice()->CreateDepthStencilState( &mInstanceInfo.mSerial.mDepthStencilDesc, &mInstanceInfo.mpDepthStencilState );
		return SUCCEEDED(hr);
	}

	//////////////////////////////////////////////////////////////////////////
	//GfxRasterizer_DX11
	//////////////////////////////////////////////////////////////////////////
	GfxRasterizer_DX11::GfxRasterizer_DX11()
		: ERes::Resource()
	{		
		mInstanceInfo.mpRasterizerState = NULL;
	}

	GfxRasterizer_DX11::~GfxRasterizer_DX11()
	{	
		if( mInstanceInfo.mpRasterizerState )
			mInstanceInfo.mpRasterizerState->Release();
	}

	bool GfxRasterizer_DX11::ResourceInit()
	{
		HRESULT hr = EMgr::GfxRender.DX11GetDevice()->CreateRasterizerState( &mInstanceInfo.mSerial.mRasterizerDesc, &mInstanceInfo.mpRasterizerState );
		return SUCCEEDED(hr);
	}
}
