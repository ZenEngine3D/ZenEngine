#pragma once
#ifndef __zCore_Res_Gfx_GfxIndex_h__
#define __zCore_Res_Gfx_GfxIndex_h__

#include ZENHeaderRenderer(zcResGfxIndex)

namespace zcRes
{

class GfxIndex : public GfxIndex_Platform
{
ZENClassDeclare(GfxIndex, GfxIndex_Platform)
public:
	zU8*				Lock();
	void				Unlock(const zenGfx::zContext& _rContext);

protected:	
	zU8*				mpLockData = nullptr;
	zEngineRef<GfxIndex_Data> mData;
};

}


#endif
