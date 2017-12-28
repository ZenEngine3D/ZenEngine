#pragma once

namespace zcGfx
{

class CommandList_DX11 : public CommandList_Base
{
zenClassDeclare(CommandList_DX11, CommandList_Base)
protected:
		CommandList_DX11(const zStringHash32& _zContextName, const CommandListRef& _rParent, const zcRes::GfxRenderPassRef& _rRenderpass);
};

}
