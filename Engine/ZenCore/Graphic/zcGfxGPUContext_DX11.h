#pragma once

namespace zcGfx
{

class GPUContext_DX11 : public GPUContext_Base
{
zenClassDeclare(GPUContext_DX11, GPUContext_Base)
public:
									
	void							Reset(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	void							UpdateState(const CommandDraw& _Drawcall);
	zenInline ID3D11DeviceContext*	GetDeviceContext()const;
	
protected:		
	zenInline void					UpdateStateRenderpass(const zcGfx::CommandDraw& _Drawcall);
	zenInline void					UpdateShaderState_Samplers(const zcGfx::CommandDraw& _Drawcall, eShaderStage _eShaderStage);
	zenInline void					UpdateShaderState_ConstantBuffers(const zcGfx::CommandDraw& _Drawcall, eShaderStage _eShaderStage);
	zenInline void					UpdateShaderState_Textures(zU16& Out_ChangedFirst, zU16& Out_ChangedLast, const zcGfx::CommandDraw& _Drawcall, eShaderStage _eShaderStage);	
	zenInline void					UpdateShaderState_StructBuffers(zU16& Out_ChangedFirst, zU16& Out_ChangedLast, const zcGfx::CommandDraw& _Drawcall, eShaderStage _eShaderStage);

	ID3D11Device*					mpDevice			= nullptr;
	ID3D11DeviceContext*			mpDeviceContext		= nullptr;

	// Info on input shaders resources
	zHash32							mahShaderInputStamp[keShaderStage__Count][keShaderRes__Count];				//!< Hash of assigned resources per stage/restype, to quickly know if something has changed
	zU16							maShaderInputSlotCount[keShaderStage__Count][keShaderRes__Count];			//!< Slot count to last valid Resource view per resource type
	ID3D11ShaderResourceView*		maResourceView[keShaderStage__Count][zcExp::kuDX11_ResourcesPerStageMax];	//!< Resource view of all assigned resources (textures, structbuffer, uav, ...)
	eShaderResource					maResourceType[keShaderStage__Count][zcExp::kuDX11_ResourcesPerStageMax];	//!< Resource type assigned to matching resourceview slot
		
	D3D11_PRIMITIVE_TOPOLOGY		mePrimitiveType		= D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
	zVec4U16						mvScreenScissor		= zVec4U16(0, 0, 0, 0);
	bool							mbScreenScissorOn	= false;

	// Useful for debugging/tracking but not needed
	zcRes::GfxShaderAnyRef			marShader[keShaderStage__Count];
	zcRes::GfxStateSamplerRef		marSampler[keShaderStage__Count][zcExp::kuDX11_SamplerPerStageMax];
	zcRes::GfxCBufferRef			marCBuffer[keShaderStage__Count][zcExp::kuDX11_CBufferPerStageMax];
	zcRes::GfxShaderResourceRef		marResource[keShaderStage__Count][zcExp::kuDX11_ResourcesPerStageMax];		
};

}

#include "zcGfxGPUContext_DX11.inl"
