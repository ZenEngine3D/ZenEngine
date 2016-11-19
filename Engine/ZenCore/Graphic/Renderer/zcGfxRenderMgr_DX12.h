#pragma once
#ifndef __zCore_Gfx_Renderer_Manager_DX12_h__
#define __zCore_Gfx_Renderer_Manager_DX12_h__
//SF DX12

#include "zcGfxRenderMgrTmp_DX12.h"

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
	zListLink								mlstLink;
public:
	typedef zList<DX12QueryDisjoint, &DX12QueryDisjoint::mlstLink, false> List;
protected:
	static List								slstQueryCreated;
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

	D3D12_CPU_DESCRIPTOR_HANDLE GetHandle() const
	{
		return mhDescriptor;
	}

	static ResourceDescriptor Allocate()
	{
		//! @todo 1 support bounds check
		ResourceDescriptor NewDesc;
		NewDesc.muIndex				= saDescriptorUsed.AddIndexTrue();
		NewDesc.mhDescriptor.ptr	= srDXCPUHandle.ptr + NewDesc.muIndex*suDescriptorSize;
		return NewDesc;
	}
	
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
		saDescriptorUsed.SetRange(0, DescriptorRTV::kuHeapCount, false);

		
		DirectXComRef< ID3D12DescriptorHeap > m_srvHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE m_srvHeapCPUHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE m_srvHeapGPUHandle;

		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc;		
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.NumDescriptors = 10000;//DirectX12::g_numShaderResourceDesciptors;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		srvHeapDesc.NodeMask = 0;
		if(FAILED(zcMgr::GfxRender.GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_srvHeap))))
			throw "Failed to create shader resource view descriptor heap";
		//ox_debugOnly(m_srvHeap->SetName(L"Renderer::m_srvHeap"));
		//ox_renew(m_srvHeapAllocator, 0, DirectX12::g_numShaderResourceDesciptors);
		m_srvHeapCPUHandle = m_srvHeap->GetCPUDescriptorHandleForHeapStart();
		m_srvHeapGPUHandle = m_srvHeap->GetGPUDescriptorHandleForHeapStart();
		
		return true;
	}
protected:
	zUInt										muIndex	= kuInvalid;
	D3D12_CPU_DESCRIPTOR_HANDLE					mhDescriptor;

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

using DescriptorRTV = ResourceDescriptor<0, 128>;
using DescriptorDSV = ResourceDescriptor<1, 64>;
using DescriptorSRV = ResourceDescriptor<2, 128>;

//=================================================================================================
//! @brief		zbType::Manager used to control hardware DX12 renderer
//! @details	
//=================================================================================================	
class ManagerRender : public ManagerRender_Base
{
zenClassDeclare(ManagerRender, ManagerRender_Base)
//---------------------------------------------------------
// Common to all ManagerRender
//---------------------------------------------------------
public:
	struct RenderContext
	{
												RenderContext();
		zcRes::GfxRenderPassRef					mrRenderpass		= nullptr;
		zcRes::GfxViewRef						mrStateView			= nullptr;
		zcRes::GfxStateBlendRef					mrStateBlend		= nullptr;
		zcRes::GfxStateDepthStencilRef			mrStateDepthStencil	= nullptr;
		zcRes::GfxStateRasterRef				mrStateRaster		= nullptr;
		
		// Useful for debugging/tracking but not needed
		zcRes::GfxShaderAnyRef					marShader[keShaderStage__Count];
		zcRes::GfxSamplerRef					marSampler[keShaderStage__Count][zcExp::kuDX12_SamplerPerStageMax];
		zcRes::GfxCBufferRef					marCBuffer[keShaderStage__Count][zcExp::kuDX12_CBufferPerStageMax];
		zcRes::GfxShaderResourceRef				marResource[keShaderStage__Count][zcExp::kuDX12_ResourcesPerStageMax];		
		// Info on input shaders resources
		zHash32									mahShaderInputStamp[keShaderStage__Count][keShaderRes__Count];				//!< Hash of assigned resources per stage/restype, to quickly know if something has changed
		zU16									maShaderInputSlotCount[keShaderStage__Count][keShaderRes__Count];			//!< Slot count to last valid Resource view per resource type
		ID3D11ShaderResourceView*				maResourceView[keShaderStage__Count][zcExp::kuDX12_ResourcesPerStageMax];	//!< Resource view of all assigned resources (textures, structbuffer, uav, ...)
		eShaderResource							maResourceType[keShaderStage__Count][zcExp::kuDX12_ResourcesPerStageMax];	//!< Resource type assigned to matching resourceview slot
		
		D3D11_PRIMITIVE_TOPOLOGY				mePrimitiveType		= D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
		zVec4U16								mvScreenScissor		= zVec4U16(0, 0, 0, 0);
		bool									mbScreenScissorOn	= false;
	};

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
	const DirectXComRef<IDXGIFactory4>&			GetFactory()const			{return mrDXFactory;}
	const DirectXComRef<ID3D12Device>&			GetDevice()const			{return mrDXDevice;}
	//const DirectXComRef<ID3D12CommandAllocator>&GetCommandAllocator()const	{return mrDXCommandAllocator;}
//	CommandQueue&								GetCmdQueueDirect()			{return mrCmdQueueDirect;}	
	//CommandListManager&							GetCommandMgr()				{return mCommandManager;}
	//ID3D12GraphicsCommandList*					GetCommandList()			{return mCommandList;}
	DXGI_FORMAT									ZenFormatToNative( zenConst::eTextureFormat _eTexFormat )const		{ return meFormatConv[_eTexFormat]; }
	DXGI_FORMAT									ZenFormatToTypeless( zenConst::eTextureFormat _eTexFormat )const	{ return meFormatConvTypeless[_eTexFormat]; }
	DXGI_FORMAT									ZenFormatToDepthDSV( zenConst::eTextureFormat _eTexFormat )const	{ return meFormatConvDepthDSV[_eTexFormat]; }
	DXGI_FORMAT									ZenFormatToDepthSRV( zenConst::eTextureFormat _eTexFormat )const	{ return meFormatConvDepthSRV[_eTexFormat]; }
	DXGI_FORMAT									ZenFormatToStencilSRV( zenConst::eTextureFormat _eTexFormat )const	{ return meFormatConvStencilSRV[_eTexFormat]; }
	
//! @todo urgent clean this up
	void										UnbindTextures(){};
	void										UnbindResources(){};

protected:
	zenInline void								UpdateGPUState(const zEngineRef<zcGfx::Command>& _rDrawcall, RenderContext& _Context);
	zenInline void								UpdateShaderState(const zcGfx::CommandDraw& _Drawcall, RenderContext& _Context);	
	zenInline void								UpdateShaderState_Samplers(const zcGfx::CommandDraw& _Drawcall, RenderContext& _Context, eShaderStage _eShaderStage);
	zenInline void								UpdateShaderState_ConstantBuffers(const zcGfx::CommandDraw& _Drawcall, RenderContext& _Context, eShaderStage _eShaderStage);
	zenInline void								UpdateShaderState_Textures(zU16& Out_ChangedFirst, zU16& Out_ChangedLast, const zcGfx::CommandDraw& _Drawcall, RenderContext& _Context, eShaderStage _eShaderStage);	
	zenInline void								UpdateShaderState_StructBuffers(zU16& Out_ChangedFirst, zU16& Out_ChangedLast, const zcGfx::CommandDraw& _Drawcall, RenderContext& _Context, eShaderStage _eShaderStage);
	zEngineRef<zcGfx::Command>					mrPreviousDrawcall;
	DXGI_FORMAT									meFormatConv[zenConst::keTexFormat__Count];
	DXGI_FORMAT									meFormatConvTypeless[zenConst::keTexFormat__Count];
	DXGI_FORMAT									meFormatConvDepthDSV[zenConst::keTexFormat__Count];
	DXGI_FORMAT									meFormatConvDepthSRV[zenConst::keTexFormat__Count];
	DXGI_FORMAT									meFormatConvStencilSRV[zenConst::keTexFormat__Count];

	//ID3D12Device*								mDX12pDevice			= nullptr;
	//ID3D11DeviceContext*						mDX12pContextImmediate	= nullptr; //SF
	//ID3DUserDefinedAnnotation*					mDX12pPerf				= nullptr;

	DirectXComRef<IDXGIFactory4>				mrDXFactory;
	DirectXComRef< IDXGIAdapter3 >				mrDXAdapter;
	DirectXComRef<ID3D12Device>					mrDXDevice;
	DirectXComRef<ID3D12GraphicsCommandList>	mrDXCommandList; //! @todo 3 have this per gfx context for multithreading
	DirectXComRef<ID3D12Debug1>					mrDXDebugController;

public:
	DirectXComRef<ID3D12CommandAllocator>		m_commandAllocator;
	DirectXComRef<ID3D12CommandQueue>			m_commandQueue;
	DirectXComRef<ID3D12GraphicsCommandList>	m_commandList;
	// Synchronization objects.
	HANDLE										m_fenceEvent;
	DirectXComRef<ID3D12Fence>					m_fence;
	UINT64										m_fenceValue;
	void										WaitForPreviousFrame();
	//CommandListManager							mCommandManager;
	//ID3D12GraphicsCommandList*					mCommandList			= nullptr;
	//ID3D12CommandAllocator*						mCurrentAllocator		= nullptr;
protected:
	
	bool										mbTextureUnbind			= false;
	bool										mbResourceUnbind		= false;
	bool										mbDX12ProfilerDetected	= true;		
	zEngineRef<DX12QueryDisjoint>				mrQueryDisjoint;
//---------------------------------------------------------
// ManagerBase Section
//---------------------------------------------------------
public:
	virtual	bool			Load			();
	virtual	bool			Unload			();
};

}

#endif
