#include "zcCore.h"

namespace zcGfx
{

CommandList_DX12::CommandList_DX12(const zStringHash32& _zContextName, const CommandListRef& _rParent, const zcRes::GfxRenderPassRef& _rRenderpass)
: Super(_zContextName, _rParent, _rRenderpass)
{
	for( zUInt idx(0); idx<zenArrayCount(maBarrierCheck); ++idx)
		maBarrierCheck[idx].Reserve(64);
}

void CommandList_DX12::AddBarrierCheck(bool _bPreDataUpdate, const BarrierCheck& _WantedState)
{
	maBarrierCheck[_bPreDataUpdate].Push(_WantedState);
}

}