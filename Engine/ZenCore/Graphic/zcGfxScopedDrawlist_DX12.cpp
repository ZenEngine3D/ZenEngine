#include "zcCore.h"

namespace zcGfx
{

ScopedDrawlist_DX12::ScopedDrawlist_DX12(const zStringHash32& _zContextName, ScopedDrawlist* _pParent, const zcRes::GfxRenderPassRef& _rRenderpass)
: Super(_zContextName, _pParent, _rRenderpass)
{
	for( zUInt idx(0); idx<zenArrayCount(maBarrierCheck); ++idx)
		maBarrierCheck[idx].Reserve(64);
}

void ScopedDrawlist_DX12::AddBarrierCheck(bool _bPreDataUpdate, const BarrierCheck& _WantedState)
{
	maBarrierCheck[_bPreDataUpdate].Push(_WantedState);
}

}