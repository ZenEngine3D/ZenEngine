#pragma once
#ifndef __zCore_Res_Gfx_GfxView_DX11_h__
#define __zCore_Res_Gfx_GfxView_DX11_h__

namespace zcRes
{
	class GfxViewProxy_DX11 : public zRefCountedAutoDel
	{
	ZENClassDeclare(GfxViewProxy_DX11, zRefCountedAutoDel)
	public:
												GfxViewProxy_DX11();
		virtual									~GfxViewProxy_DX11();												
		bool									Initialize(class GfxView& _Owner);
		void									Clear( bool _bClearColor, const zVec4F& _vRGBA, bool _bClearDepth=true, float _fDepth=1, bool _bClearStencil=false, zU8 _uStencil=0 );
		zArrayStatic<GfxRenderTargetProxyRef>	marProxTargetColor;
		GfxRenderTargetProxyRef					mrProxTargetDepth;
		ZENDbgCode(class GfxView*			mpOwner);
	};
}

#endif