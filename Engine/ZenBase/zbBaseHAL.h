#pragma once

#define ForwardDeclarePlatformHAL(HalName) using HalName ## _HAL = class zenDefineStich3(HalName, _, ZEN_PLATFORM); // Creates a typedef (ClassName)_HAL from Platform specific classes
#define ForwardDeclareRendererHAL(HalName) using HalName ## _HAL = class zenDefineStich3(HalName, _, ZEN_RENDERER); // Creates a typedef (ClassName)_HAL from Renderer specific classes

namespace zbSys
{
	ForwardDeclarePlatformHAL(zbFiber);
}