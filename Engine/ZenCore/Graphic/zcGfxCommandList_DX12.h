#pragma once

namespace zcGfx
{

class CommandList_DX12 : public CommandList_Base
{
zenClassDeclare(CommandList_DX12, CommandList_Base)
public:
	//! @todo 1 replace wanted state with actual barrier value?
	struct BarrierCheck
	{
		BarrierCheck(){};
		BarrierCheck(const BarrierCheck& _Copy) : mpResource(_Copy.mpResource), meWantedState(_Copy.meWantedState) {}
		BarrierCheck(GpuMemoryResource* _pResource, D3D12_RESOURCE_STATES _WantedState) : mpResource(_pResource), meWantedState(_WantedState){}		
		GpuMemoryResource*				mpResource		= nullptr;
		D3D12_RESOURCE_STATES			meWantedState	= D3D12_RESOURCE_STATE_COMMON;
	};
	void								AddBarrierCheck(bool _bPreDataUpdate, const BarrierCheck& _WantedState);
	const zArrayDyn<BarrierCheck>&	GetBarrierCheck(bool _bPreDataUpdate) const{ return maBarrierCheck[_bPreDataUpdate]; }

protected:
										CommandList_DX12(const zStringHash32& _zContextName, const CommandListRef& _rParent, const zcRes::GfxRenderPassRef& _rRenderpass);
	zArrayDyn<BarrierCheck>			maBarrierCheck[2];	//!< List of Barriers check needed 0:PreUpdate 1:PostUpdate
};

}