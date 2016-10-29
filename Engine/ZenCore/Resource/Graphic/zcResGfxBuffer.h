#pragma once
#ifndef __zCore_Res_Gfx_GfxBuffer_h__
#define __zCore_Res_Gfx_GfxBuffer_h__

#include ZENHeaderRenderer(zcResGfxBuffer)

namespace zcRes
{

class GfxBuffer : protected GfxBufferHAL
{
zenClassDeclare(GfxBuffer, GfxBufferHAL)
public:	
	void*				Lock();
	void				Unlock(const zenGfx::zContext& _rContext);		
	using				GfxBufferHAL::Update;	// Expose HAL method
protected:	
	zU8*				mpLockData = nullptr; //! @todo safe support multi threading (lock return context?)
};

}

#endif
