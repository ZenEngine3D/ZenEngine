#pragma once
#ifndef __zCore_Res_Gfx_GfxIndex_h__
#define __zCore_Res_Gfx_GfxIndex_h__

namespace zcRes
{

class GfxIndex : public TResource<GfxIndex, GfxIndexResData, GfxIndexProxy, GfxIndexExporterRuntime>
{
ZENClassDeclare(GfxIndex, TResource)
public:
	//! @todo urgent figure out interface for gamethread access to proxy functions
	zU8*	Lock();	
	void	Unlock();
};


}

#endif