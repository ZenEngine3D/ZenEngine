#pragma once

namespace zcGfx
{

class ScopedDrawlist_DX12 : public ScopedDrawlist_Base
{
zenClassDeclare(ScopedDrawlist_DX12, ScopedDrawlist_Base)
public:
	//! @todo 1 replace wanted state with actual barrier value?
	struct BarrierCheck
	{
		BarrierCheck(){};
		BarrierCheck(const BarrierCheck& _Copy) : mpResource(_Copy.mpResource), meWantedState(_Copy.meWantedState) {}
		BarrierCheck(GpuMemoryResource* _pResource, D3D12_RESOURCE_STATES _WantedState) : mpResource(_pResource), meWantedState(_WantedState)
		{
			if( (zUInt)mpResource < 0x100 )
				printf("oups");
		}		
		GpuMemoryResource*				mpResource		= nullptr;
		D3D12_RESOURCE_STATES			meWantedState	= D3D12_RESOURCE_STATE_COMMON;
	};
	void								AddBarrierCheck(bool _bPreDataUpdate, const BarrierCheck& _WantedState);
	const zArrayDynamic<BarrierCheck>&	GetBarrierCheck(bool _bPreDataUpdate) const{ return maBarrierCheck[_bPreDataUpdate]; }

protected:
										ScopedDrawlist_DX12(const zStringHash32& _zContextName, ScopedDrawlist* _pParent, const zcRes::GfxRenderPassRef& _rRenderpass);
	zArrayDynamic<BarrierCheck>			maBarrierCheck[2];	//!< List of Barriers check needed 0:PreUpdate 1:PostUpdate
};

}