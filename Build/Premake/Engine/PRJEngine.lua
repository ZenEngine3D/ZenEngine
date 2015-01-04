vPCHEngineBase 		= "Engine/Include/ZenBase/zenBase.h"
vPCHEngineCore 		= "Engine/Include/ZenCore/zenCore.h"
vPCHEngineEngine 	= "Engine/Include/ZenEngine/zenEngine.h"

-------------------------------------------------------------------------------
-- Engine Libraries 
-------------------------------------------------------------------------------
vThirdPartySrc = {"Engine/ThirdParty/pugixml/src"}
Orion_AddGameAndToolLib("libZenBase", 		"ZenEngine", 	{"Engine/libZenBase", 		"Engine/Include/ZenBase"}, 		"libZenBase.h")
Orion_AddGameAndToolLib("libZenCore", 		"ZenEngine",	{"Engine/libZenCore", 		"Engine/Include/ZenCore"}, 		"libZenCore.h")
Orion_AddGameAndToolLib("libZenEngine", 	"ZenEngine",	{"Engine/libZenEngine", 	"Engine/Include/ZenEngine"}, 	"libZenEngine.h")
Orion_AddGameAndToolLib("libZenExternal",	"ZenEngine",	{"Engine/libZenExternal", 	"Engine/Include/ZenExternal"},	"libZenExternal.h")
Orion_AddGameAndToolLib("libThirdParty",	"ZenEngine",	vThirdPartySrc,												"")

-------------------------------------------------------------------------------
-- Engine Samples
-------------------------------------------------------------------------------

-- SAMPLE CORE --
--TODO change to api pch and not whole engine
Orion_AddGameExe("SampleBaseType",		"Samples", 	{"Engine/Samples/Core/Type"},		"Engine/libZenEngine/libZenEngine.h", {}) 
Orion_AddGameExe("SampleBaseMem",		"Samples", 	{"Engine/Samples/Core/Mem"},		"Engine/libZenEngine/libZenEngine.h", {}) 
Orion_AddGameExe("SampleBaseLog",		"Samples", 	{"Engine/Samples/Core/Log"},		"Engine/libZenEngine/libZenEngine.h", {}) 
Orion_AddGameExe("SampleBaseMath",		"Samples", 	{"Engine/Samples/Core/Math"},		"Engine/libZenEngine/libZenEngine.h", {}) 
Orion_AddGameExe("SampleBaseSignal",	"Samples", 	{"Engine/Samples/Core/Signal"},		"Engine/libZenEngine/libZenEngine.h", {}) 
Orion_AddGameExe("SampleBaseSystem",	"Samples", 	{"Engine/Samples/Core/System"},		"Engine/libZenEngine/libZenEngine.h", {}) 

-- SAMPLE GFX --
Orion_AddGameExe("SampleGfxBasic",		"Samples",	{"Engine/Samples/Gfx/Renderer"},	vPCHEngineEngine, {})