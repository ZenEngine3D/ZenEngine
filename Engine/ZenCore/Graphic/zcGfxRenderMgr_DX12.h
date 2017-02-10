#pragma once

//SF DX12

#include "zcGfxRenderMgrTmp_DX12.h"

//! @todo 0 clean remove this, needed until we support vertex buffer properly
struct Vertex
{
	zVec3F position;
	zVec2F uv;
};

namespace zcGfx
{
//=================================================================================================
//! @brief		Encapsulate DirectX object needed to query device context about clock rate
//! @details	
//! @todo		Move to own file, with other queries
//=================================================================================================	
class DX12QueryDisjoint : public zRefCounted
{
public:
	static zEngineRef<DX12QueryDisjoint>	Create();					//!< @brief Get a new disjoint query	
	void									Start();					//!< @brief Starts clock frequency query
	void									Stop();						//!< @brief Stops clock frequency query	
	zU64									GetClockRate();				//!< @brief Gets clock frequency result (0 if invalid)

protected:
											DX12QueryDisjoint();
	virtual void							ReferenceDeleteCB();		//!< @brief Return object to free list instead of deleting it
	ID3D11Query*							mpDX12Query;				//!< @brief DirectX disjoint query object used to get result
	D3D11_QUERY_DATA_TIMESTAMP_DISJOINT		mDisjointInfo;				//!< @brief Frequency infos returned from query, about the GPU
	zU64									muFrameStop;				//!< @brief When query was ended (to make sure 1 frame elapsed)
	bool									mbValidResult;				//!< @brief True if we got the result back from GPU
	
protected:	zListLink						mlstLink;
public:		typedef zList<DX12QueryDisjoint, &DX12QueryDisjoint::mlstLink, false> List;
protected:	static List						slstQueryCreated;
};

//=================================================================================================
//! @brief		Encapsulate DirectX object needed to query device context about current time
//! @details	
//=================================================================================================	
class DX12QueryTimestamp : public zRefCounted
{
public:
	static zEngineRef<DX12QueryTimestamp>	Create();				//!< @brief Get a new disjoint query and start the timestamp request
	zU64									GetTimestampUSec();		//!< @brief Retrieve the timestamp result (0 if invalid)
		
protected:
											DX12QueryTimestamp();
	virtual void							ReferenceDeleteCB();	//!< @brief Return object to free list instead of deleting it
	ID3D11Query*							mpDX12Query;			//!< @brief DirectX timestamp query object used to get result
	zEngineRef<DX12QueryDisjoint>			mrQueryDisjoint;		//!< @brief Reference to Disjoint query to use for getting gpu frequency
	bool									mbValidResult;			//!< @brief True if we got the result back from GPU
	zU64									muTimestamp;			//!< @brief Time on the GPU when query was processed (in microseconds)
	zListLink								mlstLink;
public:
	typedef zList<DX12QueryTimestamp, &DX12QueryTimestamp::mlstLink, false> List;
protected:
	static List								slstQueryCreated;
};


//=================================================================================================
//! @brief		Keep track of resource descriptor heap
//! @details	
//! @todo 1 Cleanup this once figure out how it work. Should use ref count?
//=================================================================================================	
template<int TType, int THeapCount>
class ResourceDescriptor
{
public:
	enum constant { kuInvalid=0xFFFFFFFF, kuType=TType, kuHeapCount=THeapCount };
	bool IsValid()const
	{
		return muIndex != kuInvalid;
	}

	void Free()
	{
		if( IsValid() )
			saDescriptorUsed.Set(muIndex, false);
		muIndex = kuInvalid;
	}

	const D3D12_CPU_DESCRIPTOR_HANDLE& GetCpuHandle() const 
	{
		return mhDescriptor;
	}

	const D3D12_GPU_DESCRIPTOR_HANDLE& GetGpuHandle() const
	{
		return mhDescriptorGPU;
	}
	static ResourceDescriptor Allocate()
	{
		//! @todo 1 support bounds check
		ResourceDescriptor NewDesc;
		NewDesc.muIndex				= saDescriptorUsed.AddIndexTrue();
		NewDesc.mhDescriptor.ptr	= srDXCPUHandle.ptr + NewDesc.muIndex*suDescriptorSize;
		NewDesc.mhDescriptorGPU.ptr	= srDXGPUHandle.ptr + NewDesc.muIndex*suDescriptorSize;
		return NewDesc;
	}
	
	static const DirectXComRef<ID3D12DescriptorHeap>& GetDescHeap() {return srDXDescriptorHeap;}

	static bool Initialize( D3D12_DESCRIPTOR_HEAP_TYPE _eHeapType, D3D12_DESCRIPTOR_HEAP_FLAGS _eFlags=D3D12_DESCRIPTOR_HEAP_FLAG_NONE )
	{
		zenAssertMsg(suDescriptorSize == 0, "Resource Descriptor Heap already initialized");
		D3D12_DESCRIPTOR_HEAP_DESC HeapDesc={};
		HeapDesc.Type			= _eHeapType;
		HeapDesc.NumDescriptors = kuHeapCount;
		HeapDesc.Flags			= _eFlags;
		HeapDesc.NodeMask		= 0;
		HRESULT hr = zcMgr::GfxRender.GetDevice()->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&srDXDescriptorHeap));
		if( FAILED( hr ) )
			return false;
		
		srDXCPUHandle			= srDXDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		srDXGPUHandle			= srDXDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
		suDescriptorSize		= zcMgr::GfxRender.GetDevice()->GetDescriptorHandleIncrementSize(_eHeapType);
		saDescriptorUsed.SetRange(0, ResourceDescriptor::kuHeapCount, false);
		return true;
	}
protected:
	zUInt										muIndex	= kuInvalid;
	D3D12_CPU_DESCRIPTOR_HANDLE					mhDescriptor;
	D3D12_GPU_DESCRIPTOR_HANDLE					mhDescriptorGPU;

	static DirectXComRef<ID3D12DescriptorHeap>	srDXDescriptorHeap;
	static D3D12_CPU_DESCRIPTOR_HANDLE			srDXCPUHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE			srDXGPUHandle;
	static zUInt								suDescriptorSize;
	static zArrayBits							saDescriptorUsed;
	static 
	friend class ManagerRender;
};

template<int TType, int THeapCount>	DirectXComRef<ID3D12DescriptorHeap> ResourceDescriptor<TType,THeapCount>::srDXDescriptorHeap;
template<int TType, int THeapCount>	D3D12_CPU_DESCRIPTOR_HANDLE			ResourceDescriptor<TType,THeapCount>::srDXCPUHandle;
template<int TType, int THeapCount>	D3D12_GPU_DESCRIPTOR_HANDLE			ResourceDescriptor<TType,THeapCount>::srDXGPUHandle;
template<int TType, int THeapCount>	zUInt								ResourceDescriptor<TType,THeapCount>::suDescriptorSize = 0;
template<int TType, int THeapCount>	zArrayBits							ResourceDescriptor<TType,THeapCount>::saDescriptorUsed;

using DescriptorRTV			= ResourceDescriptor<0, 128>;
using DescriptorDSV			= ResourceDescriptor<1, 64>;
using DescriptorSRV_UAV_CBV = ResourceDescriptor<2, 1024>;


//=================================================================================================
//! @brief		zbType::Manager used to control hardware DX12 renderer
//! @details	
//=================================================================================================	
class ManagerRender_DX12 : public ManagerRender_Base
{
zenClassDeclare(ManagerRender_DX12, ManagerRender_Base)
enum kuConstant{ kuContextCount = 1 };
//---------------------------------------------------------
// Common to all ManagerRender
//---------------------------------------------------------
public:
	
	virtual void								FrameBegin(zcRes::GfxWindowRef _FrameWindow);
	virtual void								FrameEnd();
	void										Render(zArrayDynamic<zEngineRef<zcGfx::Command>>& _aDrawcalls);
	void										NamedEventBegin(const zStringHash32& zName);
	void										NamedEventEnd();
	const zEngineRef<DX12QueryDisjoint>&		GetQueryDisjoint()const;

//---------------------------------------------------------
// DirectX device infos
//---------------------------------------------------------
public:		
	const DirectXComRef<IDXGIFactory4>&				GetFactory()const			{return mrDXFactory;}
	const DirectXComRef<ID3D12Device>&				GetDevice()const			{return mrDXDevice;}

	DXGI_FORMAT										ZenFormatToNative( zenConst::eTextureFormat _eTexFormat )const		{ return meFormatConv[_eTexFormat]; }
	DXGI_FORMAT										ZenFormatToTypeless( zenConst::eTextureFormat _eTexFormat )const	{ return meFormatConvTypeless[_eTexFormat]; }
	DXGI_FORMAT										ZenFormatToDepthDSV( zenConst::eTextureFormat _eTexFormat )const	{ return meFormatConvDepthDSV[_eTexFormat]; }
	DXGI_FORMAT										ZenFormatToDepthSRV( zenConst::eTextureFormat _eTexFormat )const	{ return meFormatConvDepthSRV[_eTexFormat]; }
	DXGI_FORMAT										ZenFormatToStencilSRV( zenConst::eTextureFormat _eTexFormat )const	{ return meFormatConvStencilSRV[_eTexFormat]; }
	
//! @todo urgent clean this up
	void											UnbindTextures(){};
	void											UnbindResources(){};
	
	DirectXComRef<ID3D12Resource>&					GetTempResourceHandle();

protected:
	DXGI_FORMAT										meFormatConv[zenConst::keTexFormat__Count];
	DXGI_FORMAT										meFormatConvTypeless[zenConst::keTexFormat__Count];
	DXGI_FORMAT										meFormatConvDepthDSV[zenConst::keTexFormat__Count];
	DXGI_FORMAT										meFormatConvDepthSRV[zenConst::keTexFormat__Count];
	DXGI_FORMAT										meFormatConvStencilSRV[zenConst::keTexFormat__Count];

	DirectXComRef<IDXGIFactory4>					mrDXFactory;
	DirectXComRef<IDXGIAdapter3>					mrDXAdapter;
	DirectXComRef<ID3D12Device>						mrDXDevice;
	DirectXComRef<ID3D12Debug1>						mrDXDebugController;	
	zArrayDynamic<DirectXComRef<ID3D12Resource>>	marTempResource[2];		//!< Temp allocated resources freed after 2 frames

public:
	zcGfx::RootSignature							mRootSignatureDefault;
	DirectXComRef<ID3D12CommandAllocator>			mrCommandAllocator;
	DirectXComRef<ID3D12CommandQueue>				mrCommandQueue;
	DirectXComRef<ID3D12GraphicsCommandList>		marCommandList[kuContextCount][2];
	
	DirectXComRef<ID3D12PipelineState>				mTmpPipelineState;
	DirectXComRef<ID3D12Resource>					mTmpVertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW						mTmpVertexBufferView;
	zcGfx::DescriptorSRV_UAV_CBV					mTmpTextureView;
	zenRes::zGfxTexture2D							mrTmpTexture;
	// Synchronization objects.
	HANDLE											m_fenceEvent;
	DirectXComRef<ID3D12Fence>						m_fence;
	UINT64											m_fenceValue;
	void											WaitForPreviousFrame();

protected:
	GPUContext										mGpuContext[kuContextCount];	//!< @note Only 1 context for the moment, increase when multihreading is supported	
	bool											mbTextureUnbind		= false;
	bool											mbResourceUnbind	= false;
	bool											mbProfilerDetected	= false;		
	zEngineRef<DX12QueryDisjoint>					mrQueryDisjoint;

//---------------------------------------------------------
// ManagerBase Section
//---------------------------------------------------------
public:
	virtual	bool									Load();
	virtual	bool									Unload();


	friend class CommandDraw_DX12; //! @todo 0 remove this
};

}

