vPCHEngineBase 		= "Engine/ZenApi/ZenBase/zenBase.h"
vPCHEngineCore 		= "Engine/ZenApi/ZenCore/zenCore.h"
vPCHEngineEngine 	= "Engine/ZenApi/ZenEngine/zenEngine.h"

-------------------------------------------------------------------------------
-- Engine Libraries 
-------------------------------------------------------------------------------
Orion_AddGameAndToolLib("libZenBase", 		"ZenEngine", 	{"Engine/ZenBase/**", 		"Engine/ZenApi/ZenBase/**"},		"zbBase.h")
Orion_AddGameAndToolLib("libZenCore", 		"ZenEngine",	{"Engine/ZenCore/**", 		"Engine/ZenApi/ZenCore/**"},		"zcCore.h")
Orion_AddGameAndToolLib("libZenEngine", 	"ZenEngine",	{"Engine/ZenEngine/**", 	"Engine/ZenApi/ZenEngine/**"}, 		"zeEngine.h")
Orion_AddGameAndToolLib("libZenExternal",	"ZenEngine",	{"Engine/ZenExternal/**", 	"Engine/ZenApi/ZenExternal/**"},	"zxExternal.h")
Orion_AddGameAndToolLib("lib3rdPugiXml",	"ZenEngine/ThirdParty",	{"Engine/ThirdParty/pugixml/src/*"},				"")
Orion_AddGameAndToolLib("lib3rdImgui",		"ZenEngine/ThirdParty",	{"Engine/ThirdParty/imgui/*"},						"")

-------------------------------------------------------------------------------
-- Engine Samples
-------------------------------------------------------------------------------

-- SAMPLE CORE --
--TODO change to api pch and not whole engine
Orion_AddGameExe("SampleBaseType",		"Samples", 	{"Engine/Samples/Core/Type/**"},	vPCHEngineEngine, {}) 
Orion_AddGameExe("SampleBaseMem",		"Samples", 	{"Engine/Samples/Core/Mem/**"},		vPCHEngineEngine, {}) 
Orion_AddGameExe("SampleBaseLog",		"Samples", 	{"Engine/Samples/Core/Log/**"},		vPCHEngineEngine, {}) 
Orion_AddGameExe("SampleBaseMath",		"Samples", 	{"Engine/Samples/Core/Math/**"},	vPCHEngineEngine, {}) 
Orion_AddGameExe("SampleBaseSignal",	"Samples", 	{"Engine/Samples/Core/Signal/**"},	vPCHEngineEngine, {}) 
Orion_AddGameExe("SampleBaseSystem",	"Samples", 	{"Engine/Samples/Core/System/**"},	vPCHEngineEngine, {}) 

-- SAMPLE GFX --
Orion_AddGameExe("SampleGfxBasic",		"Samples",	{"Engine/Samples/Gfx/Renderer/**"},	vPCHEngineEngine, {})
Orion_AddGameExe("SampleGfxDebugUI",	"Samples",	{"Engine/Samples/Gfx/DebugUI/**"},	vPCHEngineEngine, {})