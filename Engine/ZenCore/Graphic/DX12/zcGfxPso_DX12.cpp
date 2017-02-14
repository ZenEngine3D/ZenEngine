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

const zEngineRef<PSO_DX12>& PSO_DX12::GetAdd(const zcRes::GfxRenderPassRef& _rRenderpass, const zcRes::GfxMeshStripRef& _rMeshStrip)
{
	const zcRes::GfxRenderPass_HAL* pRenderpassHAL	= _rRenderpass.HAL();
	const zcRes::GfxView_HAL* pViewHAL				= pRenderpassHAL->mrStateView.HAL();
	
	// Extract RenderTargets formats
	DXGI_FORMAT aRTFormat[] = {DXGI_FORMAT_UNKNOWN,DXGI_FORMAT_UNKNOWN,DXGI_FORMAT_UNKNOWN,DXGI_FORMAT_UNKNOWN,DXGI_FORMAT_UNKNOWN,DXGI_FORMAT_UNKNOWN,DXGI_FORMAT_UNKNOWN,DXGI_FORMAT_UNKNOWN,DXGI_FORMAT_UNKNOWN};
	zenStaticAssert( zenArrayCount(aRTFormat) == D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT+1 );
	const zUInt RTCount								= pViewHAL->maRTColorConfig.Count();
	for( zUInt idxRT(0); idxRT<RTCount; ++idxRT)
	{
		const zcRes::GfxTarget2DRef& rTarget = pViewHAL->maRTColorConfig[idxRT].mrTargetSurface;
		aRTFormat[idxRT] = rTarget.IsValid() ? zcMgr::GfxRender.ZenFormatToNative(rTarget.HAL()->meFormat) : DXGI_FORMAT_UNKNOWN;
	}
	const zcRes::GfxTarget2DRef& rDepthTarget = pViewHAL->mRTDepthConfig.mrTargetSurface;
	aRTFormat[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT] = rDepthTarget.IsValid() ? zcMgr::GfxRender.ZenFormatToNative(rDepthTarget.HAL()->meFormat) : DXGI_FORMAT_UNKNOWN;

	// Find PSO's key based on its settings
	zHash64 hKey(_rMeshStrip.HAL()->mrShaderBinding.HAL()->mResID.GetHashID());
	hKey.Append( &_rMeshStrip.HAL()->mrIndexBuffer.HAL()->mePrimitiveType, sizeof(D3D12_PRIMITIVE_TOPOLOGY_TYPE) );
	hKey.Append( &pRenderpassHAL->mRasterStateID, sizeof(zResID) );
	hKey.Append( &pRenderpassHAL->mBlendStateID, sizeof(zResID) );
	hKey.Append( &pRenderpassHAL->mDepthStencilStateID, sizeof(zResID) );	
	hKey.Append( aRTFormat, sizeof(aRTFormat) );
				
	zEngineRef<PSO_DX12>& rPSO = sdPSOs.GetAdd( hKey );

	//! @todo 2 Implement async compiling of PSOs
	if( !rPSO.IsValid() )
	{
		PSO_DX12* pNewPSO = zenNewDefault PSO_DX12();
		// Describe and create the graphics pipeline state object (PSO).
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PsoDesc = {};
		PsoDesc.InputLayout						= { inputElementDescs, _countof(inputElementDescs) };
		PsoDesc.pRootSignature					= zcMgr::GfxRender.mRootSignatureDefault.Get();
		PsoDesc.VS								= zcMgr::GfxRender.mrTmpShaderVS.HAL()->mDXShaderCode;
		PsoDesc.PS								= zcMgr::GfxRender.mrTmpShaderPS.HAL()->mDXShaderCode;
		PsoDesc.RasterizerState					= pRenderpassHAL->mrStateRaster.HAL()->mRasterizerDesc;
		PsoDesc.BlendState						= pRenderpassHAL->mrStateBlend.HAL()->mBlendDesc;
		PsoDesc.DepthStencilState				= pRenderpassHAL->mrStateDepthStencil.HAL()->mDepthStencilDesc;
		PsoDesc.SampleMask						= UINT_MAX;
		PsoDesc.PrimitiveTopologyType			= _rMeshStrip.HAL()->mrIndexBuffer.HAL()->mePrimitiveType;
		PsoDesc.SampleDesc.Count				= 1;
		PsoDesc.NumRenderTargets				= (UINT)RTCount;
		PsoDesc.DSVFormat						= aRTFormat[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT];
		for( zUInt idxRT(0); idxRT<RTCount; ++idxRT)
			PsoDesc.RTVFormats[idxRT]			= aRTFormat[idxRT];
		
		HRESULT hr								= zcMgr::GfxRender.GetDevice()->CreateGraphicsPipelineState(&PsoDesc, IID_PPV_ARGS(&pNewPSO->mrDXPSO));
		rPSO									= pNewPSO;
		pNewPSO->mbIsCompiled					= SUCCEEDED(hr);
	}
	return rPSO;
}

}
