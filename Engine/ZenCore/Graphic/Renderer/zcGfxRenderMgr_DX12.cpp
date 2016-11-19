#include "zcCore.h"
#include <DXGI.h>
#include <DXGI1_4.h>

//#include <D3D12Shader.h>
//#include <D3Dcompiler.h>
namespace zcMgr { zcGfx::ManagerRender GfxRender; }

//SF DX12

namespace zcGfx
{

DX12QueryDisjoint::List		DX12QueryDisjoint::slstQueryCreated;
DX12QueryTimestamp::List	DX12QueryTimestamp::slstQueryCreated;

DX12QueryDisjoint::DX12QueryDisjoint()
{
#if !DISABLE_DX12
	D3D11_QUERY_DESC QueryDesc;
	QueryDesc.Query		= D3D11_QUERY_TIMESTAMP_DISJOINT;
	QueryDesc.MiscFlags	= 0;
	HRESULT result		= zcMgr::GfxRender.DX12GetDevice()->CreateQuery(&QueryDesc, &mpDX12Query);
	zenAssert(result == S_OK);
#endif
	slstQueryCreated.PushHead(*this);
}

void DX12QueryDisjoint::ReferenceDeleteCB()
{
	slstQueryCreated.PushHead(*this);
}

void DX12QueryDisjoint::Start()
{
#if !DISABLE_DX12
	muFrameStop				= zUInt(-1);
	mbValidResult			= false;
	mDisjointInfo.Disjoint	= true;
	zcMgr::GfxRender.DX12GetDeviceContext()->Begin(mpDX12Query);	
#endif
}

void DX12QueryDisjoint::Stop()
{
#if !DISABLE_DX12
	if( muFrameStop )
	{
		muFrameStop	= zcMgr::GfxRender.GetFrameRendered();
		zcMgr::GfxRender.DX12GetDeviceContext()->End(mpDX12Query);
	}
#endif
}

zU64 DX12QueryDisjoint::GetClockRate()
{
#if !DISABLE_DX12
	zenAssertMsg(muFrameStop != zUInt(-1), "Query need to be started and stopped before we get results back");
	zenAssertMsg(muFrameStop < zcMgr::GfxRender.GetFrameRendered(), "Must wait a complete frame before getting results");
	if( !mbValidResult ) 
	{		
		HRESULT result	= zcMgr::GfxRender.DX12GetDeviceContext()->GetData(mpDX12Query, &mDisjointInfo, sizeof(mDisjointInfo), 0);
		mbValidResult	= (result == S_OK);
	}
#endif
	return mDisjointInfo.Disjoint ? 0 : mDisjointInfo.Frequency;
}

zEngineRef<DX12QueryDisjoint> DX12QueryDisjoint::Create()
{	
	const zUInt uGrowSize = 8;
	if( slstQueryCreated.IsEmpty() )
	{
		for(zUInt idx(0); idx<uGrowSize; ++idx)
			zenNewDefault DX12QueryDisjoint();
	}
	return slstQueryCreated.PopTail();
}

DX12QueryTimestamp::DX12QueryTimestamp()
{
#if !DISABLE_DX12
	D3D11_QUERY_DESC QueryDesc;
	QueryDesc.Query		= D3D11_QUERY_TIMESTAMP;
	QueryDesc.MiscFlags	= 0;
	HRESULT result		= zcMgr::GfxRender.DX12GetDevice()->CreateQuery(&QueryDesc, &mpDX12Query);
	zenAssert(result == S_OK);
#endif
	slstQueryCreated.PushHead(*this);
}

void DX12QueryTimestamp::ReferenceDeleteCB()
{
	if( mrQueryDisjoint.IsValid() )
	{
		zU64 uDiscard = GetTimestampUSec(); // This prevents DX warning about starting a new query(when query reused), without having retrieved the value first
		mrQueryDisjoint	= nullptr;
		mbValidResult	= false;
	}
	slstQueryCreated.PushHead(*this);
}

zU64 DX12QueryTimestamp::GetTimestampUSec()
{	
#if !DISABLE_DX12
	if( mbValidResult == false )
	{
		zU64 uClockRate = mrQueryDisjoint->GetClockRate();		
		if( uClockRate != 0 )
		{
			zcMgr::GfxRender.DX12GetDeviceContext()->GetData(mpDX12Query, &muTimestamp, sizeof(muTimestamp), 0);
			muTimestamp		= uClockRate ? (muTimestamp*1000*1000/uClockRate) : 0;
			mbValidResult	= true;
		}
	}
#endif
	return muTimestamp;
}

zEngineRef<DX12QueryTimestamp> DX12QueryTimestamp::Create()
{	
#if !DISABLE_DX12
	const zUInt uGrowSize = 128;
	if( slstQueryCreated.IsEmpty() )
	{
		for(zUInt idx(0); idx<uGrowSize; ++idx)
			zenNewDefault DX12QueryTimestamp();
	}

	DX12QueryTimestamp* pQuery	= slstQueryCreated.PopTail();
	pQuery->mrQueryDisjoint		= zcMgr::GfxRender.GetQueryDisjoint();
	pQuery->mbValidResult		= false;
	pQuery->muTimestamp			= 0;
	zcMgr::GfxRender.DX12GetDeviceContext()->End(pQuery->mpDX12Query);
	return pQuery;
#else
	return slstQueryCreated.PopTail();
#endif
}

ManagerRender::RenderContext::RenderContext()
{
	zenMem::Zero(mahShaderInputStamp);	
	zenMem::Zero(maShaderInputSlotCount);
	zenMem::Zero(maResourceView);
	zenMem::Zero(maResourceType);
}

// Helper function for acquiring the first available hardware adapter that supports Direct3D 12.
// If no such adapter can be found, *ppAdapter will be set to nullptr.
void GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
{
	Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
	*ppAdapter = nullptr;

	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Don't select the Basic Render Driver adapter.
			// If you want a software adapter, pass in "/warp" on the command line.
			continue;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the
		// actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}

	*ppAdapter = adapter.Detach();
}

bool ManagerRender::Load()
{	
	HRESULT hr	= S_OK;
	
	//-------------------------------------------------------------------------
	// Configure Texture Format mapping
	//-------------------------------------------------------------------------
	zenMem::Set(meFormatConv,			DXGI_FORMAT_UNKNOWN, sizeof(meFormatConv) );	
	zenMem::Set(meFormatConvTypeless,	DXGI_FORMAT_UNKNOWN, sizeof(meFormatConvTypeless) );	
	zenMem::Set(meFormatConvDepthDSV,	DXGI_FORMAT_UNKNOWN, sizeof(meFormatConvDepthDSV) );	
	zenMem::Set(meFormatConvDepthSRV,	DXGI_FORMAT_UNKNOWN, sizeof(meFormatConvDepthSRV) );	
	zenMem::Set(meFormatConvStencilSRV, DXGI_FORMAT_UNKNOWN, sizeof(meFormatConvStencilSRV) );	
	
	// Surface formats
	meFormatConv[zenConst::keTexFormat_R8]				= DXGI_FORMAT_R8_UNORM;	
	meFormatConv[zenConst::keTexFormat_RGB8]			= DXGI_FORMAT_R10G10B10A2_UNORM;
	meFormatConv[zenConst::keTexFormat_RGBA8]			= DXGI_FORMAT_R8G8B8A8_UNORM;
	meFormatConv[zenConst::keTexFormat_D24S8]			= DXGI_FORMAT_D24_UNORM_S8_UINT;
	meFormatConv[zenConst::keTexFormat_D32]				= DXGI_FORMAT_D32_FLOAT;
	meFormatConv[zenConst::keTexFormat_BC1]				= DXGI_FORMAT_BC1_UNORM;	
	meFormatConv[zenConst::keTexFormat_BC2]				= DXGI_FORMAT_BC2_UNORM;
	meFormatConv[zenConst::keTexFormat_BC3]				= DXGI_FORMAT_BC3_UNORM;
	meFormatConv[zenConst::keTexFormat_BC5]				= DXGI_FORMAT_BC5_UNORM;
	meFormatConv[zenConst::keTexFormat_BC7]				= DXGI_FORMAT_BC7_UNORM;
	meFormatConv[zenConst::keTexFormat_RGBA32f]			= DXGI_FORMAT_R32_FLOAT;
	
	// Typeless format
	meFormatConvTypeless[zenConst::keTexFormat_R8]		= DXGI_FORMAT_R8_TYPELESS;	
	meFormatConvTypeless[zenConst::keTexFormat_RGB8]	= DXGI_FORMAT_UNKNOWN;
	meFormatConvTypeless[zenConst::keTexFormat_RGBA8]	= DXGI_FORMAT_R8G8B8A8_TYPELESS;
	meFormatConvTypeless[zenConst::keTexFormat_D24S8]	= DXGI_FORMAT_R24G8_TYPELESS;
	meFormatConvTypeless[zenConst::keTexFormat_D32]		= DXGI_FORMAT_R32_TYPELESS;
	meFormatConvTypeless[zenConst::keTexFormat_BC1]		= DXGI_FORMAT_BC1_TYPELESS;	
	meFormatConvTypeless[zenConst::keTexFormat_BC2]		= DXGI_FORMAT_BC2_TYPELESS;
	meFormatConvTypeless[zenConst::keTexFormat_BC3]		= DXGI_FORMAT_BC3_TYPELESS;
	meFormatConvTypeless[zenConst::keTexFormat_BC5]		= DXGI_FORMAT_BC5_TYPELESS;
	meFormatConvTypeless[zenConst::keTexFormat_BC7]		= DXGI_FORMAT_BC7_TYPELESS;
	meFormatConvTypeless[zenConst::keTexFormat_RGBA32f]	= DXGI_FORMAT_R32_TYPELESS;

	// Pending Format to add: DXGI_FORMAT_D32_FLOAT_S8X24_UINT, DXGI_FORMAT_D16_UNORM 
	meFormatConvDepthDSV[zenConst::keTexFormat_D24S8]	= DXGI_FORMAT_D24_UNORM_S8_UINT;
	meFormatConvDepthDSV[zenConst::keTexFormat_D32]		= DXGI_FORMAT_D32_FLOAT;
	// Pending Format to add: DXGI_FORMAT_R32G8X24_TYPELESS, DXGI_FORMAT_R16_UNORM
	meFormatConvDepthSRV[zenConst::keTexFormat_D24S8]	= DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	meFormatConvDepthSRV[zenConst::keTexFormat_D32]		= DXGI_FORMAT_R32_FLOAT;
	// Pending Format to add: DXGI_FORMAT_X32_TYPELESS_G8X24_UINT
	meFormatConvStencilSRV[zenConst::keTexFormat_D24S8]	= DXGI_FORMAT_X24_TYPELESS_G8_UINT;

	// Enable the D3D12 debug layer.
	if( _DEBUG )
	{
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&mrDXDebugController))))
		{
			mrDXDebugController->EnableDebugLayer();
			mrDXDebugController->SetEnableGPUBasedValidation(true);
		}
	}

	// Create DirectX GI Factory	
	hr = CreateDXGIFactory1(IID_PPV_ARGS(&mrDXFactory) );
	if( FAILED(hr) )
		return false;
	
	//SF Temp Find first valid GPU
	{
		Microsoft::WRL::ComPtr<IDXGIAdapter1> hardwareAdapter;
	#if 0
		GetHardwareAdapter(mrDXFactory.Get(), &hardwareAdapter);
	#else
		hr = mrDXFactory->EnumWarpAdapter(IID_PPV_ARGS(&hardwareAdapter));
		if( FAILED(hr) )
			return false;
		hr = hardwareAdapter.As(&mrDXAdapter);
		if( FAILED(hr) )
			return false;
	//	if(FAILED(mrDXFactory->EnumAdapters1(0, &hardwareAdapter)))
	//		throw "Failed to retrieve default adapter";

	#endif
	}

	// Create DirectX Device
	hr =  D3D12CreateDevice(mrDXAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mrDXDevice) );
	if( FAILED( hr ) )
		return FALSE;

#if !ZEN_BUILD_FINAL	
	mrDXDevice->SetStablePowerState(TRUE); // Prevent the GPU from overclocking or underclocking to get consistent timings
#endif

	// Create Descriptors heap		
	if( !DescriptorSRV::Initialize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE) )
		return FALSE;
	if( !DescriptorRTV::Initialize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) )
		return FALSE;
	if( !DescriptorDSV::Initialize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV) )
		return FALSE;

	//mCommandManager.Create(mrDXDevice.Get());
	hr = mrDXDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
	if( FAILED( hr ) )
		return FALSE;	
	
	// Create the command list.
	mrDXDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_commandList));

	// Command lists are created in the recording state, but there is nothing
	// to record yet. The main loop expects it to be closed, so close it now.
	m_commandList->Close();

	// Describe and create the command queue.
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	mrDXDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));


	// Create synchronization objects and wait until assets have been uploaded to the GPU.
	{
		mrDXDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
		m_fenceValue = 1;

		// Create an event handle to use for frame synchronization.
		m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
//		if (m_fenceEvent == nullptr);
		//{
		//	ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
		//}

		// Wait for the command list to execute; we are reusing the same command 
		// list in our main loop but for now, we just want to wait for setup to 
		// complete before continuing.
		WaitForPreviousFrame();
	}

		/*
#if !DISABLE_DX12
	D3D11_INPUT_ELEMENT_DESC EmptyDesc;
	hr = mDX12pContextImmediate->QueryInterface( __uuidof(mDX12pPerf), reinterpret_cast<void**>(&mDX12pPerf) );	
	if( FAILED( hr ) )
		return FALSE;
#endif*/
	return true;
}

bool ManagerRender::Unload()
{
#if !DISABLE_DX12
	if( mDX12pPerf ) 
		mDX12pPerf->Release();
#endif	
	//mrDXCmdQueueDirect.Reset(); //SF Clear/Release?
	mrDXDevice.Reset();
	mrDXFactory.Reset();
#if !DISABLE_DX12
	mDX12pPerf				= nullptr;
	mDX12pContextImmediate	= nullptr;
#endif
	
	return true;
}


 inline D3D12_RESOURCE_BARRIER Transition(
        _In_ ID3D12Resource* pResource,
        D3D12_RESOURCE_STATES stateBefore,
        D3D12_RESOURCE_STATES stateAfter,
        UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
        D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE)
    {
        D3D12_RESOURCE_BARRIER result;
        ZeroMemory(&result, sizeof(result));
        D3D12_RESOURCE_BARRIER &barrier = result;
        result.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        result.Flags = flags;
        barrier.Transition.pResource = pResource;
        barrier.Transition.StateBefore = stateBefore;
        barrier.Transition.StateAfter = stateAfter;
        barrier.Transition.Subresource = subresource;
        return result;
    }

 void ManagerRender::WaitForPreviousFrame()
{
	// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
	// This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
	// sample illustrates how to use fences for efficient resource usage and to
	// maximize GPU utilization.

	// Signal and increment the fence value.
	const UINT64 fence = m_fenceValue;
	m_commandQueue->Signal(m_fence.Get(), fence);
	m_fenceValue++;

	// Wait until the previous frame is finished.
	if (m_fence->GetCompletedValue() < fence)
	{
		m_fence->SetEventOnCompletion(fence, m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}

	//m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
}

void ManagerRender::FrameBegin(zcRes::GfxWindowRef _FrameWindow)
{
	WaitForPreviousFrame();
	Super::FrameBegin(_FrameWindow);
	zcGfx::Command::ResetCommandCount();	
	const zcRes::GfxTarget2DRef& rBackbuffer = grWindowRender->GetBackbuffer();

	// Command list allocators can only be reset when the associated 
	// command lists have finished execution on the GPU; apps should use 
	// fences to determine GPU execution progress.
	m_commandAllocator->Reset();

	// However, when ExecuteCommandList() is called on a particular command 
	// list, that command list can then be reset at any time and must be before re-recording.
	m_commandList->Reset(m_commandAllocator.Get(), nullptr);

	// Indicate that the back buffer will be used as a render target.
	D3D12_RESOURCE_BARRIER Barrier = Transition(rBackbuffer.HAL()->mrTextureResource.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_commandList->ResourceBarrier(1, &Barrier );

	//mCommandManager.CreateNewCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT, &mCommandList, &mCurrentAllocator);
	m_commandList->OMSetRenderTargets(1, &rBackbuffer.HAL()->mTargetColorView.GetHandle(), FALSE, nullptr);
	

#if !DISABLE_DX12
	mrPreviousDrawcall		= nullptr;
	mbDX12ProfilerDetected	= mDX12pPerf && mDX12pPerf->GetStatus();
	mrQueryDisjoint			= DX12QueryDisjoint::Create();
	mrQueryDisjoint->Start();
#endif
}

void ManagerRender::FrameEnd()
{	
	const zcRes::GfxTarget2DRef& rBackbuffer = grWindowRender->GetBackbuffer();

	//mrQueryDisjoint->Stop();		

	// Indicate that the back buffer will now be used to present.
	D3D12_RESOURCE_BARRIER Barrier = Transition(rBackbuffer.HAL()->mrTextureResource.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_commandList->ResourceBarrier(1, &Barrier);

	HRESULT hr = m_commandList->Close();
	if (FAILED(hr))
		return;
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(zenArrayCount(ppCommandLists), ppCommandLists);
	grWindowRender.HAL()->mrDXSwapChain->Present( 0, 0 );	
	
	/*
	DXGI_FRAME_STATISTICS FrameStatistics;
	hr = m_pDXGISwapChain->GetFrameStatistics(&FrameStatistics);

	if (FrameStatistics.PresentCount > m_PreviousPresentCount)
	{
		if (m_PreviousRefreshCount > 0 &&
			(FrameStatistics.PresentRefreshCount - m_PreviousRefreshCount) > (FrameStatistics.PresentCount - m_PreviousPresentCount))
		{
			++m_GlitchCount;
		}
	}
	m_PreviousPresentCount = FrameStatistics.PresentCount;
	m_PreviousRefreshCount = FrameStatistics.SyncRefreshCount;
	*/
	UnbindResources();
	Super::FrameEnd();
}

void ManagerRender::NamedEventBegin(const zStringHash32& zName)
{
#if !DISABLE_DX12
	if( mbDX12ProfilerDetected )
	{
		WCHAR zEventName[64];
		mbstowcs_s(nullptr, zEventName, zName.mzName, zenArrayCount(zEventName));
		mDX12pPerf->BeginEvent(zEventName);
	}
#endif
}

void ManagerRender::NamedEventEnd()
{
#if !DISABLE_DX12
	if( mbDX12ProfilerDetected )
		mDX12pPerf->EndEvent();
#endif
}

const zEngineRef<DX12QueryDisjoint>& ManagerRender::GetQueryDisjoint()const
{
	return mrQueryDisjoint;
}

void ManagerRender::UpdateGPUState(const zEngineRef<zcGfx::Command>& _rDrawcall, RenderContext& _Context)
{
#if !DISABLE_DX12
	if( _Context.mrRenderpass != _rDrawcall->mrRenderPass )
	{			
		_Context.mrRenderpass						= _rDrawcall->mrRenderPass;
		const zcRes::GfxRenderPassRef& rRenderpass	= _Context.mrRenderpass;

		if( _Context.mrStateRaster !=  rRenderpass.HAL()->mrStateRaster )
		{			
			_Context.mrStateRaster		= rRenderpass.HAL()->mrStateRaster;
			_Context.mbScreenScissorOn	= _Context.mrStateRaster.HAL()->mRasterizerDesc.ScissorEnable == TRUE;
			mDX12pContextImmediate->RSSetState(_Context.mrStateRaster.HAL()->mpRasterizerState);
		}
		if( _Context.mrStateBlend != rRenderpass.HAL()->mrStateBlend )
		{			
			_Context.mrStateBlend		= rRenderpass.HAL()->mrStateBlend;
			mDX12pContextImmediate->OMSetBlendState(	_Context.mrStateBlend.HAL()->mpBlendState, 
														_Context.mrStateBlend.HAL()->mafBlendFactor, 
														_Context.mrStateBlend.HAL()->muSampleMask );
		}
		if( _Context.mrStateDepthStencil != rRenderpass.HAL()->mrStateDepthStencil )
		{				
			_Context.mrStateDepthStencil= rRenderpass.HAL()->mrStateDepthStencil;
			mDX12pContextImmediate->OMSetDepthStencilState(	_Context.mrStateDepthStencil.HAL()->mpDepthStencilState, 
															_Context.mrStateDepthStencil.HAL()->muStencilValue);
		}
		if( _Context.mrStateView != rRenderpass.HAL()->mrStateView )
		{
			const zcRes::GfxViewRef& rNewView		= rRenderpass->mrStateView;
			const zcRes::GfxViewRef& rPreviousView	= _Context.mrStateView;
			UINT maxCount							= zenMath::Max( rNewView.HAL()->muColorCount, rPreviousView.IsValid() ? rPreviousView.HAL()->muColorCount : 0);
			_Context.mrStateView					= rNewView;
			
			zcMgr::GfxRender.UnbindTextures();
			mDX12pContextImmediate->OMSetRenderTargets(maxCount, rNewView.HAL()->mpColorViews, rNewView.HAL()->mpDepthView );
			mDX12pContextImmediate->RSSetViewports( 1, &rNewView.HAL()->mViewport );
		}
	}
#endif
}

//==================================================================================================
//! @details Compare current bound samplers with wanted one and update their binding if different
//==================================================================================================
void ManagerRender::UpdateShaderState_Samplers(const zcGfx::CommandDraw& _Drawcall, RenderContext& _Context, eShaderStage _eShaderStage )
{	
#if !DISABLE_DX12
	ID3D11SamplerState*	aResourceView[zcExp::kuDX12_SamplerPerStageMax];
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
		case keShaderStage_Vertex:	DX12GetDeviceContext()->VSSetSamplers( uChangedFirst, uChangedLast-uChangedFirst+1, &aResourceView[uChangedFirst] ); break;
		case keShaderStage_Pixel:	DX12GetDeviceContext()->PSSetSamplers( uChangedFirst, uChangedLast-uChangedFirst+1, &aResourceView[uChangedFirst] ); break;
		default: break;
		}
	}
#endif
}

//==================================================================================================
//! @details Compare current bound Constant Buffers with wanted one and update their binding if different
//==================================================================================================
void ManagerRender::UpdateShaderState_ConstantBuffers(const zcGfx::CommandDraw& _Drawcall, RenderContext& _Context, eShaderStage _eShaderStage )
{	
#if !DISABLE_DX12
	ID3D11Buffer* aResourceView[zcExp::kuDX12_CBufferPerStageMax];
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
			rResource.HAL()->Update( *DX12GetDeviceContext() ); 
	}

	// Send it to API
	_Context.maShaderInputSlotCount[_eShaderStage][kShaderRes] = uValidCount;
	if( uChangedLast >= uChangedFirst )
	{
		switch( _eShaderStage )
		{
			case keShaderStage_Vertex:	DX12GetDeviceContext()->VSSetConstantBuffers( uChangedFirst, uChangedLast-uChangedFirst+1, &aResourceView[uChangedFirst] );	break;
			case keShaderStage_Pixel:	DX12GetDeviceContext()->PSSetConstantBuffers( uChangedFirst, uChangedLast-uChangedFirst+1, &aResourceView[uChangedFirst] );	break;		
			default: break;
		}
	}
#endif
}

//==================================================================================================
//! @details Compare current bound textures with wanted one and update their binding if different
//==================================================================================================
void ManagerRender::UpdateShaderState_Textures(zU16& Out_ChangedFirst, zU16& Out_ChangedLast, const zcGfx::CommandDraw& _Drawcall, RenderContext& _Context, eShaderStage _eShaderStage)
{	
#if !DISABLE_DX12
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
#endif
}

//==================================================================================================
//! @details Compare current bound textures with wanted one and update their binding if different
//==================================================================================================
void ManagerRender::UpdateShaderState_StructBuffers(zU16& Out_ChangedFirst, zU16& Out_ChangedLast, const zcGfx::CommandDraw& _Drawcall, RenderContext& _Context, eShaderStage _eShaderStage)
{	
#if !DISABLE_DX12
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
#endif
}

//==================================================================================================
//! @details Check all GPU states, and update the one that differ from current Drawcall
//==================================================================================================
void ManagerRender::UpdateShaderState(const zcGfx::CommandDraw& _Drawcall, RenderContext& _Context)
{
#if !DISABLE_DX12
	UINT UnusedOffset = 0;
	const zcRes::GfxMeshStripRef&		rMeshStrip	= _Drawcall.mrMeshStrip;
	const zcRes::GfxIndexRef&			rIndex		= rMeshStrip.HAL()->mrIndexBuffer;
	const zcRes::GfxShaderBindingRef&	rShaderBind	= rMeshStrip.HAL()->mrShaderBinding;			
	const zcRes::GfxViewRef&			rView		= _Context.mrStateView;

	if( _Context.mePrimitiveType != rIndex.HAL()->mePrimitiveType )
	{
		_Context.mePrimitiveType = rIndex.HAL()->mePrimitiveType;
		mDX12pContextImmediate->IASetPrimitiveTopology( rIndex.HAL()->mePrimitiveType );
	}
	
	if( _Context.marShader[zenConst::keShaderStage_Vertex] != rShaderBind.HAL()->marShader[zenConst::keShaderStage_Vertex] )
	{
		_Context.marShader[zenConst::keShaderStage_Vertex]	= rShaderBind.HAL()->marShader[zenConst::keShaderStage_Vertex];
		zcRes::GfxShaderVertexRef rShaderVertex				= _Context.marShader[zenConst::keShaderStage_Vertex];
		mDX12pContextImmediate->VSSetShader( rShaderVertex.HAL()->mpVertexShader, nullptr, 0 );
	}
	
	if( _Context.marShader[zenConst::keShaderStage_Pixel]!= rShaderBind.HAL()->marShader[zenConst::keShaderStage_Pixel] )
	{
		_Context.marShader[zenConst::keShaderStage_Pixel]	= rShaderBind.HAL()->marShader[zenConst::keShaderStage_Pixel];
		zcRes::GfxShaderPixelRef rShaderPixel				= _Context.marShader[zenConst::keShaderStage_Pixel];
		mDX12pContextImmediate->PSSetShader( rShaderPixel.HAL()->mpPixelShader, nullptr, 0 );
	}
	
	if(_Context.mbScreenScissorOn || _Context.mvScreenScissor != _Drawcall.mvScreenScissor )
	{
		D3D11_RECT ScissorRect;
		_Context.mvScreenScissor	= _Drawcall.mvScreenScissor;		
		ScissorRect.left			= _Drawcall.mvScreenScissor.x;
		ScissorRect.top				= _Drawcall.mvScreenScissor.y;
		ScissorRect.right			= zenMath::Min<zU16>(_Drawcall.mvScreenScissor.z, (zU16)rView.HAL()->mViewport.Width);
		ScissorRect.bottom			= zenMath::Min<zU16>(_Drawcall.mvScreenScissor.w, (zU16)rView.HAL()->mViewport.Height);
		mDX12pContextImmediate->RSSetScissorRects(1, &ScissorRect);
	}
	
	mDX12pContextImmediate->IASetIndexBuffer	( rIndex.HAL()->mpIndiceBuffer, rIndex.HAL()->meIndiceFormat, 0 );	

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
			case keShaderStage_Vertex:	DX12GetDeviceContext()->VSSetShaderResources( uResChangeStart, uResChangeEnd-uResChangeStart+1, &_Context.maResourceView[stageIdx][uResChangeStart] );	break;
			case keShaderStage_Pixel:	DX12GetDeviceContext()->PSSetShaderResources( uResChangeStart, uResChangeEnd-uResChangeStart+1, &_Context.maResourceView[stageIdx][uResChangeStart] );	break;
			default: break;
			}
		}
	}
	
	mbTextureUnbind		= false;
	mbResourceUnbind	= false;
#endif
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
					#if !DISABLE_DX12
						mDX12pContextImmediate->DrawIndexed(pCommandDraw->muIndexCount, pCommandDraw->muIndexFirst, rMeshStrip.HAL()->muVertexFirst);
					#endif
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
		ID3D11ShaderResourceView* StageTextureViews[zcExp::kuDX12_TexturePerStageMax];
		zenMem::Set(StageTextureViews, 0, sizeof(StageTextureViews) );	
		DX12GetDeviceContext()->VSSetShaderResources( 0, zcExp::kuDX12_TexturePerStageMax, StageTextureViews );
		DX12GetDeviceContext()->PSSetShaderResources( 0, zcExp::kuDX12_TexturePerStageMax, StageTextureViews );
		mbTextureUnbind = true;
	}
}

void ManagerRender::UnbindResources()
{
	if( mbResourceUnbind == false )
	{
		UnbindTextures();
		//mDX12pContextImmediate->OMSetRenderTargets(0, nullptr, nullptr );
		//mbResourceUnbind = true;
	}

}
#endif
}