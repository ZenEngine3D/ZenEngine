#include "zcCore.h"

namespace zcGfx
{
zMap< zEngineRef<PSO_DX12> >::Key64 PSO_DX12::sdPSOs;

void ResetPSOReference( zMap< zEngineRef<PSO_DX12> >::Key64& _dDictPso, zEngineRef<PSO_DX12>& _rPsoDel)
{
	_rPsoDel = nullptr;
}

void PSO_DX12::StaticInitialize()
{
	sdPSOs.Init( 1024 );
	sdPSOs.SetDeleteItemCB( ResetPSOReference );
}

//==================================================================================================
//! @details :	Try to find the PSO associated to a drawcall, and if not present, request its 
//!				Creation
//!-------------------------------------------------------------------------------------------------
//! @param rRenderpass	: Renderpass associated with the draw request
//! @param _rMeshstrip	: Object to render (shaders, index formats, ...)
//! @return				: The found or created PSO needed
//==================================================================================================
const zEngineRef<PSO_DX12>& PSO_DX12::GetAdd(const zcRes::GfxRenderPassRef& _rRenderpass, const zcRes::GfxMeshStripRef& _rMeshStrip)
{
	const zcRes::GfxRenderPass_HAL* pRenderpassHAL	= _rRenderpass.HAL();
	const zcRes::GfxView_HAL* pViewHAL				= pRenderpassHAL->mrStateView.HAL();
	//----------------------------------------------------------------------------------------------
	// Extract RenderTargets formats
	//----------------------------------------------------------------------------------------------
	DXGI_FORMAT aRTFormat[] = {DXGI_FORMAT_UNKNOWN,DXGI_FORMAT_UNKNOWN,DXGI_FORMAT_UNKNOWN,DXGI_FORMAT_UNKNOWN,DXGI_FORMAT_UNKNOWN,DXGI_FORMAT_UNKNOWN,DXGI_FORMAT_UNKNOWN,DXGI_FORMAT_UNKNOWN,DXGI_FORMAT_UNKNOWN};
	zenStaticAssert( zenArrayCount(aRTFormat) == D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT+1 );
	const zUInt RTCount									= pViewHAL->maRTColorConfig.size();
	const zcRes::GfxTarget2DRef& rDepthTarget			= pViewHAL->mRTDepthConfig.mrTargetSurface;
	aRTFormat[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT]	= rDepthTarget.IsValid() ? zcMgr::GfxRender.ZenFormatToNative(rDepthTarget.HAL()->meFormat) : DXGI_FORMAT_UNKNOWN;
	for( zUInt idxRT(0); idxRT<RTCount; ++idxRT)
	{
		const zcRes::GfxTarget2DRef& rTarget			= pViewHAL->maRTColorConfig[idxRT].mrTargetSurface;
		aRTFormat[idxRT]								= rTarget.IsValid() ? zcMgr::GfxRender.ZenFormatToNative(rTarget.HAL()->meFormat) : DXGI_FORMAT_UNKNOWN;
	}	
	
	//----------------------------------------------------------------------------------------------
	// Find PSO's key and see if already created
	//----------------------------------------------------------------------------------------------
	zHash64 hKey(_rMeshStrip.HAL()->mrShaderBinding.HAL()->mResID.GetHashID());
	hKey.Append( &_rMeshStrip.HAL()->mrIndexBuffer.HAL()->mePrimitiveType, sizeof(D3D12_PRIMITIVE_TOPOLOGY_TYPE) );
	hKey.Append( &pRenderpassHAL->mRasterStateID, sizeof(zResID) );
	hKey.Append( &pRenderpassHAL->mBlendStateID, sizeof(zResID) );
	hKey.Append( &pRenderpassHAL->mDepthStencilStateID, sizeof(zResID) );	
	hKey.Append( aRTFormat, sizeof(aRTFormat) );				
	zEngineRef<PSO_DX12>& rPSO = sdPSOs.GetAdd( hKey );

	//----------------------------------------------------------------------------------------------
	//! @todo 2 Implement async compiling of PSOs
	//----------------------------------------------------------------------------------------------
	if( !rPSO.IsValid() )
	{
		// Describe and create the graphics pipeline state object (PSO).	
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PsoDesc = {};
		PSO_DX12* pNewPSO						= zenMem::New<PSO_DX12>();
		PsoDesc.InputLayout						= { nullptr, 0 };
		PsoDesc.pRootSignature					= zcMgr::GfxRender.mRootSignatureDefault.Get();		
		PsoDesc.RasterizerState					= pRenderpassHAL->mrStateRaster.HAL()->mRasterizerDesc;
		PsoDesc.BlendState						= pRenderpassHAL->mrStateBlend.HAL()->mBlendDesc;
		PsoDesc.DepthStencilState				= pRenderpassHAL->mrStateDepthStencil.HAL()->mDepthStencilDesc;
		PsoDesc.SampleMask						= UINT_MAX;
		PsoDesc.PrimitiveTopologyType			= _rMeshStrip.HAL()->mrIndexBuffer.HAL()->mePrimitiveType;
		PsoDesc.SampleDesc.Count				= 1;
		PsoDesc.NumRenderTargets				= (UINT)RTCount;
		PsoDesc.DSVFormat						= aRTFormat[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT];
		
		// Output formats
		for( zUInt idxRT(0); idxRT<RTCount; ++idxRT)
			PsoDesc.RTVFormats[idxRT]			= aRTFormat[idxRT];
		
		// Shaders
		if( _rMeshStrip.HAL()->mrShaderBinding.HAL()->marShader[keShaderStage_Vertex].IsValid() )
		{
			zcRes::GfxShaderVertexRef rShader	= _rMeshStrip.HAL()->mrShaderBinding.HAL()->marShader[keShaderStage_Vertex];
			PsoDesc.VS							= rShader.HAL()->mDXShaderCode;
		}

		if( _rMeshStrip.HAL()->mrShaderBinding.HAL()->marShader[keShaderStage_Pixel].IsValid() )
		{
			zcRes::GfxShaderPixelRef rShader	= _rMeshStrip.HAL()->mrShaderBinding.HAL()->marShader[keShaderStage_Pixel];
			PsoDesc.PS							= rShader.HAL()->mDXShaderCode;
		}
		
		HRESULT hr								= zcMgr::GfxRender.GetDevice()->CreateGraphicsPipelineState(&PsoDesc, IID_PPV_ARGS(&pNewPSO->mrDXPSO));
		rPSO									= pNewPSO;
		pNewPSO->mbIsCompiled					= SUCCEEDED(hr);
	}
	return rPSO;
}

}
