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
	using 				GfxIndexHAL::Lock;		// Expose HAL method
	using 				GfxIndexHAL::Unlock;	// Expose HAL method
};

}

#endif
