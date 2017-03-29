#include "zcCore.h"

namespace zcGfx
{

void GPUContext_DX11::Reset(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{	
	Super::Reset();
	zenAssert(_pDevice);
	zenAssert(_pDeviceContext);
	mpDevice		= _pDevice;
	mpDeviceContext = _pDeviceContext;

	zenMem::Zero(mahShaderInputStamp);	
	zenMem::Zero(maShaderInputSlotCount);
	zenMem::Zero(maResourceView);
	zenMem::Zero(maResourceType);

	// Useful for debugging/tracking but not needed
	for( zUInt stageIdx(0); stageIdx<keShaderStage__Count; ++stageIdx)
	{
		marShader[stageIdx] = nullptr;
		for(zUInt samplerIdx(0); samplerIdx<zcExp::kuDX11_SamplerPerStageMax; ++samplerIdx)
			marSampler[stageIdx][samplerIdx] = nullptr;
		for(zUInt cbufferIdx(0); cbufferIdx<zcExp::kuDX11_CBufferPerStageMax; ++cbufferIdx)
			marCBuffer[stageIdx][cbufferIdx] = nullptr;
		for(zUInt resIdx(0); resIdx<zcExp::kuDX11_ResourcesPerStageMax; ++resIdx)
			marResource[stageIdx][resIdx];		
	}
}

//==================================================================================================
//! @details Compare current bound samplers with wanted one and update their binding if different
//==================================================================================================
void GPUContext_DX11::UpdateShaderState_Samplers( const zcGfx::CommandDraw& _Drawcall, eShaderStage _eShaderStage )
{	
	ID3D11SamplerState*	aResourceView[zcExp::kuDX11_SamplerPerStageMax];
	const eShaderResource kShaderRes							= keShaderRes_Sampler;	
	const zArrayStatic<zcRes::GfxShaderResourceRef>& arResource	= _Drawcall.mrMeshStrip.HAL()->marShaderResources[_eShaderStage][kShaderRes];
	
	// Retrieve samplers new slots assignment
	zU16 uChangedFirst(0xFFFF), uChangedLast(0), uValidCount(0), uResCount(static_cast<zU16>(arResource.Count()));
	zU16 uAssignCount = zenMath::Max(uResCount, maShaderInputSlotCount[_eShaderStage][kShaderRes]);
	for( zU16 slotIdx(0); slotIdx<uAssignCount; ++slotIdx )
	{
		marSampler[_eShaderStage][slotIdx]	= slotIdx < uResCount ? arResource[slotIdx] : nullptr;
		zcRes::GfxStateSamplerRef& rResource				= marSampler[_eShaderStage][slotIdx];
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
}

//==================================================================================================
//! @details Compare current bound Constant Buffers with wanted one and update their binding if different
//==================================================================================================
void GPUContext_DX11::UpdateShaderState_ConstantBuffers( const zcGfx::CommandDraw& _Drawcall, eShaderStage _eShaderStage )
{	
	ID3D11Buffer* aResourceView[zcExp::kuDX11_CBufferPerStageMax];
	const eShaderResource kShaderRes							= keShaderRes_CBuffer;	
	const zArrayStatic<zcRes::GfxShaderResourceRef>& arResource	= _Drawcall.mrMeshStrip.HAL()->marShaderResources[_eShaderStage][kShaderRes];
	
	// Retrieve samplers new slots assignment
	zU16 uChangedFirst(0xFFFF), uChangedLast(0), uValidCount(0), uSlotCount(static_cast<zU16>(arResource.Count()));
	zU16 uAssignCount = zenMath::Max(uSlotCount, maShaderInputSlotCount[_eShaderStage][kShaderRes]);
	for( zU16 slotIdx(0); slotIdx<uAssignCount; ++slotIdx )
	{
		marCBuffer[_eShaderStage][slotIdx]	= slotIdx < uSlotCount ? arResource[slotIdx] : nullptr;
		zcRes::GfxCBufferRef& rResource		= marCBuffer[_eShaderStage][slotIdx];
		ID3D11Buffer* pDXView				= rResource.IsValid() ? rResource.HAL()->mpBufferBinding : nullptr;		
		uChangedFirst						= pDXView != aResourceView[slotIdx]	? zenMath::Min(uChangedFirst, slotIdx)	: uChangedFirst;
		uChangedLast						= pDXView != aResourceView[slotIdx]	? slotIdx								: uChangedLast;
		uValidCount							= pDXView							? slotIdx+1								: uValidCount;
		aResourceView[slotIdx]				= pDXView;
		
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
}

//==================================================================================================
//! @details Compare current bound textures with wanted one and update their binding if different
//==================================================================================================
void GPUContext_DX11::UpdateShaderState_Textures( zU16& Out_ChangedFirst, zU16& Out_ChangedLast, const zcGfx::CommandDraw& _Drawcall, eShaderStage _eShaderStage)
{				
	const eShaderResource kShaderRes							= keShaderRes_Texture;
	const zArrayStatic<zcRes::GfxShaderResourceRef>& arResource = _Drawcall.mrMeshStrip.HAL()->marShaderResources[_eShaderStage][kShaderRes];
	zU16 uValidCount											= 0;
	const zU16 uSlotCount										= static_cast<zU16>(arResource.Count());
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
}

//==================================================================================================
//! @details Compare current bound textures with wanted one and update their binding if different
//==================================================================================================
void GPUContext_DX11::UpdateShaderState_StructBuffers( zU16& Out_ChangedFirst, zU16& Out_ChangedLast, const zcGfx::CommandDraw& _Drawcall, eShaderStage _eShaderStage)
{	
	const eShaderResource kShaderRes							= keShaderRes_Buffer;
	const zArrayStatic<zcRes::GfxShaderResourceRef>& arResource = _Drawcall.mrMeshStrip.HAL()->marShaderResources[_eShaderStage][kShaderRes];
	zU16 uValidCount											= 0;
	const zU16 uSlotCount										= static_cast<zU16>(arResource.Count());
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
}

//==================================================================================================
//! @details 
//==================================================================================================
void GPUContext_DX11::UpdateStateRenderpass(const zcGfx::CommandDraw& _Drawcall)
{	
	if( mrRenderpass != _Drawcall.mrRenderPass )
	{			
		mrRenderpass		= _Drawcall.mrRenderPass;
		auto rRenderpassHAL	= mrRenderpass.HAL();
		if( mrStateRaster != rRenderpassHAL->mrStateRaster )
		{			
			mrStateRaster		= rRenderpassHAL->mrStateRaster;
			mbScreenScissorOn	= mrStateRaster.HAL()->mRasterizerDesc.ScissorEnable == TRUE;
			mpDeviceContext->RSSetState( mrStateRaster.HAL()->mpRasterizerState);
		}
		if( mrStateBlend != rRenderpassHAL->mrStateBlend )
		{			
			mrStateBlend		= rRenderpassHAL->mrStateBlend;
			mpDeviceContext->OMSetBlendState( mrStateBlend.HAL()->mpBlendState, mrStateBlend.HAL()->mafBlendFactor, mrStateBlend.HAL()->muSampleMask );
		}
		if( mrStateDepthStencil != rRenderpassHAL->mrStateDepthStencil )
		{				
			mrStateDepthStencil	= rRenderpassHAL->mrStateDepthStencil;
			mpDeviceContext->OMSetDepthStencilState( mrStateDepthStencil.HAL()->mpDepthStencilState, mrStateDepthStencil.HAL()->muStencilValue );
		}
		if( mrStateView != mrRenderpass.HAL()->mrStateView )
		{
			const zcRes::GfxViewRef& rNewView		= mrRenderpass->mrStateView;
			const zcRes::GfxViewRef& rPreviousView	= mrStateView;
			UINT maxCount							= zenMath::Max( rNewView.HAL()->muColorCount, rPreviousView.IsValid() ? rPreviousView.HAL()->muColorCount : 0);
			mrStateView								= rNewView;
			
			zcMgr::GfxRender.UnbindTextures();
			mpDeviceContext->OMSetRenderTargets(maxCount, rNewView.HAL()->mpColorViews, rNewView.HAL()->mpDepthView );
			mpDeviceContext->RSSetViewports( 1, &rNewView.HAL()->mViewport );
		}
	}
}

//==================================================================================================
//! @details Check all GPU states, and update the one that differ from current Drawcall
//==================================================================================================
void GPUContext_DX11::UpdateState(const zcGfx::CommandDraw& _Drawcall)
{
	UINT UnusedOffset = 0;
	const zcRes::GfxMeshStripRef&		rMeshStrip	= _Drawcall.mrMeshStrip;
	const zcRes::GfxIndexRef&			rIndex		= rMeshStrip.HAL()->mrIndexBuffer;
	const zcRes::GfxShaderBindingRef&	rShaderBind	= rMeshStrip.HAL()->mrShaderBinding;			
	const zcRes::GfxViewRef&			rView		= mrStateView;
	
	UpdateStateRenderpass(_Drawcall);
	if( mePrimitiveType != rIndex.HAL()->mePrimitiveType )
	{
		mePrimitiveType = rIndex.HAL()->mePrimitiveType;
		mpDeviceContext->IASetPrimitiveTopology( mePrimitiveType );
	}
	
	if( marShader[zenConst::keShaderStage_Vertex] != rShaderBind.HAL()->marShader[zenConst::keShaderStage_Vertex] )
	{
		marShader[zenConst::keShaderStage_Vertex]	= rShaderBind.HAL()->marShader[zenConst::keShaderStage_Vertex];
		zcRes::GfxShaderVertexRef rShaderVertex		= marShader[zenConst::keShaderStage_Vertex];
		mpDeviceContext->VSSetShader( rShaderVertex.HAL()->mpVertexShader, nullptr, 0 );
	}
	
	if( marShader[zenConst::keShaderStage_Pixel]!= rShaderBind.HAL()->marShader[zenConst::keShaderStage_Pixel] )
	{
		marShader[zenConst::keShaderStage_Pixel]	= rShaderBind.HAL()->marShader[zenConst::keShaderStage_Pixel];
		zcRes::GfxShaderPixelRef rShaderPixel		= marShader[zenConst::keShaderStage_Pixel];
		mpDeviceContext->PSSetShader( rShaderPixel.HAL()->mpPixelShader, nullptr, 0 );
	}
	

	if( mbScreenScissorOn )
	{
		zVec4U16 vScreenScissor = _Drawcall.mvScreenScissor;
		vScreenScissor.z		= zenMath::Min<zU16>(vScreenScissor.z, (zU16)rView.HAL()->mViewport.Width+vScreenScissor.x);
		vScreenScissor.w		= zenMath::Min<zU16>(vScreenScissor.w, (zU16)rView.HAL()->mViewport.Height+vScreenScissor.y);
		if( mvScreenScissor != vScreenScissor )
		{
			D3D11_RECT ScissorRect	= {vScreenScissor.x, vScreenScissor.y, vScreenScissor.z, vScreenScissor.w};
			mvScreenScissor			= vScreenScissor;
			mpDeviceContext->RSSetScissorRects(1, &ScissorRect);
		}
	}
	mpDeviceContext->IASetIndexBuffer	( rIndex.HAL()->mpIndiceBuffer, rIndex.HAL()->meIndiceFormat, 0 );	

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

			bUpdated[resTypeIdx]								= mahShaderInputStamp[stageIdx][resTypeIdx] != zMeshStripResStamp;
			mahShaderInputStamp[stageIdx][resTypeIdx]	= zMeshStripResStamp;
		}
		
		// Special case shader resources update
		if( bUpdated[keShaderRes_Sampler] )
			UpdateShaderState_Samplers(_Drawcall, eStageIdx);
		if( bUpdated[keShaderRes_CBuffer] )
			UpdateShaderState_ConstantBuffers(_Drawcall, eStageIdx);
		
		// Generic shader resources update
		zU16 uResChangeStart(0xFFFF), uResChangeEnd(0);
		if( bUpdated[keShaderRes_Buffer] )
			UpdateShaderState_StructBuffers(uResChangeStart, uResChangeEnd, _Drawcall, eStageIdx);
		
		if( bUpdated[keShaderRes_Texture] )
			UpdateShaderState_Textures(uResChangeStart, uResChangeEnd, _Drawcall, eStageIdx);
		
		if( uResChangeEnd >= uResChangeStart )
		{
			switch( eStageIdx )
			{
			case keShaderStage_Vertex:	mpDeviceContext->VSSetShaderResources( uResChangeStart, uResChangeEnd-uResChangeStart+1, &maResourceView[stageIdx][uResChangeStart] );	break;
			case keShaderStage_Pixel:	mpDeviceContext->PSSetShaderResources( uResChangeStart, uResChangeEnd-uResChangeStart+1, &maResourceView[stageIdx][uResChangeStart] );	break;
			default: break;
			}
		}
	}
}

}
