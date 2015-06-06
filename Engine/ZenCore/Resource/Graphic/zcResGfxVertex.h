#pragma once
#ifndef __zCore_Res_Gfx_GfxVertex_h__
#define __zCore_Res_Gfx_GfxVertex_h__

namespace zcRes
{

class GfxVertex : public TResource<GfxVertex, GfxVertexResData, GfxVertexProxy, GfxVertexExporterRuntime>
{
ZENClassDeclare(GfxVertex, TResource)
public:
	//! @todo urgent figure out interface for gamethread access to proxy functions
	zU8*	Lock();
	void	Unlock();
};

}

#endif