#pragma once

namespace zcGfx
{

class GPUContext_DX12 : public GPUContext_Base
{
zenClassDeclare(GPUContext_DX12, GPUContext_Base)
public:
	struct PendingFence { DirectXComRef<ID3D12Fence> mrFence; zU64 mValue; };				
	void										Reset(const DirectXComRef<ID3D12Device>& _rDevice, const DirectXComRef<ID3D12GraphicsCommandList>& _rCommandList, const DirectXComRef<ID3D12DescriptorHeap>& _rResViewDescHeap );
	void										UpdateState(const zcGfx::CommandDraw_HAL& _Drawcall);
	zenInline const DirectXComRef<ID3D12GraphicsCommandList>& GetCommandList()const;
	zenInline void								AddFence(const DirectXComRef<ID3D12Fence>& _rFence, zU64 mValue);
	void										FlushPendingFences(const DirectXComRef<ID3D12CommandQueue>& _rCmdQueue);
protected:
	zenInline void								UpdateStateRenderpass(const zcGfx::CommandDraw_HAL& _Drawcall);
	zenInline void								UpdateShaderState_Samplers(const zcGfx::CommandDraw_HAL& _Drawcall, eShaderStage _eShaderStage);
	zenInline void								UpdateShaderState_ConstantBuffers(const zcGfx::CommandDraw_HAL& _Drawcall, eShaderStage _eShaderStage);
	zenInline void								UpdateShaderState_Textures(zU16& Out_ChangedFirst, zU16& Out_ChangedLast, const zcGfx::CommandDraw_HAL& _Drawcall, eShaderStage _eShaderStage);	
	zenInline void								UpdateShaderState_StructBuffers(zU16& Out_ChangedFirst, zU16& Out_ChangedLast, const zcGfx::CommandDraw_HAL& _Drawcall, eShaderStage _eShaderStage);
	
	DirectXComRef<ID3D12Device>					mrDevice;
	DirectXComRef<ID3D12GraphicsCommandList>	mrCommandList;
	zArrayDynamic<D3D12_RESOURCE_BARRIER>		maPendingBarriers;
	zArrayDynamic<PendingFence>					maPendingFences;	//!< List of fences to submit alongside the CommandList
	zcGfx::RootSignature						mRootSignature;
	zEngineRef<PSO_DX12>						mrPSO;
	zVec4U16									mvScreenScissor		= zVec4U16(0, 0, 0, 0);
	D3D12_PRIMITIVE_TOPOLOGY					mePrimitive			= D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
};

}

#include "zcGfxGPUContext_DX12.inl"
