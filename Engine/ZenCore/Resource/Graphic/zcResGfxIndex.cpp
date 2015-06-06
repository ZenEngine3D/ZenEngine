#include "zcCore.h"

namespace zcRes
{	
	zU8* GfxIndex::Lock()
	{	
		return mrProxy->Lock(); 
	}

	void GfxIndex::Unlock()
	{ 
		mrProxy->Unlock(); 
	}

}