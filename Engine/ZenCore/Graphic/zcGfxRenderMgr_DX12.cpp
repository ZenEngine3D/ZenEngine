#include "zcCore.h"
#include <DXGI.h>
#include <DXGI1_4.h>

//SF DX12

//==============================================================================================
//! Temp texture creation
//! @todo 0 remove this once texture support added
//==============================================================================================
// Generate a simple black and white checkerboard texture.
static const UINT TextureWidth = 256;
static const UINT TextureHeight = 256;
static const UINT TexturePixelSize = 4;	// The number of bytes used to represent a pixel in the texture.
std::vector<UINT8> GenerateTextureData()
{
	const UINT rowPitch = TextureWidth * TexturePixelSize;
	const UINT cellPitch = rowPitch >> 3;		// The width of a cell in the checkboard texture.
	const UINT cellHeight = TextureWidth >> 3;	// The height of a cell in the checkerboard texture.
	const UINT textureSize = rowPitch * TextureHeight;

	std::vector<UINT8> data(textureSize);
	UINT8* pData = &data[0];

	for (UINT n = 0; n < textureSize; n += TexturePixelSize)
	{
		UINT x = n % rowPitch;
		UINT y = n / rowPitch;
		UINT i = x / cellPitch;
		UINT j = y / cellHeight;

		if (i % 2 == j % 2)
		{
			pData[n] = 0x00;		// R
			pData[n + 1] = 0x00;	// G
			pData[n + 2] = 0x00;	// B
			pData[n + 3] = 0xff;	// A
		}
		else
		{
			pData[n] = 0xff;		// R
			pData[n + 1] = 0xff;	// G
			pData[n + 2] = 0xff;	// B
			pData[n + 3] = 0xff;	// A
		}
	}

	return data;
}

namespace zcGfx
{

DX12QueryDisjoint::List		DX12QueryDisjoint::slstQueryCreated;
DX12QueryTimestamp::List	DX12QueryTimestamp::slstQueryCreated;

DX12QueryDisjoint::DX12QueryDisjoint()
{
#if !ZEN_RENDERER_DX12
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
#if !ZEN_RENDERER_DX12
	muFrameStop				= zUInt(-1);
	mbValidResult			= false;
	mDisjointInfo.Disjoint	= true;
	zcMgr::GfxRender.DX12GetDeviceContext()->Begin(mpDX12Query);	
#endif
}

void DX12QueryDisjoint::Stop()
{
#if !ZEN_RENDERER_DX12
	if( muFrameStop )
	{
		muFrameStop	= zcMgr::GfxRender.GetFrameRendered();
		zcMgr::GfxRender.DX12GetDeviceContext()->End(mpDX12Query);
	}
#endif
}

zU64 DX12QueryDisjoint::GetClockRate()
{
#if !ZEN_RENDERER_DX12
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
#if !ZEN_RENDERER_DX12
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
#if !ZEN_RENDERER_DX12
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
#if !ZEN_RENDERER_DX12
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

bool ManagerRender_DX12::Load()
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

	PSO_DX12::StaticInitialize();

	//----------------------------------------------------------------------------------------------
	// Create DirectX Device
	//----------------------------------------------------------------------------------------------
	UINT uDxgiFactoryFlags = 0;

	// Enable the D3D12 debug layer.
#if _DEBUG
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&mrDXDebugController))))
	{
		mrDXDebugController->EnableDebugLayer();
		mrDXDebugController->SetEnableGPUBasedValidation(true);
		uDxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
	}	
#endif

	// Create DirectX GI Factory	
	hr = CreateDXGIFactory2(uDxgiFactoryFlags, IID_PPV_ARGS(&mrDXFactory) );
	if( FAILED(hr) )
		return false;
	
	//SF Temp Find first valid GPU
	{
		Microsoft::WRL::ComPtr<IDXGIAdapter1> hardwareAdapter;
	#if 1
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
	
	hr =  D3D12CreateDevice(mrDXAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mrDXDevice) );
	if( FAILED( hr ) )
		return FALSE;

#if !ZEN_BUILD_FINAL	
	mrDXDevice->SetStablePowerState(TRUE); // Prevent the GPU from overclocking or underclocking to get consistent timings
#endif
	//----------------------------------------------------------------------------------------------
	// Create Descriptors heap		
	//----------------------------------------------------------------------------------------------
	if( !DescriptorSRV_UAV_CBV::Initialize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE) )
		return false;
	if( !DescriptorRTV::Initialize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) )
		return false;
	if( !DescriptorDSV::Initialize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV) )
		return false;
	
	//----------------------------------------------------------------------------------------------
	// Create Commandlist/Queue
	//----------------------------------------------------------------------------------------------
	//mCommandManager.Create(mrDXDevice.Get());
	hr = mrDXDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mrCommandAllocator));
	if( FAILED( hr ) )
		return FALSE;	
	
	// Create the command list.
	DirectXComRef<ID3D12GraphicsCommandList>* pCmdList = &marCommandList[0][0];

	
	for( zUInt idxCmdList(0); idxCmdList< sizeof(marCommandList) / sizeof(DirectXComRef<ID3D12GraphicsCommandList>); ++idxCmdList )
	{
		DirectXComRef<ID3D12GraphicsCommandList> rCmdList;
		mrDXDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mrCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&rCmdList));
		wchar_t zName[64];
		swprintf_s(zName, L"CommandList %02i", static_cast<int>(idxCmdList) );
		rCmdList->SetName( zName );		
		rCmdList->Close(); // Command lists are created in the recording state but main loop expects it to be closed
		pCmdList[idxCmdList] = rCmdList;
	}

	// Describe and create the command queue.
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	mrDXDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mrCommandQueue));

	//----------------------------------------------------------------------------------------------
	// Create a root signature 
	//----------------------------------------------------------------------------------------------
	{
		RootSignature::StaticInitialize();

		CD3DX12_DESCRIPTOR_RANGE1 SRVTable;
		SRVTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
		mRootSignatureDefault = RootSignature({RootSignature::SlotTable(1,	&SRVTable,	D3D12_SHADER_VISIBILITY_PIXEL)}, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	}

	//==============================================================================================
	//! Temp PSO creation
	//! @todo 0 replace this with proper PSO support 
	//==============================================================================================
	// Command list allocators can only be reset when the associated 
	// command lists have finished execution on the GPU; apps should use 
	// fences to determine GPU execution progress.
	mrCommandAllocator->Reset();

	// However, when ExecuteCommandList() is called on a particular command 
	// list, that command list can then be reset at any time and must be before re-recording.
	marCommandList[0][0]->Reset(mrCommandAllocator.Get(), nullptr);

	mrTmpShaderVS	= zenRes::zGfxShaderVertex::Create( "Shader/DX12Sample.sl", "VSMain");
	mrTmpShaderPS	= zenRes::zGfxShaderPixel::Create( "Shader/DX12Sample.sl", "PSMain" );

	// Define the vertex input layout.
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	// Describe and create the graphics pipeline state object (PSO).
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = mRootSignatureDefault.Get();
	psoDesc.VS = mrTmpShaderVS.HAL()->mDXShaderCode;
	psoDesc.PS = mrTmpShaderPS.HAL()->mDXShaderCode;
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	hr = mrDXDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mTmpPipelineState));
	if( FAILED(hr) )
		return false;

	//==============================================================================================
	//! Temp VertexBuffer creation
	//! @todo 0 replace this with proper Buffer support 
	//==============================================================================================
	// Create the vertex buffer.
	{
		float m_aspectRatio = 1280.f / 800.f;
		// Define the geometry for a triangle.
		Vertex triangleVertices[] =
		{
			{ { 0.0f, 0.25f * m_aspectRatio, 0.0f }, { 0.5f, 0.0f } },
			{ { 0.25f, -0.25f * m_aspectRatio, 0.0f }, { 1.0f, 1.0f } },
			{ { -0.25f, -0.25f * m_aspectRatio, 0.0f }, { 0.0f, 1.0f } }
		};

		const UINT vertexBufferSize = sizeof(triangleVertices);

		// Note: using upload heaps to transfer static data like vert buffers is not 
		// recommended. Every time the GPU needs it, the upload heap will be marshalled 
		// over. Please read up on Default Heap usage. An upload heap is used here for 
		// code simplicity and because there are very few verts to actually transfer.
		hr = mrDXDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mTmpVertexBuffer));
		
		if( FAILED(hr) )
			return false;
		zSetGfxResourceName(mTmpVertexBuffer, L"TempVertexBuffer");

		// Copy the triangle data to the vertex buffer.
		UINT8* pVertexDataBegin;
		CD3DX12_RANGE readRange(0, 0);		// We do not intend to read from this resource on the CPU.
		hr = mTmpVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
		if( FAILED(hr) )
			return false;

		memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
		mTmpVertexBuffer->Unmap(0, nullptr);

		// Initialize the vertex buffer view.
		mTmpVertexBufferView.BufferLocation = mTmpVertexBuffer->GetGPUVirtualAddress();
		mTmpVertexBufferView.StrideInBytes = sizeof(Vertex);
		mTmpVertexBufferView.SizeInBytes = vertexBufferSize;
	}

	//==============================================================================================
	//! Temp Texture creation
	//! @todo 0 replace this with proper texture support 
	//==============================================================================================

	//-----------------------------------------------------------
	// Prepare some data for asset creation
	zArrayStatic<zU8>			aTexRGBA;
	zVec2U16					vTexSize(256,256);
	zenConst::eTextureFormat	eTexFormat = zenConst::keTexFormat_RGBA8;
	aTexRGBA.SetCount( vTexSize.x*vTexSize.y*4 );
	zU8*						pTexCur = aTexRGBA.First();
	for(zUInt line=0; line<vTexSize.y; ++line)
	{
		for(zUInt col=0; col<vTexSize.x; ++col)
		{
			*pTexCur++ = (line/16+col/16) % 2 == 0 ? 0xFF : 0x10;
			*pTexCur++ = (line/16+col/16) % 2 == 0 ? 0xFF : 0x10;
			*pTexCur++ = (line/16+col/16) % 2 == 0 ? 0xFF : 0xFF;
			*pTexCur++ = 1;
		}
	};

	mrTmpTexture	= zenRes::zGfxTexture2D::Create(zenConst::keTexFormat_RGBA8, vTexSize, aTexRGBA );
	
	// Close the command list and execute it to begin the initial GPU setup.
	hr = marCommandList[0][0]->Close();
	if( FAILED(hr) )
		return false;

	ID3D12CommandList* ppCommandLists[] = { marCommandList[0][0].Get() };
	mrCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
	
	//----------------------------------------------------------------------------------------------
	// Create synchronization objects and wait until assets have been uploaded to the GPU.
	//----------------------------------------------------------------------------------------------
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

bool ManagerRender_DX12::Unload()
{
#if !ZEN_RENDERER_DX12
	if( mDX12pPerf ) 
		mDX12pPerf->Release();
#endif	
	//mrDXCmdQueueDirect.Reset(); //SF Clear/Release?
	mrDXDevice.Reset();
	mrDXFactory.Reset();
#if !ZEN_RENDERER_DX12
	mDX12pPerf				= nullptr;
	mDX12pContextImmediate	= nullptr;
#endif
	
	return true;
}

 void ManagerRender_DX12::WaitForPreviousFrame()
{
	// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
	// This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
	// sample illustrates how to use fences for efficient resource usage and to
	// maximize GPU utilization.

	// Signal and increment the fence value.
	const UINT64 fence = m_fenceValue;
	mrCommandQueue->Signal(m_fence.Get(), fence);
	m_fenceValue++;

	// Wait until the previous frame is finished.
	if (m_fence->GetCompletedValue() < fence)
	{
		m_fence->SetEventOnCompletion(fence, m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}

	//m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
}

void ManagerRender_DX12::FrameBegin(zcRes::GfxWindowRef _FrameWindow)
{	
	WaitForPreviousFrame();
	marTempResource[muFrameRendered%zenArrayCount(marTempResource)].Clear();

	// Command list allocators can only be reset when the associated 
	// command lists have finished execution on the GPU; apps should use 
	// fences to determine GPU execution progress.
	mrCommandAllocator->Reset();

	//! @todo 1 support mutiple context/cmdlist
	// However, when ExecuteCommandList() is called on a particular command 
	// list, that command list can then be reset at any time and must be before re-recording.
	marCommandList[0][muFrameRendered%2]->Reset(mrCommandAllocator.Get(), nullptr);

	mGpuContext[0].Reset( mrDXDevice, marCommandList[0][muFrameRendered%2] );
	Super::FrameBegin(_FrameWindow);

	// Indicate that the back buffer will be used as a render target.
	const zcRes::GfxTarget2DRef& rBackbuffer = grWindowRender->GetBackbuffer();
	mGpuContext[0].GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(rBackbuffer.HAL()->mrResource.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET) );
	
#if !ZEN_RENDERER_DX12
	mrPreviousDrawcall		= nullptr;
	mbProfilerDetected		= mDX12pPerf && mDX12pPerf->GetStatus();
	mrQueryDisjoint			= DX12QueryDisjoint::Create();
	mrQueryDisjoint->Start();
#endif
}

void ManagerRender_DX12::FrameEnd()
{	
	const zcRes::GfxTarget2DRef& rBackbuffer = grWindowRender->GetBackbuffer();

	//mrQueryDisjoint->Stop();		

	// Indicate that the back buffer will now be used to present.
	mGpuContext[0].GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(rBackbuffer.HAL()->mrResource.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	HRESULT hr = mGpuContext[0].GetCommandList()->Close();
	if (FAILED(hr))
		return;
	ID3D12CommandList* ppCommandLists[] = { marCommandList[0][muFrameRendered%2].Get() };
	mrCommandQueue->ExecuteCommandLists(zenArrayCount(ppCommandLists), ppCommandLists);
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
	Super::FrameEnd();	
}

void ManagerRender_DX12::NamedEventBegin(const zStringHash32& zName)
{
#if !ZEN_RENDERER_DX12
	if( mbProfilerDetected )
	{
		WCHAR zEventName[64];
		mbstowcs_s(nullptr, zEventName, zName.mzName, zenArrayCount(zEventName));
		mDX12pPerf->BeginEvent(zEventName);
	}
#endif
}

void ManagerRender_DX12::NamedEventEnd()
{
#if !ZEN_RENDERER_DX12
	if( mbProfilerDetected )
		mDX12pPerf->EndEvent();
#endif
}

const zEngineRef<DX12QueryDisjoint>& ManagerRender_DX12::GetQueryDisjoint()const
{
	return mrQueryDisjoint;
}

void ManagerRender_DX12::Render(zArrayDynamic<zEngineRef<zcGfx::Command>>& _aDrawcalls)
{		
	mGpuContext[0].Submit(_aDrawcalls);
}

DirectXComRef<ID3D12Resource>& ManagerRender_DX12::GetTempResourceHandle()
{
	marTempResource[muFrameRendered%zenArrayCount(marTempResource)].Push(nullptr);
	return *marTempResource[muFrameRendered%zenArrayCount(marTempResource)].Last();
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