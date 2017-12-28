#pragma once

namespace zcRes
{
}

namespace zcGfx
{
	ForwardDeclareRendererHAL(CommandList);
	ForwardDeclareRendererHAL(GPUContext);
	ForwardDeclareRendererHAL(ManagerRender);

	ForwardDeclareRendererHAL(CommandDraw);
	ForwardDeclareRendererHAL(CommandClearColor);
	ForwardDeclareRendererHAL(CommandClearDepthStencil);
	ForwardDeclareRendererHAL(CommandGPUScopedEvent);
}

namespace zcPerf
{
	ForwardDeclarePlatformHAL(EventCPU);
	ForwardDeclareRendererHAL(EventGPU);	
}

