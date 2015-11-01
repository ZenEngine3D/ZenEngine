#pragma once
#ifndef __zCore_Res_Gfx_GfxView_h__
#define __zCore_Res_Gfx_GfxView_h__

namespace zcRes
{
	class GfxView : public TResource<GfxView, GfxViewResData, GfxViewProxy, GfxViewExporterRuntime>
	{
	ZENClassDeclare(GfxView, TResource)
	public:
		const zVec2U16&				GetDim()const;
	};

}

#endif