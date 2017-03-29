#include "zcCore.h"

namespace zcGfx
{

GPUContext_Base::GPUContext_Base()
{
	Reset();
}

void GPUContext_Base::Reset()
{
	mrRenderpass		= nullptr;
	mrStateView			= nullptr;
	mrStateBlend		= nullptr;
	mrStateDepthStencil	= nullptr;
	mrStateRaster		= nullptr;
}

void GPUContext::Submit(const zArrayDynamic<zEngineRef<zcGfx::Command>>& _aDrawcalls)
{
	const zEngineRef<zcGfx::Command>* prDrawcall	= _aDrawcalls.First();
	const zEngineRef<zcGfx::Command>* prDrawcallEnd	= _aDrawcalls.Last();
	while( prDrawcall <= prDrawcallEnd )
	{
		(*prDrawcall)->Invoke(*this);
		++prDrawcall;		
	}
}


}
