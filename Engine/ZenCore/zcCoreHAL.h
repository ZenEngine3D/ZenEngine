#pragma once

namespace zcRes
{
}

namespace zcGfx
{
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

