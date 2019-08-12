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

void GPUContext::Submit(const zArrayDyn<zEngineRef<zcGfx::Command>>& _aDrawcalls)
{
	for(const auto& drawcall : _aDrawcalls)
		drawcall->Invoke(*this);
}


}
