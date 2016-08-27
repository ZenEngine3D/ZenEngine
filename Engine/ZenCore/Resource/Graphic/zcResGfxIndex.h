#pragma once
#ifndef __zCore_Res_Gfx_GfxIndex_h__
#define __zCore_Res_Gfx_GfxIndex_h__

#include ZENHeaderRenderer(zcResGfxIndex)

namespace zcRes
{

class GfxIndex : protected GfxIndexHAL
{
zenClassDeclare(GfxIndex, GfxIndexHAL)
public:
	zU8*				Lock();
	void				Unlock(const zenGfx::zContext& _rContext);		

	using				GfxIndexHAL::Update;	// Expose HAL method
protected:	
	zU8*				mpLockData = nullptr; //! @todo safe support multi threading (lock return context?)
};

}

#endif
