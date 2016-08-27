#pragma once
#ifndef __zCore_Res_Gfx_State_h__
#define __zCore_Res_Gfx_State_h__

#include ZENHeaderRenderer(zcResGfxState)

namespace zcRes
{
	class GfxSampler : protected GfxSamplerHAL
	{
	zenClassDeclare(GfxSampler, GfxSamplerHAL)
	};

	class GfxStateBlend : protected GfxStateBlendHAL
	{
	zenClassDeclare(GfxStateBlend, GfxStateBlendHAL)		
	};

	class GfxStateDepthStencil : protected GfxStateDepthStencilHAL
	{
	zenClassDeclare(GfxStateDepthStencil, GfxStateDepthStencilHAL)		
	};

	class GfxStateRaster : protected GfxStateRasterHAL
	{
	zenClassDeclare(GfxStateRaster, GfxStateRasterHAL)		
	};
}

#endif
