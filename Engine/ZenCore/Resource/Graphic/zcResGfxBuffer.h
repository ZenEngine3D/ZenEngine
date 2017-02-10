#pragma once
#ifndef __zCore_Res_Gfx_GfxBuffer_h__
#define __zCore_Res_Gfx_GfxBuffer_h__

#include zenHeaderRenderer(zcResGfxBuffer)

namespace zcRes
{

class GfxBuffer : protected GfxBuffer_HAL
{
zenClassDeclare(GfxBuffer, GfxBuffer_HAL)
public:	
	using	Super::Lock;
	using	Super::Unlock;
};

}

#endif
