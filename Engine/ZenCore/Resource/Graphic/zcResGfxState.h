#pragma once
#ifndef __zCore_Res_Gfx_State_h__
#define __zCore_Res_Gfx_State_h__

#include zenHeaderRenderer(zcResGfxState)

namespace zcRes
{
	class GfxStateSampler : protected GfxStateSampler_HAL
	{
	zenClassDeclare(GfxStateSampler, GfxStateSampler_HAL)
	};

	class GfxStateBlend : protected GfxStateBlend_HAL
	{
	zenClassDeclare(GfxStateBlend, GfxStateBlend_HAL)		
	};

	class GfxStateDepthStencil : protected GfxStateDepthStencil_HAL
	{
	zenClassDeclare(GfxStateDepthStencil, GfxStateDepthStencil_HAL)		
	};

	class GfxStateRaster : protected GfxStateRaster_HAL
	{
	zenClassDeclare(GfxStateRaster, GfxStateRaster_HAL)		
	};
}

#endif
