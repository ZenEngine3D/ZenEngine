#include "zcCore.h"

namespace zcGfx
{

ScopedDrawlist_DX11::ScopedDrawlist_DX11(const zStringHash32& _zContextName, ScopedDrawlist* _pParent, const zcRes::GfxRenderPassRef& _rRenderpass)
: Super(_zContextName, _pParent, _rRenderpass)
{
}

}