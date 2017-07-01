#pragma once
#ifndef __zCore_Res_Gfx_Renderpass_h__
#define __zCore_Res_Gfx_Renderpass_h__

#include zenHeaderRenderer( zcResGfxRenderpass )

namespace zcRes
{
	//! @todo change this to protected
	class GfxRenderPass : public GfxRenderPass_HAL
	{
	zenClassDeclare(GfxRenderPass, GfxRenderPass_HAL)
	};

}

#endif
