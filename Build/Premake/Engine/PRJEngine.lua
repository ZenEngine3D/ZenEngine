vPCHEngineCore 		= "AWEngine/Include/AWECore/AWCore.h"
vPCHEngineEngine 	= "AWEngine/Include/AWEEngine/AWEngine.h"
vPCHEngineFramework = "AWEngine/Include/AWEFramework/AWFramework.h"

-------------------------------------------------------------------------------
-- Engine Libraries 
-------------------------------------------------------------------------------
Orion_AddGameAndToolLib("libEngineCore", 		"Engine", 	{"AWEngine/AWECore"}, 		"CoreAll.h")
Orion_AddGameAndToolLib("libEngineEngine", 		"Engine",	{"AWEngine/AWEEngine"}, 	"EngineAll.h")
Orion_AddGameAndToolLib("libEngineFramework", 	"Engine",	{"AWEngine/AWEFramework"}, 	"FrameworkAll.h")
Orion_AddGameAndToolLib("libEngine3rdParty",	"Engine",	{"AWEngine/External/pugixml/src"}, 	"")

-------------------------------------------------------------------------------
-- Engine Samples
-------------------------------------------------------------------------------

-- SAMPLE CORE --
--TODO change to api pch and not whole engine
Orion_AddGameExe("SampleCoreType",			"Engine/Sample", {"AWEngine/AWESamples/Core/Type"},		"AWEngine/AWEFramework/FrameworkAll.h", {}) 
Orion_AddGameExe("SampleCoreMem",			"Engine/Sample", {"AWEngine/AWESamples/Core/Mem"},		"AWEngine/AWEFramework/FrameworkAll.h", {}) 
Orion_AddGameExe("SampleCoreLog",			"Engine/Sample", {"AWEngine/AWESamples/Core/Log"},		"AWEngine/AWEFramework/FrameworkAll.h", {}) 
Orion_AddGameExe("SampleCoreMath",			"Engine/Sample", {"AWEngine/AWESamples/Core/Math"},		"AWEngine/AWEFramework/FrameworkAll.h", {}) 
Orion_AddGameExe("SampleCoreSystem",		"Engine/Sample", {"AWEngine/AWESamples/Core/System"},	"AWEngine/AWEFramework/FrameworkAll.h", {}) 

-- SAMPLE GFX --
Orion_AddGameExe("SampleGfxBasic",			"Engine/Sample", {"AWEngine/AWESamples/Gfx/Renderer"},	vPCHEngineFramework, {})

--[[
Orion_AddGameLib("libEngineGameCore", 		"Engine", 	{"AWEngine/AWECore"}, 				"CoreAll.h")
Orion_AddGameLib("libEngineGameEngine", 	"Engine",	{"AWEngine/AWEEngine"}, 			"EngineAll.h")
Orion_AddGameLib("libEngineGameFramework", 	"Engine",	{"AWEngine/AWEFramework"}, 			"FrameworkAll.h")
Orion_AddToolLib("libEngineToolCore", 		"Engine",	{"AWEngine/AWECore"}, 				"CoreAll.h")
Orion_AddToolLib("libEngineToolEngine", 	"Engine",	{"AWEngine/AWEEngine"}, 			"EngineAll.h")
Orion_AddToolLib("libEngineToolFramework", 	"Engine",	{"AWEngine/AWEFramework"}, 			"FrameworkAll.h")
Orion_AddToolLib("libEngineTool3rdParty", 	"Engine",	{"AWEngine/External/pugixml/src"}, 	"")

-------------------------------------------------------------------------------
-- Engine Samples
-------------------------------------------------------------------------------

-- SAMPLE CORE --
--TODO change to api pch and not whole engine
Orion_AddGameExe("SampleCoreType",			"Engine/Sample", {"AWEngine/AWESamples/Core/Type"},		"AWEngine/AWEFramework/FrameworkAll.h", {}) 
Orion_AddGameExe("SampleCoreMem",			"Engine/Sample", {"AWEngine/AWESamples/Core/Mem"},		"AWEngine/AWEFramework/FrameworkAll.h", {}) 
Orion_AddGameExe("SampleCoreLog",			"Engine/Sample", {"AWEngine/AWESamples/Core/Log"},		"AWEngine/AWEFramework/FrameworkAll.h", {}) 
Orion_AddGameExe("SampleCoreMath",			"Engine/Sample", {"AWEngine/AWESamples/Core/Math"},		"AWEngine/AWEFramework/FrameworkAll.h", {}) 
Orion_AddGameExe("SampleCoreSystem",		"Engine/Sample", {"AWEngine/AWESamples/Core/System"},	"AWEngine/AWEFramework/FrameworkAll.h", {}) 

-- SAMPLE GFX --
Orion_AddGameExe("SampleGfxBasic",			"Engine/Sample", {"AWEngine/AWESamples/Gfx/Renderer"},	vPCHEngineFramework, {})

]]