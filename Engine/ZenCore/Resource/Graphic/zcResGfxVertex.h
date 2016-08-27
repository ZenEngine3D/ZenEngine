#pragma once
#ifndef __zCore_Res_Gfx_GfxVertex_h__
#define __zCore_Res_Gfx_GfxVertex_h__

#include ZENHeaderRenderer(zcResGfxVertex)

namespace zcRes
{

class GfxVertex : protected GfxVertexHAL
{
zenClassDeclare(GfxVertex, GfxVertexHAL)
public:
	zU8*				Lock(); //! todo urgent specify which stream
	void				Unlock(const zenGfx::zContext& _rContext);		

	using				GfxVertexHAL::Update;	// Expose HAL method
protected:	
	zU8*				mpLockData = nullptr; //! @todo safe support multi threading (lock return context?)
};

}

#endif
