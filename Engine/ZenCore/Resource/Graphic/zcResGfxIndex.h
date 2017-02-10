#pragma once
#ifndef __zCore_Res_Gfx_GfxIndex_h__
#define __zCore_Res_Gfx_GfxIndex_h__

#include zenHeaderRenderer(zcResGfxIndex)

namespace zcRes
{

class GfxIndex : protected GfxIndex_HAL
{
zenClassDeclare(GfxIndex, GfxIndex_HAL)
public:
	using 			Super::Lock;	// Expose HAL method
	using 			Super::Unlock;	// Expose HAL method
};

}

#endif
