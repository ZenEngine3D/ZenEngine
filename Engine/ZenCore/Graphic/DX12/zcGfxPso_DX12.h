#pragma once

#include <unordered_map>

namespace zcGfx
{

class PSO_DX12 : public zRefCounted
{
zenClassDeclare(PSO_DX12, zRefCounted)
public:
	zenInline bool								IsCompiled(){ return mbIsCompiled; }	//!< @brief True if PSO has been built and can be assigned to CommandList
	zenInline ID3D12PipelineState*				GetPSO() { return mrDXPSO.Get(); }		//!< @brief Return PSO directx object
	static const zEngineRef<PSO_DX12>&			GetAdd(const zcRes::GfxRenderPassRef& _rRenderpass, const zcRes::GfxMeshStripRef& _rMeshStrip);	//!< @brief Request PSO for a drawcall, build PSO if not already created
	static void									StaticInitialize();						//!< @brief Iniatize PSO's map

protected:
	bool										mbIsCompiled = false;			//!< If PSO's compiling is done and can be used
	DirectXComRef<ID3D12PipelineState>			mrDXPSO;						//!< DirectX object to bind as PSO
	static zMap< zEngineRef<PSO_DX12> >::Key64	sdPSOs;									//!< Map of all built PSOs
};

}
