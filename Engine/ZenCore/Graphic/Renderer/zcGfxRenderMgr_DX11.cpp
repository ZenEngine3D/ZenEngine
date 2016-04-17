#include "zcCore.h"

namespace zcMgr { zcGfx::ManagerRender GfxRender; }

namespace zcGfx
{

zList<DX11QueryDisjoint, &DX11QueryDisjoint::mlstLink>	DX11QueryDisjoint::slstQueryCreated;
zList<DX11QueryTimestamp, &DX11QueryTimestamp::mlstLink> DX11QueryTimestamp::slstQueryCreated;

DX11QueryDisjoint::DX11QueryDisjoint()
{
	D3D11_QUERY_DESC QueryDesc;
	QueryDesc.Query		= D3D11_QUERY_TIMESTAMP_DISJOINT;
	QueryDesc.MiscFlags	= 0;
	HRESULT result		= zcMgr::GfxRender.DX11GetDevice()->CreateQuery(&QueryDesc, &mpDX11Query);
	ZENAssert(result == S_OK);
	slstQueryCreated.PushHead(*this);
}

void DX11QueryDisjoint::ReferenceNoneCB()
{
	slstQueryCreated.PushHead(*this);
}


void DX11QueryDisjoint::Start()
{
	muFrameStop				= zUInt(-1);
	mbValidResult			= false;
	mDisjointInfo.Disjoint	= true;
	zcMgr::GfxRender.DX11GetDeviceContext()->Begin(mpDX11Query);	
}

void DX11QueryDisjoint::Stop()
{
	if( muFrameStop )
	{
		muFrameStop	= zcMgr::GfxRender.GetFrameRendered();
		zcMgr::GfxRender.DX11GetDeviceContext()->End(mpDX11Query);
	}
}

zU64 DX11QueryDisjoint::GetClockRate()
{
	ZENAssertMsg(muFrameStop != zUInt(-1), "Query need to be started and stopped before we get results back");
	ZENAssertMsg(muFrameStop < zcMgr::GfxRender.GetFrameRendered(), "Must wait a complete frame before getting results");
	if( !mbValidResult ) 
	{		
		HRESULT result	= zcMgr::GfxRender.DX11GetDeviceContext()->GetData(mpDX11Query, &mDisjointInfo, sizeof(mDisjointInfo), 0);
		mbValidResult	= (result == S_OK);
	}
	return mDisjointInfo.Disjoint ? 0 : mDisjointInfo.Frequency;
}

zEngineRef<DX11QueryDisjoint> DX11QueryDisjoint::Create()
{	
	const zUInt uGrowSize = 8;
	if( slstQueryCreated.IsEmpty() )
	{
		for(zUInt idx(0); idx<uGrowSize; ++idx)
			zenNewDefault DX11QueryDisjoint();
	}
	return slstQueryCreated.PopTail();
}

DX11QueryTimestamp::DX11QueryTimestamp()
{
	D3D11_QUERY_DESC QueryDesc;
	QueryDesc.Query		= D3D11_QUERY_TIMESTAMP;
	QueryDesc.MiscFlags	= 0;
	HRESULT result		= zcMgr::GfxRender.DX11GetDevice()->CreateQuery(&QueryDesc, &mpDX11Query);
	ZENAssert(result == S_OK);
	slstQueryCreated.PushHead(*this);
}

void DX11QueryTimestamp::ReferenceNoneCB()
{
	if( mrQueryDisjoint.IsValid() )
	{
		zU64 uDiscard = GetTimestampUSec(); // This prevents DX warning about starting a new query(when query reused), without having retrieved the value first
		mrQueryDisjoint	= nullptr;
		mbValidResult	= false;
	}
	slstQueryCreated.PushHead(*this);
}

zU64 DX11QueryTimestamp::GetTimestampUSec()
{	
	if( mbValidResult == false )
	{
		zU64 uClockRate = mrQueryDisjoint->GetClockRate();		
		if( uClockRate != 0 )
		{
			zcMgr::GfxRender.DX11GetDeviceContext()->GetData(mpDX11Query, &muTimestamp, sizeof(muTimestamp), 0);
			muTimestamp		= uClockRate ? (muTimestamp*1000*1000/uClockRate) : 0;
			mbValidResult	= true;
		}
	}
	return muTimestamp;
}

zEngineRef<DX11QueryTimestamp> DX11QueryTimestamp::Create()
{	
	const zUInt uGrowSize = 128;
	if( slstQueryCreated.IsEmpty() )
	{
		for(zUInt idx(0); idx<uGrowSize; ++idx)
			zenNewDefault DX11QueryTimestamp();
	}

	DX11QueryTimestamp* pQuery	= slstQueryCreated.PopTail();
	pQuery->mrQueryDisjoint		= zcMgr::GfxRender.GetQueryDisjoint();
	pQuery->mbValidResult		= false;
	pQuery->muTimestamp			= 0;
	zcMgr::GfxRender.DX11GetDeviceContext()->End(pQuery->mpDX11Query);
	return pQuery;
}

ManagerRender::RenderContext::RenderContext()
{
	zenMem::Zero(muPerStageTextureCount, sizeof(muPerStageTextureCount));
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

	const zArrayStatic<D3D_FEATURE_LEVEL> aFeatureLevels = { D3D_FEATURE_LEVEL_11_0 };
	mDX11DriverType	= D3D_DRIVER_TYPE_HARDWARE;	//Only support hardware dx11 support

	hr =  D3D11CreateDevice(
		nullptr, 
		mDX11DriverType,	
		nullptr, 
		createDeviceFlags, 
		aFeatureLevels.First(),
		(UINT)aFeatureLevels.Count(),
		D3D11_SDK_VERSION, 
		&mDX11pDevice, 
		&mDX11FeatureLevel, 
		&mDX11pContextImmediate
		);

	if( FAILED( hr ) )
		return FALSE;

	hr = mDX11pContextImmediate->QueryInterface( __uuidof(mDX11pPerf), reinterpret_cast<void**>(&mDX11pPerf) );	
	if( FAILED( hr ) )
		return FALSE;

	return TRUE;
}

bool ManagerRender::Unload()
{
	if( mDX11pPerf ) 
		mDX11pPerf->Release();

	if( mDX11pContextImmediate )
	{	
		mDX11pContextImmediate->ClearState();
		mDX11pContextImmediate->Release();
	}
	if( mDX11pDevice )				
		mDX11pDevice->Release();

	mDX11pPerf				= nullptr;
	mDX11pContextImmediate	= nullptr;
	mDX11pDevice			= nullptr;
	
	return true;
}

void ManagerRender::FrameBegin(zcRes::GfxWindowRef _FrameWindow)
{
	Super::FrameBegin(_FrameWindow);
	zcGfx::Command::ResetCommandCount();
	mrPreviousDrawcall		= nullptr;
	mbDX11ProfilerDetected	= mDX11pPerf && mDX11pPerf->GetStatus();
	mrQueryDisjoint			= DX11QueryDisjoint::Create();
	mrQueryDisjoint->Start();
	
}

void ManagerRender::FrameEnd()
{	
	mrQueryDisjoint->Stop();		
	gWindowRender->GetProxy()->mDX11pSwapChain->Present( 0, 0 );	
	UnbindResources();
	Super::FrameEnd();
}

void ManagerRender::NamedEventBegin(const zStringHash32& zName)
{
	if( mbDX11ProfilerDetected )
	{
		WCHAR zEventName[64];
		mbstowcs_s(nullptr, zEventName, zName.mzName, ZENArrayCount(zEventName));
		mDX11pPerf->BeginEvent(zEventName);
	}
}

void ManagerRender::NamedEventEnd()
{
	if( mbDX11ProfilerDetected )
		mDX11pPerf->EndEvent();
}

const zEngineRef<DX11QueryDisjoint>& ManagerRender::GetQueryDisjoint()const
{
	return mrQueryDisjoint;
}

void ManagerRender::UpdateGPUState(const zEngineRef<zcGfx::Command>& _rDrawcall, RenderContext& _Context)
{
	if( _Context.mrRenderpass != _rDrawcall->mrRenderPass )
	{			
		zcRes::GfxRenderPassRef& rRenderpass	= _Context.mrRenderpass;
		_Context.mrRenderpass					= _rDrawcall->mrRenderPass;
		if( _Context.mrStateRaster != rRenderpass->GetProxy()->mrProxRasterState )
		{
			_Context.mrStateRaster		= rRenderpass->GetProxy()->mrProxRasterState;
			_Context.mbScreenScissorOn	= _Context.mrStateRaster->GetProxy()->mbScreenScissorOn;
			mDX11pContextImmediate->RSSetState(_Context.mrStateRaster->GetProxy()->mpRasterizerState);
		}
		if( _Context.mrStateBlend != rRenderpass->GetProxy()->mrProxBlendState )
		{
			_Context.mrStateBlend = rRenderpass->GetProxy()->mrProxBlendState;
			mDX11pContextImmediate->OMSetBlendState( _Context.mrStateBlend->GetProxy()->mpBlendState, _Context.mrStateBlend->GetProxy()->mafBlendFactor, _Context.mrStateBlend->GetProxy()->muSampleMask );
		}
		if( mbResourceUnbound || _Context.mrStateDepthStencil != rRenderpass->GetProxy()->mrProxDepthStencilState )
		{	
			_Context.mrStateDepthStencil = rRenderpass->GetProxy()->mrProxDepthStencilState;
			mDX11pContextImmediate->OMSetDepthStencilState(_Context.mrStateDepthStencil->GetProxy()->mpDepthStencilState, _Context.mrStateDepthStencil->GetProxy()->muStencilValue);
		}
		if( mbResourceUnbound || _Context.mrStateView != rRenderpass->GetProxy()->mrProxViewState )
		{
			UINT maxCount = zenMath::Max( _Context.mrStateView.IsValid() ? _Context.mrStateView->GetProxy()->muColorCount : 0, _Context.mrRenderpass->GetProxy()->mrProxViewState->GetProxy()->muColorCount);
			_Context.mrStateView = rRenderpass->GetProxy()->mrProxViewState;
			zcMgr::GfxRender.UnbindTextures();
			mDX11pContextImmediate->OMSetRenderTargets(maxCount, _Context.mrStateView->GetProxy()->mpColorViews, _Context.mrStateView->GetProxy()->mpDepthView );
			mDX11pContextImmediate->RSSetViewports( 1, &_Context.mrStateView->GetProxy()->mViewport );
		}
	}
}

void ManagerRender::UpdateShaderState(const zcGfx::CommandDraw& _Drawcall, RenderContext& _Context)
{
	UINT UnusedOffset = 0;
	const zcRes::GfxMeshStripRef& rMeshStrip		= _Drawcall.mrMeshStrip;
	const zcRes::GfxVertexRef& rVertex				= rMeshStrip->GetProxy()->mrInputStreamProxy->GetProxy()->mrVertexProxy;
	const zcRes::GfxIndexRef& rIndex				= rMeshStrip->GetProxy()->mrIndexBufferProxy;
	const zcRes::GfxShaderBindingRef rShaderBind	= rMeshStrip->GetProxy()->mrShaderBindingProxy;
	if( _Context.mrInputStream != rMeshStrip->GetProxy()->mrInputStreamProxy )
	{
		_Context.mrInputStream = rMeshStrip->GetProxy()->mrInputStreamProxy;
		mDX11pContextImmediate->IASetInputLayout( rMeshStrip->GetProxy()->mrInputStreamProxy->GetProxy()->mpInputLayout );
	}
	if( _Context.mePrimitiveType != rIndex->GetProxy()->mePrimitiveType )
	{
		_Context.mePrimitiveType = rIndex->GetProxy()->mePrimitiveType;
		mDX11pContextImmediate->IASetPrimitiveTopology( rIndex->GetProxy()->mePrimitiveType );
	}		
	if( _Context.mrShaderVertex != rShaderBind->GetProxy()->mrProxShaderVertex )
	{
		_Context.mrShaderVertex = rShaderBind->GetProxy()->mrProxShaderVertex;
		mDX11pContextImmediate->VSSetShader( _Context.mrShaderVertex->GetProxy()->mpVertexShader, nullptr, 0 );
	}
	if( _Context.mrShaderPixel != rShaderBind->GetProxy()->mrProxShaderPixel )
	{
		_Context.mrShaderPixel = rShaderBind->GetProxy()->mrProxShaderPixel;
		mDX11pContextImmediate->PSSetShader( _Context.mrShaderPixel->GetProxy()->mpPixelShader, nullptr, 0 );
	}
	if(_Context.mbScreenScissorOn && _Context.mvScreenScissor != _Drawcall.mvScreenScissor )
	{
		_Context.mvScreenScissor = _Drawcall.mvScreenScissor;
		D3D11_RECT ScissorRect;
		ScissorRect.left	= _Drawcall.mvScreenScissor.x;
		ScissorRect.top		= _Drawcall.mvScreenScissor.y;
		ScissorRect.right	= zenMath::Min<zU16>(_Drawcall.mvScreenScissor.z, (zU16)_Context.mrStateView->GetProxy()->mViewport.Width);
		ScissorRect.bottom	= zenMath::Min<zU16>(_Drawcall.mvScreenScissor.w, (zU16)_Context.mrStateView->GetProxy()->mViewport.Height);
		mDX11pContextImmediate->RSSetScissorRects(1, &ScissorRect);
	}
	
	//! @todo clean revise this
	for(zUInt bufferIdx=0, bufferCount(rMeshStrip->GetProxy()->marShaderParamProxy.Count()); bufferIdx<bufferCount; ++bufferIdx)
	{
		zU32 uShaderMask = rShaderBind->GetProxy()->maStagePerParamDef[bufferIdx];
		for(zUInt stageIdx=0; stageIdx<zenConst::keShaderStage__Count; ++stageIdx)
			if( uShaderMask & (1<<stageIdx) )
				rMeshStrip->GetProxy()->marShaderParamProxy[bufferIdx]->GetProxy()->Bind(static_cast<zenConst::eShaderStage>(stageIdx));
	}

	mDX11pContextImmediate->IASetIndexBuffer		( rIndex->GetProxy()->mpIndiceBuffer, rIndex->GetProxy()->meIndiceFormat, 0 );
	mDX11pContextImmediate->IASetVertexBuffers		( 0, 1, rVertex->GetProxy()->maStreamBuffer.First(), rVertex->GetProxy()->maStreamStride.First(), &UnusedOffset );

	//----------------------------------------------------------------------------
	// Assign texture/sampler input for each shader stage
	//----------------------------------------------------------------------------
	ID3D11ShaderResourceView*	aStageTextureViews[zenConst::keShaderStage__Count][zcExp::kuDX11_TexturePerStageMax];
	ID3D11SamplerState*			aStageSamplerState[zenConst::keShaderStage__Count][zcExp::kuDX11_TexturePerStageMax];
	bool						abSamplerChanged[zenConst::keShaderStage__Count];
	bool						abTextureChanged[zenConst::keShaderStage__Count];
	zU16						auPerStageTextureCount[zenConst::keShaderStage__Count];
	zUInt						stageCount(rMeshStrip->GetProxy()->marTextureProxy.Count());
	
	//! @todo Clean : avoid this loop for each drawcall?
	for(zUInt stageIdx(0); stageIdx<stageCount; ++stageIdx)
	{		
		zUInt textureCount					= rMeshStrip->GetProxy()->marTextureProxy[stageIdx].Count();
		auPerStageTextureCount[stageIdx]	= 0;
		abSamplerChanged[stageIdx]			= false;
		abTextureChanged[stageIdx]			= mbTextureUnbound;
		for( zU16 textureIdx(0); textureIdx<textureCount; ++textureIdx )
		{
			zcRes::GfxTexture2dRef rTexture	= rMeshStrip->GetProxy()->marTextureProxy[stageIdx][textureIdx];
			zcRes::GfxSamplerRef rSampler	= rMeshStrip->GetProxy()->marGfxSamplerProxy[stageIdx][textureIdx];
			if( rTexture.IsValid() && rSampler.IsValid() ) //! @todo Missing: remove test, and make sure we always have default object at worst
			{
				abSamplerChanged[stageIdx]						|= _Context.maCurrentSampler[stageIdx][textureIdx] != rSampler;
				abTextureChanged[stageIdx]						|= _Context.maCurrentTexture[stageIdx][textureIdx] != rTexture;
				_Context.maCurrentSampler[stageIdx][textureIdx]	= rSampler;
				_Context.maCurrentTexture[stageIdx][textureIdx]	= rTexture;
				aStageTextureViews[stageIdx][textureIdx]		= rTexture->GetProxy()->mpTextureView;
				aStageSamplerState[stageIdx][textureIdx]		= rSampler->GetProxy()->mpSamplerState;
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

void ManagerRender::Render(zArrayDynamic<zEngineRef<zcGfx::Command>>& _aDrawcalls)
{	
	if(_aDrawcalls.Count() )
	{
		//_aDrawcalls.Sort(); //! @todo urgent readd sorting
		RenderContext				Context;
		zEngineRef<zcGfx::Command>*	prDrawcall = _aDrawcalls.First();
		for(zUInt i(0), count(_aDrawcalls.Count()); i<count; ++i, ++prDrawcall)
		{	
			if( (*prDrawcall).IsValid() && (*prDrawcall)->mrRenderPass.IsValid() )
			{	
				// Render Commands other than Draw/Compute
				//! @todo Perf test compared to virtual method
				if( (*prDrawcall)->mbIsCommandDraw )
				{
					zcGfx::CommandDraw* pCommandDraw = static_cast<zcGfx::CommandDraw*>( (*prDrawcall).Get() );
					if( pCommandDraw->mrMeshStrip.IsValid() )
					{
						const zcRes::GfxMeshStripRef& rMeshStrip = pCommandDraw->mrMeshStrip;
						UpdateGPUState(*prDrawcall, Context);
						UpdateShaderState(*pCommandDraw, Context);
						mDX11pContextImmediate->DrawIndexed(pCommandDraw->muIndexCount, pCommandDraw->muIndexFirst, rMeshStrip->GetProxy()->muVertexFirst);
					}
				}
				// All other type of command use 'slower' virtual method 'invoke' instead of type casting
				else 
				{
					(*prDrawcall)->Invoke();					
				}
				
			}
		}
		mrPreviousDrawcall = *_aDrawcalls.Last();
	}
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