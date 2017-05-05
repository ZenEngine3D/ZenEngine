#pragma once

namespace zcGfx
{

class GPUContext_DX12 : public GPUContext_Base
{
zenClassDeclare(GPUContext_DX12, GPUContext_Base)
public:
									
	void										Reset(const DirectXComRef<ID3D12Device>& _rDevice, const DirectXComRef<ID3D12GraphicsCommandList>& _rCommandList, const DirectXComRef<ID3D12DescriptorHeap>& _rResViewDescHeap );
	void										UpdateState(const zcGfx::CommandDraw_HAL& _Drawcall);
	zenInline const DirectXComRef<ID3D12GraphicsCommandList>& GetCommandList()const;
	
//protected:		@todo 0 make this protected
	zenInline void								UpdateStateRenderpass(const zcGfx::CommandDraw_HAL& _Drawcall);
	zenInline void								UpdateShaderState_Samplers(const zcGfx::CommandDraw_HAL& _Drawcall, eShaderStage _eShaderStage);
	zenInline void								UpdateShaderState_ConstantBuffers(const zcGfx::CommandDraw_HAL& _Drawcall, eShaderStage _eShaderStage);
	zenInline void								UpdateShaderState_Textures(zU16& Out_ChangedFirst, zU16& Out_ChangedLast, const zcGfx::CommandDraw_HAL& _Drawcall, eShaderStage _eShaderStage);	
	zenInline void								UpdateShaderState_StructBuffers(zU16& Out_ChangedFirst, zU16& Out_ChangedLast, const zcGfx::CommandDraw_HAL& _Drawcall, eShaderStage _eShaderStage);
	
	DirectXComRef<ID3D12Device>					mrDevice;
	DirectXComRef<ID3D12GraphicsCommandList>	mrCommandList;
	DirectXComRef<ID3D12DescriptorHeap>			mrResViewDescHeap;
	zArrayDynamic<D3D12_RESOURCE_BARRIER>		maPendingBarriers;	
	zcGfx::RootSignature						mRootSignature;
	zEngineRef<PSO_DX12>						mrPSO;
	zVec4U16									mvScreenScissor		= zVec4U16(0, 0, 0, 0);
	D3D12_PRIMITIVE_TOPOLOGY					mePrimitive			= D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
};

}

#include "zcGfxGPUContext_DX12.inl"
