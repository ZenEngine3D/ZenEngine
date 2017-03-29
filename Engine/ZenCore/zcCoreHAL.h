#pragma once

namespace zcRes
{
}

namespace zcGfx
{
	ForwardDeclareRendererHAL(ScopedDrawlist);
	ForwardDeclareRendererHAL(GPUContext);
	ForwardDeclareRendererHAL(ManagerRender);

	ForwardDeclareRendererHAL(CommandDraw);
	ForwardDeclareRendererHAL(CommandClearColor);
	ForwardDeclareRendererHAL(CommandClearDepthStencil);	
}

namespace zcPerf
{
	ForwardDeclarePlatformHAL(EventCPU);
	ForwardDeclareRendererHAL(EventGPU);	
}

