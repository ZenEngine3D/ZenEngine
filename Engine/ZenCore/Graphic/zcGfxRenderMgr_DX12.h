#pragma once

namespace zcGfx
{

class QueryHeapRingbuffer_DX12
{
public:
									QueryHeapRingbuffer_DX12(){}
	void							Initialize(D3D12_QUERY_HEAP_TYPE _eQueryType, zU64 _uCount);
	zUInt							GetNewQuery();
 	zenInline void					QueryStart(GPUContext& _Context, zU64 _uQueryIndex);
 	zenInline void					QueryEnd(GPUContext& _Context, zU64 _uQueryIndex);									
									template<typename TResultType>  
	const TResultType&				QueryResult(zU64 _uQueryIndex);
	zenInline bool					IsQueryReady(zU64 _uQueryIndex);
	void							Submit(GPUContext& _Context);
	void							Fetch(GPUContext& _Context);
	
protected:									
	DirectXComRef<ID3D12QueryHeap>	mrDXQueryHeap;
	DirectXComRef<ID3D12Resource>	mrDXQueryResources;
	DirectXComRef<ID3D12Fence>		mrDXFence;				//!< Last resolved Query Index
	D3D12_QUERY_TYPE				meQueryType;
	zArrayStatic<zU8>				maResultData;
	UINT64							muDXFrequency	= 0;	//!< For timestamp query, the clock frequency of cmdlist
	std::atomic<zU64>				muIndexCurrent	= 0;	//!< Next available Query Index
	std::atomic<zU64>				muIndexStart	= 0;	//!< First un-submitted Query Index
	zU64							muIndexFence	= 0;	//!< Last resolved Query Index saved from fence)
	zU64							muIndexResolved	= 0;	//!< Last resolved Query Index saved from fence)
	zU64							muQueryCount	= 0;	//!< Number of Queries in the RingBuffer
	zU64							muQueryDataSize	= 0;	//!< Size of 1 query
};

//=================================================================================================
//! @brief		zbType::Manager used to control hardware DX12 renderer
//! @details	
//=================================================================================================	
class ManagerRender_DX12 : public ManagerRender_Base
{
zenClassDeclare(ManagerRender_DX12, ManagerRender_Base)
enum kuConstant{ kuContextCount = 1, kuFrameBufferCount = 2 };
//---------------------------------------------------------
// Common to all ManagerRender
//---------------------------------------------------------
public:
	
	virtual void								FrameBegin(zcRes::GfxWindowRef _FrameWindow);
	virtual void								FrameEnd();
	void										SubmitToGPU(const CommandListRef& _rCommandlist, const zArrayDynamic<CommandRef>& _rCommands);

//---------------------------------------------------------
// DirectX device infos
//---------------------------------------------------------
public:		
	const DirectXComRef<IDXGIFactory4>&			GetFactory()const			{return mrDXFactory;}
	const DirectXComRef<ID3D12Device>&			GetDevice()const			{return mrDXDevice;}
	
	DXGI_FORMAT									ZenFormatToNative( zenConst::eTextureFormat _eTexFormat )const		{ return meFormatConv[_eTexFormat]; }
	DXGI_FORMAT									ZenFormatToTypeless( zenConst::eTextureFormat _eTexFormat )const	{ return meFormatConvTypeless[_eTexFormat]; }
	DXGI_FORMAT									ZenFormatToDepthDSV( zenConst::eTextureFormat _eTexFormat )const	{ return meFormatConvDepthDSV[_eTexFormat]; }
	DXGI_FORMAT									ZenFormatToDepthSRV( zenConst::eTextureFormat _eTexFormat )const	{ return meFormatConvDepthSRV[_eTexFormat]; }
	DXGI_FORMAT									ZenFormatToStencilSRV( zenConst::eTextureFormat _eTexFormat )const	{ return meFormatConvStencilSRV[_eTexFormat]; }
	
//! @todo urgent clean this up
	void										UnbindTextures(){};
	void										UnbindResources(){};

	zenInline DescriptorRangeSRV				GetFrameDescriptorSRV(zUInt _uCount); //! @todo move this to gpu context directly
	zenInline DescriptorRangeSRV				GetDescriptorSRV(zUInt _uCount);
	zenInline DescriptorRangeRTV				GetDescriptorRTV(zUInt _uCount);
	zenInline DescriptorRangeDSV				GetDescriptorDSV(zUInt _uCount);
	zenInline DescriptorRangeSampler			GetDescriptorSampler(zUInt _uCount);

	zenInline QueryHeapRingbuffer_DX12&			GetQueryTimestamp(){return mQueryTimestampHeap;}
	zenInline zU64								GetQueryTimestampFreq(){return mQueryTimestampFreq;}

protected:
	zenInline void								DispatchBarrier(const CommandListRef& _rCommandlist, bool _bPreDataUpdate );
	DXGI_FORMAT									meFormatConv[zenConst::keTexFormat__Count];
	DXGI_FORMAT									meFormatConvTypeless[zenConst::keTexFormat__Count];
	DXGI_FORMAT									meFormatConvDepthDSV[zenConst::keTexFormat__Count];
	DXGI_FORMAT									meFormatConvDepthSRV[zenConst::keTexFormat__Count];
	DXGI_FORMAT									meFormatConvStencilSRV[zenConst::keTexFormat__Count];
	
	DirectXComRef<IDXGIFactory4>				mrDXFactory;
	DirectXComRef<IDXGIAdapter3>				mrDXAdapter;
	DirectXComRef<ID3D12Device>					mrDXDevice;
	DirectXComRef<ID3D12Debug1>					mrDXDebugController;	

	DescriptorHeapSRV							mDescriptorHeapSRV;								//!< SRV/UAV/CBV descriptor heap used by resources object but not mapped to GPU (binding to GPU done with heap ring buffer)	
	DescriptorHeapRTV							mDescriptorHeapRTV;								//!< Render target descriptor heap used by resources object but not mapped to GPU (binding to GPU done with heap ring buffer)
	DescriptorHeapDSV							mDescriptorHeapDSV;								//!< Depth render target descriptor heap used by resources object but not mapped to GPU (binding to GPU done with heap ring buffer)
	DescriptorHeapSampler						mDescriptorHeapSampler;							//!< Sampler descriptor heap used by resources object but not mapped to GPU (binding to GPU done with heap ring buffer)
	DescriptorHeapSRV							maFrameDescriptorHeapSRV[kuFrameBufferCount];	//!< Descriptor heap for temporary SRV mapped to gpu everyframe and released (unlike permanent descriptor, this one gives access to resources in shader)
	DescriptorRangeSRV							maFrameDescriptorSRV[kuFrameBufferCount];		//!< Descriptor Range encompassing entire frame descriptor heap (don't use manual tracking of allocated descriptors for this, just keep incrementing used index in this descriptor range for better performances)
	zUInt										muFrameDescriptorCount	= 32*1024;				//!< Maximum number of resources that can be binded to GPU per frame @todo 2 Allow specifying the size of ring buffer
	std::atomic<zUInt>							muFrameDescriptorIndex	= 0;					//!< Position withing current frame 'maFrameDescriptorSRV' @todo 2 see about MT strategy

	QueryHeapRingbuffer_DX12					mQueryTimestampHeap;							//!< Heap of queries for Timestamps
	zU64										mQueryTimestampFreq;							//!< Last read GPU Frequency (ticks/sec), needed for Timestamp)
	DirectXComRef<ID3D12CommandAllocator>		mrCommandAllocator;	
	DirectXComRef<ID3D12GraphicsCommandList>	marCommandList[kuFrameBufferCount][kuContextCount];
	
	// Synchronization objects.
	HANDLE										m_fenceEvent;
	DirectXComRef<ID3D12Fence>					m_fence;
	UINT64										m_fenceValue;
	void										WaitForPreviousFrame();

//! @todo 1 stop having this public
public:
	zcGfx::RootSignature						mRootSignatureDefault;
	DirectXComRef<ID3D12CommandQueue>			mrCommandQueue;

protected:
	GPUContext									mGpuContext[kuContextCount];	//!< @note Only 1 context for the moment, increase when multithreading is supported		
	bool										mbTextureUnbind		= false;
	bool										mbResourceUnbind	= false;
	bool										mbProfilerDetected	= false;		

//---------------------------------------------------------
// ManagerBase Section
//---------------------------------------------------------
public:
	virtual	bool								Load();
	virtual	bool								Unload();
};

}

#include "zcGfxRenderMgr_DX12.inl"
