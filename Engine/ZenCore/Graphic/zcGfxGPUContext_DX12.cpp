#include "zcCore.h"

namespace zcGfx 
{

//! @todo 1 Improve this
zenInline zcGfx::DescriptorRangeSRV* GetResourceDescriptor( const zcRes::GfxShaderResourceRef& _rResource )
{
	if( _rResource.IsValid() )
	{
		if( _rResource.GetResID().GetType() == keResType_GfxBuffer )
		{
			zcRes::GfxBufferRef rBuffer = _rResource;
			return &rBuffer.HAL()->mResource.mView;
		}
		if( _rResource.GetResID().GetType() == keResType_GfxCBuffer )
		{
			zcRes::GfxCBufferRef rCBuffer = _rResource;
			return &rCBuffer.HAL()->mCBufferView;
		}
		if( _rResource.GetResID().GetType() == keResType_GfxTexture2D )
		{
			zcRes::GfxTexture2DRef rTexture = _rResource;
			return &rTexture.HAL()->mResource.mView;
		}
	}
	return nullptr;
}

void GPUContext_DX12::Reset(const DirectXComRef<ID3D12Device>& _rDevice, const DirectXComRef<ID3D12GraphicsCommandList>& _rCommandList, const DirectXComRef<ID3D12DescriptorHeap>& _rResViewDescHeap )
{	
	Super::Reset();
		
	zenAssert(_rDevice.Get());
	zenAssert(_rCommandList.Get());
	mrDevice						= _rDevice;
	mrCommandList					= _rCommandList;
	mRootSignature					= RootSignature();
	mrPSO							= nullptr;
	mePrimitive						= D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
	ID3D12DescriptorHeap* ppHeaps[] = { _rResViewDescHeap.Get() };
	mrCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void GPUContext_DX12::FlushPendingFences(const DirectXComRef<ID3D12CommandQueue>& _rCmdQueue)
{
	zUInt uCount(maPendingFences.size());
	for(zUInt idx(0); idx<uCount; ++idx)
	{
		_rCmdQueue->Signal( maPendingFences[idx].mrFence.Get(), maPendingFences[idx].mValue );
	}
	maPendingFences.Clear();
	maPendingFences.Reserve(uCount);
}

//==================================================================================================
//! @details Compare current bound samplers with wanted one and update their binding if different
//==================================================================================================
void GPUContext_DX12::UpdateShaderState_Samplers( const zcGfx::CommandDraw_HAL& _Drawcall, eShaderStage _eShaderStage )
{	
#if !ZEN_RENDERER_DX12
	ID3D11SamplerState*	aResourceView[zcExp::kuDX11_SamplerPerStageMax];
	const eShaderResource kShaderRes							= keShaderRes_Sampler;	
	const zArrayDyn<zcRes::GfxShaderResourceRef>& arResource	= _Drawcall.mrMeshStrip.HAL()->marShaderResources[_eShaderStage][kShaderRes];
	
	// Retrieve samplers new slots assignment
	zU16 uChangedFirst(0xFFFF), uChangedLast(0), uValidCount(0), uResCount(static_cast<zU16>(arResource.size()));
	zU16 uAssignCount = zenMath::Max(uResCount, maShaderInputSlotCount[_eShaderStage][kShaderRes]);
	for( zU16 slotIdx(0); slotIdx<uAssignCount; ++slotIdx )
	{
		marSampler[_eShaderStage][slotIdx]	= slotIdx < uResCount ? arResource[slotIdx] : nullptr;
		zcRes::GfxStateSamplerRef& rResource		= marSampler[_eShaderStage][slotIdx];
		ID3D11SamplerState* pDXView					= rResource.IsValid() ? rResource.HAL()->mpSamplerState	: nullptr;		
		uChangedFirst								= pDXView != aResourceView[slotIdx]	? zenMath::Min(uChangedFirst, slotIdx)	: uChangedFirst;
		uChangedLast								= pDXView != aResourceView[slotIdx]	? slotIdx								: uChangedLast;
		uValidCount									= pDXView							? slotIdx+1								: uValidCount;
		aResourceView[slotIdx]						= pDXView;
	}

	// Send it to API
	maShaderInputSlotCount[_eShaderStage][kShaderRes] = uValidCount;
	if( uChangedLast >= uChangedFirst )
	{
		switch( _eShaderStage )
		{
		case keShaderStage_Vertex:	mpDeviceContext->VSSetSamplers( uChangedFirst, uChangedLast-uChangedFirst+1, &aResourceView[uChangedFirst] ); break;
		case keShaderStage_Pixel:	mpDeviceContext->PSSetSamplers( uChangedFirst, uChangedLast-uChangedFirst+1, &aResourceView[uChangedFirst] ); break;
		default: break;
		}
	}
#endif
}

//==================================================================================================
//! @details Compare current bound Constant Buffers with wanted one and update their binding if different
//==================================================================================================
void GPUContext_DX12::UpdateShaderState_ConstantBuffers( const zcGfx::CommandDraw_HAL& _Drawcall, eShaderStage _eShaderStage )
{	
#if !ZEN_RENDERER_DX12
	ID3D11Buffer* aResourceView[zcExp::kuDX11_CBufferPerStageMax];
	const eShaderResource kShaderRes							= keShaderRes_CBuffer;	
	const zArrayDyn<zcRes::GfxShaderResourceRef>& arResource	= _Drawcall.mrMeshStrip.HAL()->marShaderResources[_eShaderStage][kShaderRes];
	
	// Retrieve samplers new slots assignment
	zU16 uChangedFirst(0xFFFF), uChangedLast(0), uValidCount(0), uSlotCount(static_cast<zU16>(arResource.size()));
	zU16 uAssignCount = zenMath::Max(uSlotCount, maShaderInputSlotCount[_eShaderStage][kShaderRes]);
	for( zU16 slotIdx(0); slotIdx<uAssignCount; ++slotIdx )
	{
		marCBuffer[_eShaderStage][slotIdx]	= slotIdx < uSlotCount ? arResource[slotIdx] : nullptr;
		zcRes::GfxCBufferRef& rResource				= marCBuffer[_eShaderStage][slotIdx];
		ID3D11Buffer* pDXView						= rResource.IsValid() ? rResource.HAL()->mpBufferBinding : nullptr;		
		uChangedFirst								= pDXView != aResourceView[slotIdx]	? zenMath::Min(uChangedFirst, slotIdx)	: uChangedFirst;
		uChangedLast								= pDXView != aResourceView[slotIdx]	? slotIdx								: uChangedLast;
		uValidCount									= pDXView							? slotIdx+1								: uValidCount;
		aResourceView[slotIdx]						= pDXView;
		
		//Must update CBuffer constant value if updated
		if( rResource.IsValid() )
			rResource.HAL()->Update( *mpDeviceContext ); 
	}

	// Send it to API
	maShaderInputSlotCount[_eShaderStage][kShaderRes] = uValidCount;
	if( uChangedLast >= uChangedFirst )
	{
		switch( _eShaderStage )
		{
			case keShaderStage_Vertex:	mpDeviceContext->VSSetConstantBuffers( uChangedFirst, uChangedLast-uChangedFirst+1, &aResourceView[uChangedFirst] );	break;
			case keShaderStage_Pixel:	mpDeviceContext->PSSetConstantBuffers( uChangedFirst, uChangedLast-uChangedFirst+1, &aResourceView[uChangedFirst] );	break;		
			default: break;
		}
	}
#endif
}

//==================================================================================================
//! @details Compare current bound textures with wanted one and update their binding if different
//==================================================================================================
void GPUContext_DX12::UpdateShaderState_Textures( zU16& Out_ChangedFirst, zU16& Out_ChangedLast, const zcGfx::CommandDraw_HAL& _Drawcall, eShaderStage _eShaderStage)
{	
#if !ZEN_RENDERER_DX12
	const eShaderResource kShaderRes							= keShaderRes_Texture;
	const zArrayDyn<zcRes::GfxShaderResourceRef>& arResource	= _Drawcall.mrMeshStrip.HAL()->marShaderResources[_eShaderStage][kShaderRes];
	zU16 uValidCount											= 0;
	const zU16 uSlotCount										= static_cast<zU16>(arResource.size());
	const zU16 uAssignCount										= zenMath::Max(uSlotCount, maShaderInputSlotCount[_eShaderStage][kShaderRes]);	
	for( zU16 slotIdx(0); slotIdx<uAssignCount; ++slotIdx )
	{
		zcRes::GfxTexture2DRef rResource	= slotIdx < uSlotCount	? arResource[slotIdx]			: nullptr;
		ID3D11ShaderResourceView* pDXView	= rResource.IsValid()	? rResource.HAL()->mpTextureView : nullptr;
		if( pDXView || maResourceType[_eShaderStage][slotIdx] == kShaderRes )
		{
			Out_ChangedFirst						= pDXView && pDXView != maResourceView[_eShaderStage][slotIdx] ? zenMath::Min(Out_ChangedFirst, slotIdx)	: Out_ChangedFirst;
			Out_ChangedLast							= pDXView && pDXView != maResourceView[_eShaderStage][slotIdx] ? zenMath::Max(Out_ChangedLast, slotIdx)	: Out_ChangedLast;
			uValidCount								= pDXView	? slotIdx+1	: uValidCount;
			maResourceType[_eShaderStage][slotIdx]	= pDXView	? kShaderRes : keShaderRes__Invalid;
			maResourceView[_eShaderStage][slotIdx]	= pDXView;			
			marResource[_eShaderStage][slotIdx]		= rResource;
		}		
	}
	maShaderInputSlotCount[_eShaderStage][kShaderRes] = uValidCount;
#endif
}

//==================================================================================================
//! @details Compare current bound textures with wanted one and update their binding if different
//==================================================================================================
void GPUContext_DX12::UpdateShaderState_StructBuffers( zU16& Out_ChangedFirst, zU16& Out_ChangedLast, const zcGfx::CommandDraw_HAL& _Drawcall, eShaderStage _eShaderStage)
{	
#if !ZEN_RENDERER_DX12 //! @todo 2 look into these defines
	const eShaderResource kShaderRes							= keShaderRes_Buffer;
	const zArrayDyn<zcRes::GfxShaderResourceRef>& arResource	= _Drawcall.mrMeshStrip.HAL()->marShaderResources[_eShaderStage][kShaderRes];
	zU16 uValidCount											= 0;
	const zU16 uSlotCount										= static_cast<zU16>(arResource.size());
	const zU16 uAssignCount										= zenMath::Max(uSlotCount, maShaderInputSlotCount[_eShaderStage][kShaderRes]);
	
	for( zU16 slotIdx(0); slotIdx<uAssignCount; ++slotIdx )
	{
		zcRes::GfxBufferRef rResource						= slotIdx < uSlotCount	? arResource[slotIdx]		: nullptr;
		ID3D11ShaderResourceView* pDXView					= rResource.IsValid()	? rResource.HAL()->mpSRV	: nullptr;
		if( pDXView || maResourceType[_eShaderStage][slotIdx] == kShaderRes )
		{
			Out_ChangedFirst								= pDXView && pDXView != maResourceView[_eShaderStage][slotIdx] ? zenMath::Min(Out_ChangedFirst, slotIdx)	: Out_ChangedFirst;
			Out_ChangedLast									= pDXView && pDXView != maResourceView[_eShaderStage][slotIdx] ? zenMath::Max(Out_ChangedLast, slotIdx)	: Out_ChangedLast;
			uValidCount										= pDXView	? slotIdx+1	: uValidCount;
			maResourceType[_eShaderStage][slotIdx]	= pDXView	? kShaderRes : keShaderRes__Invalid;
			maResourceView[_eShaderStage][slotIdx]	= pDXView;			
			marResource[_eShaderStage][slotIdx]	= rResource;
		}		
	}
	maShaderInputSlotCount[_eShaderStage][kShaderRes] = uValidCount;
#endif
}

//==================================================================================================
//! @details 
//==================================================================================================
void GPUContext_DX12::UpdateStateRenderpass(const zcGfx::CommandDraw_HAL& _Drawcall)
{	
	if( mrRenderpass != _Drawcall.mrRenderPass )
	{
		mrRenderpass		= _Drawcall.mrRenderPass;
//		auto rRenderpassHAL	= _Drawcall.mrRenderPass.HAL();
		
		if( mrStateView != mrRenderpass.HAL()->mrStateView )
		{				
			mrStateView								= mrRenderpass.HAL()->mrStateView;
			const zcRes::GfxView_HAL* const ViewHAL	= mrStateView.HAL();
			zUInt uTargetCount(ViewHAL->maRTColorConfig.size());
			D3D12_CPU_DESCRIPTOR_HANDLE aCpuHandles[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT];
			for(zUInt idxTarget(0); idxTarget<uTargetCount; ++idxTarget )
			{
				zcRes::GfxTarget2DRef rTarget	= ViewHAL->maRTColorConfig[idxTarget].mrTargetSurface;
				if( rTarget.IsValid() )
					aCpuHandles[idxTarget]		= rTarget.HAL()->mTargetColorView.GetCpu();
			}

			zcRes::GfxTarget2DRef rDepthTarget = ViewHAL->mRTDepthConfig.mrTargetSurface;
			if( rDepthTarget.IsValid() )
				mrCommandList->OMSetStencilRef( mrRenderpass.HAL()->mrStateDepthStencil.HAL()->muStencilValue );

			mrCommandList->OMSetRenderTargets((UINT)uTargetCount, aCpuHandles, false, rDepthTarget.IsValid() ? &rDepthTarget.HAL()->mTargetDepthView.GetCpu() : nullptr);
			mrCommandList->RSSetViewports(1, &ViewHAL->mViewport);
		}		
	}
}

//==================================================================================================
//! @details Check all GPU states, and update the one that differ from current Drawcall
//==================================================================================================
void GPUContext_DX12::UpdateState(const CommandDraw_HAL& _Drawcall)
{
	auto const pMeshStripHAL						= _Drawcall.mrMeshStrip.HAL();
	const zcRes::GfxIndexRef& rIndex				= pMeshStripHAL->mrIndexBuffer;
//	const zcRes::GfxShaderBindingRef& rShaderBind	= pMeshStripHAL->mrShaderBinding;			
	const zcRes::GfxViewRef& rView					= mrStateView;

	//SF @todo 1 support multiple root signature
	//----------------------------------------------------------------------------------------------
	// Root Signature
	//----------------------------------------------------------------------------------------------
	if( mRootSignature != zcMgr::GfxRender.mRootSignatureDefault )
	{
		mRootSignature	= zcMgr::GfxRender.mRootSignatureDefault;
		mrPSO			= nullptr;
		mrCommandList->SetGraphicsRootSignature( mRootSignature.Get() );
	}

	//----------------------------------------------------------------------------------------------
	// Pipeline State Object
	//----------------------------------------------------------------------------------------------
	if( mrPSO != _Drawcall.mrPSO )
	{
		mrPSO = _Drawcall.mrPSO;
		mrCommandList->SetPipelineState( mrPSO->GetPSO() );		
	}

	//----------------------------------------------------------------------------------------------
	// ...
	//----------------------------------------------------------------------------------------------
	UpdateStateRenderpass(_Drawcall);

	//----------------------------------------------------------------------------------------------
	// Primitive Topology (point, triangle strip, line list, ...)
	//----------------------------------------------------------------------------------------------
	mrCommandList->IASetIndexBuffer( &rIndex.HAL()->mResource.mView );	//! @todo 3 Perf : Add check if it changed?
	if( mePrimitive != rIndex.HAL()->mePrimitive )
	{
		mePrimitive = rIndex.HAL()->mePrimitive;
		mrCommandList->IASetPrimitiveTopology( mePrimitive );		
	}
	
	//----------------------------------------------------------------------------------------------
	// Per drawcall scissor setting
	//----------------------------------------------------------------------------------------------
	zVec4U16 vScreenScissor				= _Drawcall.mvScreenScissor;
	const CD3DX12_VIEWPORT& Viewport	= rView.HAL()->mViewport;
	vScreenScissor.x					= zenMath::Max<zU16>(vScreenScissor.x, (zU16)Viewport.TopLeftX );
	vScreenScissor.y					= zenMath::Max<zU16>(vScreenScissor.y, (zU16)Viewport.TopLeftY );
	vScreenScissor.z					= zenMath::Min<zU16>(vScreenScissor.z, (zU16)(Viewport.Width + Viewport.TopLeftX));
	vScreenScissor.w					= zenMath::Min<zU16>(vScreenScissor.w, (zU16)(Viewport.Height + Viewport.TopLeftY));
	if( mvScreenScissor != vScreenScissor )
	{
		D3D12_RECT ScissorRect	= {vScreenScissor.x, vScreenScissor.y, vScreenScissor.z, vScreenScissor.w};
		mrCommandList->RSSetScissorRects(1, &ScissorRect);
	}

	//----------------------------------------------------------------------------------------------
	// Assign Shader input resources for each stage
	//----------------------------------------------------------------------------------------------	
	mrCommandList->SetGraphicsRoot32BitConstant(0, pMeshStripHAL->muVertexFirst, 0);	// First entry in signature root is VertexId offset to read data

	//! @todo 0 remove all of this hacked code for resource descriptors
	// Make this setup to be done by RootSignature object, that would be aware of slot assignments
	for( zUInt idxShader(0); idxShader < keShaderStage__Count; ++idxShader )
	{
		// Assign Textures/Buffer
		zUInt uDescCount(pMeshStripHAL->marDescriptorResources[idxShader].size());
		if( uDescCount )
		{
			DescriptorRangeSRV SRVDesc	= zcMgr::GfxRender.GetFrameDescriptorSRV( uDescCount );
			for( zUInt idxRes(0); idxRes < uDescCount; ++idxRes )
			{
				const zcRes::GfxShaderResourceRef& rResource	= pMeshStripHAL->marDescriptorResources[idxShader][idxRes];
				const zcGfx::DescriptorRangeSRV* const pDesc	= GetResourceDescriptor( rResource );
				mrDevice->CopyDescriptorsSimple(1, SRVDesc.GetCpu(idxRes), pDesc ? pDesc->GetCpu() : gNullSRVView.GetCpu(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			}			
			mrCommandList->SetGraphicsRootDescriptorTable(1+static_cast<UINT>(idxShader)*3+D3D12_DESCRIPTOR_RANGE_TYPE_SRV, SRVDesc.GetGpu() );
		}
		else
		{
			DescriptorRangeSRV SRVDesc	= zcMgr::GfxRender.GetFrameDescriptorSRV( 1 );
			mrDevice->CopyDescriptorsSimple(1, SRVDesc.GetCpu(), gNullSRVView.GetCpu(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			mrCommandList->SetGraphicsRootDescriptorTable(1+static_cast<UINT>(idxShader)*3+D3D12_DESCRIPTOR_RANGE_TYPE_SRV, SRVDesc.GetGpu() );
		}
		
		// Assign UAV
		DescriptorRangeSRV UAVDesc	= zcMgr::GfxRender.GetFrameDescriptorSRV( 8 ); //! @todo 0 use constants
 		for( zUInt idxRes(0); idxRes < 8; ++idxRes )
 			mrDevice->CopyDescriptorsSimple(1, UAVDesc.GetCpu(idxRes), gNullUAVView.GetCpu(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		mrCommandList->SetGraphicsRootDescriptorTable(1+static_cast<UINT>(idxShader)*3+D3D12_DESCRIPTOR_RANGE_TYPE_UAV, UAVDesc.GetGpu() );		

		// Assign Constant Buffers
		DescriptorRangeSRV CBVDesc	= zcMgr::GfxRender.GetFrameDescriptorSRV( 10 ); //! @todo 0 use constants
		const zArrayDyn<zcRes::GfxShaderResourceDescRef>& arCBufferRes		= pMeshStripHAL->marShaderResources[idxShader][keShaderRes_CBuffer];
		const zUInt countCBuffer											= arCBufferRes.size();
 		for( zUInt idxCBuffer(0); idxCBuffer < 10; ++idxCBuffer )
		{
			zcGfx::DescriptorRangeSRV* pCBufferDesc(nullptr);			
			if(idxCBuffer < countCBuffer) 
			{
				zcRes::GfxCBufferRef rCBufferRes = arCBufferRes[idxCBuffer];
				if( rCBufferRes.IsValid() )
				{
					rCBufferRes.HAL()->Update( mrCommandList ); //! @todo 2 safety (not multithread safe)
					pCBufferDesc = GetResourceDescriptor( arCBufferRes[idxCBuffer] );
				}
			}
 			mrDevice->CopyDescriptorsSimple(1, CBVDesc.GetCpu(idxCBuffer), pCBufferDesc ? pCBufferDesc->GetCpu() : gNullCBVView.GetCpu(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		}
		mrCommandList->SetGraphicsRootDescriptorTable(1+static_cast<UINT>(idxShader)*3+D3D12_DESCRIPTOR_RANGE_TYPE_CBV, CBVDesc.GetGpu() );
	}
}

} 
