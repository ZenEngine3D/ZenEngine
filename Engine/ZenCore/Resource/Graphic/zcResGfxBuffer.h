#pragma once

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
