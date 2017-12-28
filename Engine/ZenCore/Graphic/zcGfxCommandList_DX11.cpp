#include "zcCore.h"

namespace zcGfx
{

CommandList_DX11::CommandList_DX11(const zStringHash32& _zContextName, const CommandListRef& _rParent, const zcRes::GfxRenderPassRef& _rRenderpass)
: Super(_zContextName, _rParent, _rRenderpass)
{
}

}