#pragma once
#ifndef __zCore_Res_Gfx_GfxWindow_h__
#define __zCore_Res_Gfx_GfxWindow_h__

namespace zcRes
{
	class GfxWindow : public TResource<GfxWindow, GfxWindowResData, GfxWindowProxy, zcExp::ExporterNone>
	{
	ZENClassDeclare(GfxWindow, TResource)
	public:
		void						Resize(const zVec2U16& _vSize){ mrProxy->mvPendingResize	 = _vSize; } //! @todo safety make this multithread safe
		void						SetBackbuffer(zU8 _uBackbufferIndex, const GfxRenderTargetRef& _rBackbufferColor);
		const GfxRenderTargetRef&	GetBackbuffer();
	
	protected:
		GfxRenderTargetRef			mrBackbufferColor[2];
		zenRes::zGfxRenderPass		mrImGuiRndPass;
		zenRes::zGfxTexture2d		mrImGuiTexFont;
		zenRes::zGfxVertex			mrImGuiVertex;
		zenRes::zGfxIndex			mrImGuiIndex;
		zenRes::zGfxShaderVertex	mrImGuiVS;
		zenRes::zGfxShaderPixel		mrImGuiPS;
		zenRes::zGfxShaderBinding	mrImGuiShaderBind;
		zenRes::zGfxMesh			mrImGuiMesh;
		zenMath::Matrix				matUIOrthographic;
	};

}

#endif