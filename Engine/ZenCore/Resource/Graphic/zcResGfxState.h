#pragma once
#ifndef __zCore_Res_Gfx_State_h__
#define __zCore_Res_Gfx_State_h__

#include ZENHeaderRenderer(zcResGfxState)

namespace zcRes
{
	class GfxSampler : protected GfxSamplerHAL
	{
	ZENClassDeclare(GfxSampler, GfxSamplerHAL)
	};

	class GfxStateBlend : protected GfxStateBlendHAL
	{
	ZENClassDeclare(GfxStateBlend, GfxStateBlendHAL)		
	};

	class GfxStateDepthStencil : protected GfxStateDepthStencilHAL
	{
	ZENClassDeclare(GfxStateDepthStencil, GfxStateDepthStencilHAL)		
	};

	class GfxStateRaster : protected GfxStateRasterHAL
	{
	ZENClassDeclare(GfxStateRaster, GfxStateRasterHAL)		
	};
}

#endif
