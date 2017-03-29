#pragma once

namespace zcGfx
{

class ScopedDrawlist_DX11 : public ScopedDrawlist_Base
{
zenClassDeclare(ScopedDrawlist_DX11, ScopedDrawlist_Base)
protected:
		ScopedDrawlist_DX11(const zStringHash32& _zContextName, ScopedDrawlist* _pParent, const zcRes::GfxRenderPassRef& _rRenderpass);
};

}
