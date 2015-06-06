#pragma once
#ifndef __zCore_Res_Gfx_GfxView_h__
#define __zCore_Res_Gfx_GfxView_h__

namespace zcRes
{
	class GfxView : public TResource<GfxView, GfxViewResData, GfxViewProxy, GfxViewExporterRuntime>
	{
	ZENClassDeclare(GfxView, TResource)
	public:
		const zVec2S16&				GetOrigin()const;
		const zVec2U16&				GetDim()const;
		void						Clear( bool _bClearColor, const zVec4F& _vRGBA, bool _bClearDepth=true, float _fDepth=1, bool _bClearStencil=false, zU8 _uStencil=0 );
	};

}

#endif