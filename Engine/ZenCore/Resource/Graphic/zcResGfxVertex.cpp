#include "zcCore.h"

namespace zcRes
{
	zU8* GfxVertex::Lock()
	{	
		return mrProxy->Lock(); 
	}	

	void GfxVertex::Unlock()
	{ 
		mrProxy->Unlock(); 
	}

}
