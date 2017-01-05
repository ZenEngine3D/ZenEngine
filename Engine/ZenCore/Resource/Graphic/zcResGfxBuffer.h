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
	using	GfxBufferHAL::Lock;
	using	GfxBufferHAL::Unlock;
};

}

#endif
