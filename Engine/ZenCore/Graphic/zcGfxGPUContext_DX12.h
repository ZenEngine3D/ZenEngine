#pragma once

namespace zcGfx
{

class GPUContext_DX12 : public GPUContext_Base
{
zenClassDeclare(GPUContext_DX12, GPUContext_Base)
public:
									
	void										Reset(const DirectXComRef<ID3D12Device>& _rDevice, const DirectXComRef<ID3D12GraphicsCommandList>& _rCommandList );
	void										UpdateState(const zcGfx::CommandDraw& _Drawcall);
	zenInline const DirectXComRef<ID3D12GraphicsCommandList>& GetCommandList()const;
	
//protected:		@todo 0 make this protected
	zenInline void								UpdateStateRenderpass(const zcGfx::CommandDraw& _Drawcall);
	zenInline void								UpdateShaderState_Samplers(const zcGfx::CommandDraw& _Drawcall, eShaderStage _eShaderStage);
	zenInline void								UpdateShaderState_ConstantBuffers(const zcGfx::CommandDraw& _Drawcall, eShaderStage _eShaderStage);
	zenInline void								UpdateShaderState_Textures(zU16& Out_ChangedFirst, zU16& Out_ChangedLast, const zcGfx::CommandDraw& _Drawcall, eShaderStage _eShaderStage);	
	zenInline void								UpdateShaderState_StructBuffers(zU16& Out_ChangedFirst, zU16& Out_ChangedLast, const zcGfx::CommandDraw& _Drawcall, eShaderStage _eShaderStage);
	
	DirectXComRef<ID3D12Device>					mrDevice;
	DirectXComRef<ID3D12GraphicsCommandList>	mrCommandList;
	zcGfx::RootSignature						mRootSignature;
	//ID3D11Device*					mpDevice			= nullptr;
	//ID3D11DeviceContext*			mpDeviceContext		= nullptr;

	// Info on input shaders resources
	/*
	zHash32							mahShaderInputStamp[keShaderStage__Count][keShaderRes__Count];				//!< Hash of assigned resources per stage/restype, to quickly know if something has changed
	zU16							maShaderInputSlotCount[keShaderStage__Count][keShaderRes__Count];			//!< Slot count to last valid Resource view per resource type
	ID3D11ShaderResourceView*		maResourceView[keShaderStage__Count][zcExp::kuDX11_ResourcesPerStageMax];	//!< Resource view of all assigned resources (textures, structbuffer, uav, ...)
	eShaderResource					maResourceType[keShaderStage__Count][zcExp::kuDX11_ResourcesPerStageMax];	//!< Resource type assigned to matching resourceview slot
		
	D3D11_PRIMITIVE_TOPOLOGY		mePrimitiveType		= D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
	zVec4U16						mvScreenScissor		= zVec4U16(0, 0, 0, 0);
	bool							mbScreenScissorOn	= false;

	// Useful for debugging/tracking but not needed
	zcRes::GfxShaderAnyRef			marShader[keShaderStage__Count];
	zcRes::GfxSamplerRef			marSampler[keShaderStage__Count][zcExp::kuDX11_SamplerPerStageMax];
	zcRes::GfxCBufferRef			marCBuffer[keShaderStage__Count][zcExp::kuDX11_CBufferPerStageMax];
	zcRes::GfxShaderResourceRef		marResource[keShaderStage__Count][zcExp::kuDX11_ResourcesPerStageMax];		
	*/
};

}

#include "zcGfxGPUContext_DX12.inl"
