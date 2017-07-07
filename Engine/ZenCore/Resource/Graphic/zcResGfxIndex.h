#pragma once

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
