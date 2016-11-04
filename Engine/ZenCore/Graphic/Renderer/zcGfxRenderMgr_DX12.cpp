#include "zcCore.h"
#include <D3D11Shader.h>
#include <D3Dcompiler.h>
namespace zcMgr { zcGfx::ManagerRender GfxRender; }

namespace zcGfx
{

DX11QueryDisjoint::List		DX11QueryDisjoint::slstQueryCreated;
DX11QueryTimestamp::List	DX11QueryTimestamp::slstQueryCreated;

DX11QueryDisjoint::DX11QueryDisjoint()
{
	D3D11_QUERY_DESC QueryDesc;
	QueryDesc.Query		= D3D11_QUERY_TIMESTAMP_DISJOINT;
	QueryDesc.MiscFlags	= 0;
	HRESULT result		= zcMgr::GfxRender.DX11GetDevice()->CreateQuery(&QueryDesc, &mpDX11Query);
	zenAssert(result == S_OK);
	slstQueryCreated.PushHead(*this);
}

void DX11QueryDisjoint::ReferenceDeleteCB()
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
	zenAssertMsg(muFrameStop != zUInt(-1), "Query need to be started and stopped before we get results back");
	zenAssertMsg(muFrameStop < zcMgr::GfxRender.GetFrameRendered(), "Must wait a complete frame before getting results");
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
	zenAssert(result == S_OK);
	slstQueryCreated.PushHead(*this);
}

void DX11QueryTimestamp::ReferenceDeleteCB()
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
	zenMem::Zero(mahShaderInputStamp);	
	zenMem::Zero(maShaderInputSlotCount);
	zenMem::Zero(maResourceView);
	zenMem::Zero(maResourceType);
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

	D3D11_INPUT_ELEMENT_DESC EmptyDesc;
	hr = mDX11pContextImmediate->QueryInterface( __uuidof(mDX11pPerf), reinterpret_cast<void**>(&mDX11pPerf) );	
	if( FAILED( hr ) )
		return FALSE;

	//----------------------------------------------------------------------------------------------
	// Create an empty input layout since not using them.
	//----------------------------------------------------------------------------------------------
	// We're binding struct buffer directly instead, more generic and flexible, 
	// This has no impact on performances
	//! @todo Store empty shader blob instead?
	const char zEmptyVertexShaderCode[] = "void main(uint VertexId : SV_VertexID){}";
	ID3DBlob *pErrorBlob(nullptr), *pShaderCompiled(nullptr);
	if( SUCCEEDED(D3DCompile(zEmptyVertexShaderCode, zenArrayCount(zEmptyVertexShaderCode), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, &pShaderCompiled, &pErrorBlob) ) )
	{
		hr = mDX11pDevice->CreateInputLayout( &EmptyDesc, 0, pShaderCompiled->GetBufferPointer(), pShaderCompiled->GetBufferSize(), &mDX11pEmptyInputLayout );
	}
	else if( pErrorBlob )
	{
		printf("\nError Compiling shader inputlayout shader :\n%s\n", (char*)pErrorBlob->GetBufferPointer() );
	}
		
	if( pShaderCompiled )	
		pShaderCompiled->Release();
	if( pErrorBlob )
		pErrorBlob->Release();

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
	mDX11pContextImmediate->IASetInputLayout( mDX11pEmptyInputLayout );
	mrPreviousDrawcall		= nullptr;
	mbDX11ProfilerDetected	= mDX11pPerf && mDX11pPerf->GetStatus();
	mrQueryDisjoint			= DX11QueryDisjoint::Create();
	mrQueryDisjoint->Start();	
}

void ManagerRender::FrameEnd()
{	
	mrQueryDisjoint->Stop();		
	grWindowRender.HAL()->mpDX11SwapChain->Present( 0, 0 );	
	UnbindResources();
	Super::FrameEnd();
}

void ManagerRender::NamedEventBegin(const zStringHash32& zName)
{
	if( mbDX11ProfilerDetected )
	{
		WCHAR zEventName[64];
		mbstowcs_s(nullptr, zEventName, zName.mzName, zenArrayCount(zEventName));
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
		_Context.mrRenderpass						= _rDrawcall->mrRenderPass;
		const zcRes::GfxRenderPassRef& rRenderpass	= _Context.mrRenderpass;

		if( _Context.mrStateRaster !=  rRenderpass.HAL()->mrStateRaster )
		{			
			_Context.mrStateRaster		= rRenderpass.HAL()->mrStateRaster;
			_Context.mbScreenScissorOn	= _Context.mrStateRaster.HAL()->mRasterizerDesc.ScissorEnable == TRUE;
			mDX11pContextImmediate->RSSetState(_Context.mrStateRaster.HAL()->mpRasterizerState);
		}
		if( _Context.mrStateBlend != rRenderpass.HAL()->mrStateBlend )
		{			
			_Context.mrStateBlend		= rRenderpass.HAL()->mrStateBlend;
			mDX11pContextImmediate->OMSetBlendState(	_Context.mrStateBlend.HAL()->mpBlendState, 
														_Context.mrStateBlend.HAL()->mafBlendFactor, 
														_Context.mrStateBlend.HAL()->muSampleMask );
		}
		if( _Context.mrStateDepthStencil != rRenderpass.HAL()->mrStateDepthStencil )
		{				
			_Context.mrStateDepthStencil= rRenderpass.HAL()->mrStateDepthStencil;
			mDX11pContextImmediate->OMSetDepthStencilState(	_Context.mrStateDepthStencil.HAL()->mpDepthStencilState, 
															_Context.mrStateDepthStencil.HAL()->muStencilValue);
		}
		if( _Context.mrStateView != rRenderpass.HAL()->mrStateView )
		{
			const zcRes::GfxViewRef& rNewView		= rRenderpass->mrStateView;
			const zcRes::GfxViewRef& rPreviousView	= _Context.mrStateView;
			UINT maxCount							= zenMath::Max( rNewView.HAL()->muColorCount, rPreviousView.IsValid() ? rPreviousView.HAL()->muColorCount : 0);
			_Context.mrStateView					= rNewView;
			
			zcMgr::GfxRender.UnbindTextures();
			mDX11pContextImmediate->OMSetRenderTargets(maxCount, rNewView.HAL()->mpColorViews, rNewView.HAL()->mpDepthView );
			mDX11pContextImmediate->RSSetViewports( 1, &rNewView.HAL()->mViewport );
		}
	}
}

//==================================================================================================
//! @details Compare current bound samplers with wanted one and update their binding if different
//==================================================================================================
void ManagerRender::UpdateShaderState_Samplers(const zcGfx::CommandDraw& _Drawcall, RenderContext& _Context, eShaderStage _eShaderStage )
{	
	ID3D11SamplerState*	aResourceView[zcExp::kuDX11_SamplerPerStageMax];
	const eShaderResource kShaderRes							= keShaderRes_Sampler;	
	const zArrayStatic<zcRes::GfxShaderResourceRef>& arResource	= _Drawcall.mrMeshStrip.HAL()->marShaderResources[_eShaderStage][kShaderRes];
	
	// Retrieve samplers new slots assignment
	zU16 uChangedFirst(0xFFFF), uChangedLast(0), uValidCount(0), uResCount(static_cast<zU16>(arResource.Count()));
	zU16 uAssignCount = zenMath::Max(uResCount, _Context.maShaderInputSlotCount[_eShaderStage][kShaderRes]);
	for( zU16 slotIdx(0); slotIdx<uAssignCount; ++slotIdx )
	{
		_Context.marSampler[_eShaderStage][slotIdx]	= slotIdx < uResCount ? arResource[slotIdx] : nullptr;
		zcRes::GfxSamplerRef& rResource				= _Context.marSampler[_eShaderStage][slotIdx];
		ID3D11SamplerState* pDXView					= rResource.IsValid() ? rResource.HAL()->mpSamplerState	: nullptr;		
		uChangedFirst								= pDXView != aResourceView[slotIdx]	? zenMath::Min(uChangedFirst, slotIdx)	: uChangedFirst;
		uChangedLast								= pDXView != aResourceView[slotIdx]	? slotIdx								: uChangedLast;
		uValidCount									= pDXView							? slotIdx+1								: uValidCount;
		aResourceView[slotIdx]						= pDXView;
	}

	// Send it to API
	_Context.maShaderInputSlotCount[_eShaderStage][kShaderRes] = uValidCount;
	if( uChangedLast >= uChangedFirst )
	{
		switch( _eShaderStage )
		{
		case keShaderStage_Vertex:	DX11GetDeviceContext()->VSSetSamplers( uChangedFirst, uChangedLast-uChangedFirst+1, &aResourceView[uChangedFirst] ); break;
		case keShaderStage_Pixel:	DX11GetDeviceContext()->PSSetSamplers( uChangedFirst, uChangedLast-uChangedFirst+1, &aResourceView[uChangedFirst] ); break;
		default: break;
		}
	}
}

//==================================================================================================
//! @details Compare current bound Constant Buffers with wanted one and update their binding if different
//==================================================================================================
void ManagerRender::UpdateShaderState_ConstantBuffers(const zcGfx::CommandDraw& _Drawcall, RenderContext& _Context, eShaderStage _eShaderStage )
{	
	ID3D11Buffer* aResourceView[zcExp::kuDX11_CBufferPerStageMax];
	const eShaderResource kShaderRes							= keShaderRes_CBuffer;	
	const zArrayStatic<zcRes::GfxShaderResourceRef>& arResource	= _Drawcall.mrMeshStrip.HAL()->marShaderResources[_eShaderStage][kShaderRes];
	
	// Retrieve samplers new slots assignment
	zU16 uChangedFirst(0xFFFF), uChangedLast(0), uValidCount(0), uSlotCount(static_cast<zU16>(arResource.Count()));
	zU16 uAssignCount = zenMath::Max(uSlotCount, _Context.maShaderInputSlotCount[_eShaderStage][kShaderRes]);
	for( zU16 slotIdx(0); slotIdx<uAssignCount; ++slotIdx )
	{
		_Context.marCBuffer[_eShaderStage][slotIdx]	= slotIdx < uSlotCount ? arResource[slotIdx] : nullptr;
		zcRes::GfxCBufferRef& rResource				= _Context.marCBuffer[_eShaderStage][slotIdx];
		ID3D11Buffer* pDXView						= rResource.IsValid() ? rResource.HAL()->mpBufferBinding : nullptr;		
		uChangedFirst								= pDXView != aResourceView[slotIdx]	? zenMath::Min(uChangedFirst, slotIdx)	: uChangedFirst;
		uChangedLast								= pDXView != aResourceView[slotIdx]	? slotIdx								: uChangedLast;
		uValidCount									= pDXView							? slotIdx+1								: uValidCount;
		aResourceView[slotIdx]						= pDXView;
		
		//Must update CBuffer constant value if updated
		if( rResource.IsValid() )
			rResource.HAL()->Update( *DX11GetDeviceContext() ); 
	}

	// Send it to API
	_Context.maShaderInputSlotCount[_eShaderStage][kShaderRes] = uValidCount;
	if( uChangedLast >= uChangedFirst )
	{
		switch( _eShaderStage )
		{
			case keShaderStage_Vertex:	DX11GetDeviceContext()->VSSetConstantBuffers( uChangedFirst, uChangedLast-uChangedFirst+1, &aResourceView[uChangedFirst] );	break;
			case keShaderStage_Pixel:	DX11GetDeviceContext()->PSSetConstantBuffers( uChangedFirst, uChangedLast-uChangedFirst+1, &aResourceView[uChangedFirst] );	break;		
			default: break;
		}
	}
}

//==================================================================================================
//! @details Compare current bound textures with wanted one and update their binding if different
//==================================================================================================
void ManagerRender::UpdateShaderState_Textures(zU16& Out_ChangedFirst, zU16& Out_ChangedLast, const zcGfx::CommandDraw& _Drawcall, RenderContext& _Context, eShaderStage _eShaderStage)
{				
	const eShaderResource kShaderRes							= keShaderRes_Texture;
	const zArrayStatic<zcRes::GfxShaderResourceRef>& arResource = _Drawcall.mrMeshStrip.HAL()->marShaderResources[_eShaderStage][kShaderRes];
	zU16 uValidCount											= 0;
	const zU16 uSlotCount										= static_cast<zU16>(arResource.Count());
	const zU16 uAssignCount										= zenMath::Max(uSlotCount, _Context.maShaderInputSlotCount[_eShaderStage][kShaderRes]);	
	for( zU16 slotIdx(0); slotIdx<uAssignCount; ++slotIdx )
	{
		zcRes::GfxTexture2dRef rResource					= slotIdx < uSlotCount	? arResource[slotIdx]			: nullptr;
		ID3D11ShaderResourceView* pDXView					= rResource.IsValid()	? rResource.HAL()->mpTextureView : nullptr;
		if( pDXView || _Context.maResourceType[_eShaderStage][slotIdx] == kShaderRes )
		{
			Out_ChangedFirst								= pDXView && pDXView != _Context.maResourceView[_eShaderStage][slotIdx] ? zenMath::Min(Out_ChangedFirst, slotIdx)	: Out_ChangedFirst;
			Out_ChangedLast									= pDXView && pDXView != _Context.maResourceView[_eShaderStage][slotIdx] ? zenMath::Max(Out_ChangedLast, slotIdx)	: Out_ChangedLast;
			uValidCount										= pDXView	? slotIdx+1	: uValidCount;
			_Context.maResourceType[_eShaderStage][slotIdx]	= pDXView	? kShaderRes : keShaderRes__Invalid;
			_Context.maResourceView[_eShaderStage][slotIdx]	= pDXView;			
			_Context.marResource[_eShaderStage][slotIdx]	= rResource;
		}		
	}
	_Context.maShaderInputSlotCount[_eShaderStage][kShaderRes] = uValidCount;
}

//==================================================================================================
//! @details Compare current bound textures with wanted one and update their binding if different
//==================================================================================================
void ManagerRender::UpdateShaderState_StructBuffers(zU16& Out_ChangedFirst, zU16& Out_ChangedLast, const zcGfx::CommandDraw& _Drawcall, RenderContext& _Context, eShaderStage _eShaderStage)
{	
	const eShaderResource kShaderRes							= keShaderRes_Buffer;
	const zArrayStatic<zcRes::GfxShaderResourceRef>& arResource = _Drawcall.mrMeshStrip.HAL()->marShaderResources[_eShaderStage][kShaderRes];
	zU16 uValidCount											= 0;
	const zU16 uSlotCount										= static_cast<zU16>(arResource.Count());
	const zU16 uAssignCount										= zenMath::Max(uSlotCount, _Context.maShaderInputSlotCount[_eShaderStage][kShaderRes]);
	
	for( zU16 slotIdx(0); slotIdx<uAssignCount; ++slotIdx )
	{
		zcRes::GfxBufferRef rResource						= slotIdx < uSlotCount	? arResource[slotIdx]		: nullptr;
		ID3D11ShaderResourceView* pDXView					= rResource.IsValid()	? rResource.HAL()->mpSRV	: nullptr;
		if( pDXView || _Context.maResourceType[_eShaderStage][slotIdx] == kShaderRes )
		{
			Out_ChangedFirst								= pDXView && pDXView != _Context.maResourceView[_eShaderStage][slotIdx] ? zenMath::Min(Out_ChangedFirst, slotIdx)	: Out_ChangedFirst;
			Out_ChangedLast									= pDXView && pDXView != _Context.maResourceView[_eShaderStage][slotIdx] ? zenMath::Max(Out_ChangedLast, slotIdx)	: Out_ChangedLast;
			uValidCount										= pDXView	? slotIdx+1	: uValidCount;
			_Context.maResourceType[_eShaderStage][slotIdx]	= pDXView	? kShaderRes : keShaderRes__Invalid;
			_Context.maResourceView[_eShaderStage][slotIdx]	= pDXView;			
			_Context.marResource[_eShaderStage][slotIdx]	= rResource;
		}		
	}
	_Context.maShaderInputSlotCount[_eShaderStage][kShaderRes] = uValidCount;
}

//==================================================================================================
//! @details Check all GPU states, and update the one that differ from current Drawcall
//==================================================================================================
void ManagerRender::UpdateShaderState(const zcGfx::CommandDraw& _Drawcall, RenderContext& _Context)
{
	UINT UnusedOffset = 0;
	const zcRes::GfxMeshStripRef&		rMeshStrip	= _Drawcall.mrMeshStrip;
	const zcRes::GfxIndexRef&			rIndex		= rMeshStrip.HAL()->mrIndexBuffer;
	const zcRes::GfxShaderBindingRef&	rShaderBind	= rMeshStrip.HAL()->mrShaderBinding;			
	const zcRes::GfxViewRef&			rView		= _Context.mrStateView;

	if( _Context.mePrimitiveType != rIndex.HAL()->mePrimitiveType )
	{
		_Context.mePrimitiveType = rIndex.HAL()->mePrimitiveType;
		mDX11pContextImmediate->IASetPrimitiveTopology( rIndex.HAL()->mePrimitiveType );
	}
	
	if( _Context.marShader[zenConst::keShaderStage_Vertex] != rShaderBind.HAL()->marShader[zenConst::keShaderStage_Vertex] )
	{
		_Context.marShader[zenConst::keShaderStage_Vertex]	= rShaderBind.HAL()->marShader[zenConst::keShaderStage_Vertex];
		zcRes::GfxShaderVertexRef rShaderVertex				= _Context.marShader[zenConst::keShaderStage_Vertex];
		mDX11pContextImmediate->VSSetShader( rShaderVertex.HAL()->mpVertexShader, nullptr, 0 );
	}
	
	if( _Context.marShader[zenConst::keShaderStage_Pixel]!= rShaderBind.HAL()->marShader[zenConst::keShaderStage_Pixel] )
	{
		_Context.marShader[zenConst::keShaderStage_Pixel]	= rShaderBind.HAL()->marShader[zenConst::keShaderStage_Pixel];
		zcRes::GfxShaderPixelRef rShaderPixel				= _Context.marShader[zenConst::keShaderStage_Pixel];
		mDX11pContextImmediate->PSSetShader( rShaderPixel.HAL()->mpPixelShader, nullptr, 0 );
	}
	
	if(_Context.mbScreenScissorOn || _Context.mvScreenScissor != _Drawcall.mvScreenScissor )
	{
		D3D11_RECT ScissorRect;
		_Context.mvScreenScissor	= _Drawcall.mvScreenScissor;		
		ScissorRect.left			= _Drawcall.mvScreenScissor.x;
		ScissorRect.top				= _Drawcall.mvScreenScissor.y;
		ScissorRect.right			= zenMath::Min<zU16>(_Drawcall.mvScreenScissor.z, (zU16)rView.HAL()->mViewport.Width);
		ScissorRect.bottom			= zenMath::Min<zU16>(_Drawcall.mvScreenScissor.w, (zU16)rView.HAL()->mViewport.Height);
		mDX11pContextImmediate->RSSetScissorRects(1, &ScissorRect);
	}
	
	mDX11pContextImmediate->IASetIndexBuffer	( rIndex.HAL()->mpIndiceBuffer, rIndex.HAL()->meIndiceFormat, 0 );	

	//----------------------------------------------------------------------------------------------
	// Assign Shader input resources for each stage
	//----------------------------------------------------------------------------------------------
	for(zUInt stageIdx(0); stageIdx<keShaderStage__Count; ++stageIdx)
	{						
		// Check resource setup stamp to detect if there was a change
		const eShaderStage eStageIdx = (eShaderStage)stageIdx;
		bool bUpdated[keShaderRes__Count];
		for(zUInt resTypeIdx(0);resTypeIdx<keShaderRes__Count; ++resTypeIdx)
		{
			const zArrayStatic<zcRes::GfxShaderResourceRef>& arResources	= rMeshStrip.HAL()->marShaderResources[stageIdx][resTypeIdx];
			zHash32 zMeshStripResStamp										= rMeshStrip.HAL()->mhShaderResourceStamp[stageIdx][resTypeIdx];
			if( (zU32)zMeshStripResStamp == 0 )
			{
				zMeshStripResStamp = zHash32();
				for(zUInt resIdx(0), resCount(arResources.Count()); resIdx<resCount; ++resIdx)
					zMeshStripResStamp.Append( arResources[resIdx].IsValid() ? (void*)&arResources[resIdx]->mResID : (void*)&zResID(), sizeof(zResID) );
				rMeshStrip.HAL()->mhShaderResourceStamp[stageIdx][resTypeIdx] = zMeshStripResStamp;
			}

			bUpdated[resTypeIdx]								= _Context.mahShaderInputStamp[stageIdx][resTypeIdx] != zMeshStripResStamp;
			_Context.mahShaderInputStamp[stageIdx][resTypeIdx]	= zMeshStripResStamp;
		}
		
		// Special case shader resources update
		if( bUpdated[keShaderRes_Sampler] )
			UpdateShaderState_Samplers( _Drawcall, _Context, eStageIdx);
		if( bUpdated[keShaderRes_CBuffer] )
			UpdateShaderState_ConstantBuffers( _Drawcall, _Context, eStageIdx);
		
		// Generic shader resources update
		zU16 uResChangeStart(0xFFFF), uResChangeEnd(0);
		if( bUpdated[keShaderRes_Buffer] )
			UpdateShaderState_StructBuffers( uResChangeStart, uResChangeEnd, _Drawcall, _Context, eStageIdx);
		if( bUpdated[keShaderRes_Texture] )
			UpdateShaderState_Textures( uResChangeStart, uResChangeEnd, _Drawcall, _Context, eStageIdx);
		if( uResChangeEnd >= uResChangeStart )
		{
			switch( eStageIdx )
			{
			case keShaderStage_Vertex:	DX11GetDeviceContext()->VSSetShaderResources( uResChangeStart, uResChangeEnd-uResChangeStart+1, &_Context.maResourceView[stageIdx][uResChangeStart] );	break;
			case keShaderStage_Pixel:	DX11GetDeviceContext()->PSSetShaderResources( uResChangeStart, uResChangeEnd-uResChangeStart+1, &_Context.maResourceView[stageIdx][uResChangeStart] );	break;
			default: break;
			}
		}
	}
	
	mbTextureUnbind		= false;
	mbResourceUnbind	= false;
}

void ManagerRender::Render(zArrayDynamic<zEngineRef<zcGfx::Command>>& _aDrawcalls)
{	
	if(_aDrawcalls.Count() )
	{
		//_aDrawcalls.Sort(); //! @todo urgent re-add sorting
		RenderContext Context;
		zEngineRef<zcGfx::Command>*	prDrawcall = _aDrawcalls.First();
		for(zUInt i(0), count(_aDrawcalls.Count()); i<count; ++i, ++prDrawcall)
		{	
			if( (*prDrawcall).IsValid()/* && (*prDrawcall)->mrRenderPass.IsValid()*/ )
			{	
				// Render Commands other than Draw/Compute
				//! @todo Perf test compared to virtual method
				if( (*prDrawcall)->mbIsCommandDraw )
				{
					zcGfx::CommandDraw* pCommandDraw = static_cast<zcGfx::CommandDraw*>( (*prDrawcall).Get() );
					if( pCommandDraw->mrMeshStrip.IsValid() )
					{
						zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_DrawIndexed);
						const zcRes::GfxMeshStripRef& rMeshStrip = pCommandDraw->mrMeshStrip;
						UpdateGPUState(*prDrawcall, Context);
						UpdateShaderState(*pCommandDraw, Context);
						mDX11pContextImmediate->DrawIndexed(pCommandDraw->muIndexCount, pCommandDraw->muIndexFirst, rMeshStrip.HAL()->muVertexFirst);
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

#if 0 //SHADERCONST
//! @todo urgent clean this up
void ManagerRender::UnbindTextures()
{
	if( mbTextureUnbind == false )
	{
		ID3D11ShaderResourceView* StageTextureViews[zcExp::kuDX11_TexturePerStageMax];
		zenMem::Set(StageTextureViews, 0, sizeof(StageTextureViews) );	
		DX11GetDeviceContext()->VSSetShaderResources( 0, zcExp::kuDX11_TexturePerStageMax, StageTextureViews );
		DX11GetDeviceContext()->PSSetShaderResources( 0, zcExp::kuDX11_TexturePerStageMax, StageTextureViews );
		mbTextureUnbind = true;
	}
}

void ManagerRender::UnbindResources()
{
	if( mbResourceUnbind == false )
	{
		UnbindTextures();
		//mDX11pContextImmediate->OMSetRenderTargets(0, nullptr, nullptr );
		//mbResourceUnbind = true;
	}

}
#endif
}