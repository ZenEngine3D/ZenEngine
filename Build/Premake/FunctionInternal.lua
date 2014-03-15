vSourceFiles			= {"**.h", "**.inl", "**.cpp", "**.ui"}
vLibEngineGame 			= {"libZenBase", "libZenCore", "libZenEngine"  }
vLibEngineTool 			= {"libZenBase", "libZenCore", "libZenEngine", "libThirdParty" }
bLibEngineGameRender	= {"d3d11", "d3dcompiler", "dxguid"}	--TODO per platform config
bLibEngineToolRender	= {"d3d11", "d3dcompiler", "dxguid"}

--[[ Useful bit of code 
if vPath:sub(vPath,-1) ~= '/' then
			vPath = vPath .. "/"
		end
		vFiles:insert(vFiles, vPath .. vExt)		
--]]

-- ============================================================================
-- 	Setup the build parameter per build/platform configuration
-- ============================================================================
function Orion_ConfigureBuild()
	local vIsLib = kind() == "StaticLib"	
	
	--[[ Build version config ]]--
	configuration		( "Debug" )
		defines			( {"DEBUG", "_DEBUG", "AW_BUILD_DEBUG"} )
		flags 			( {"Symbols"} )
		optimize		( "Off" )
		targetsuffix 	( "_Deb" )
	configuration 		( "Release" )
		defines			( {"NDEBUG", "AW_BUILD_RELEASE"} )
		flags 			( {"Symbols"} )
		optimize		( "On" )
		targetsuffix 	( "_Rel")
	configuration 		( "Final" )
		defines			( {"NDEBUG", "AW_BUILD_FINAL"} )
		optimize		( "Full" )
	
	--[[ Platform ]]--
	configuration		( "PC*")
		system 			( "Windows")
		defines			( {"WIN32","_WINDOWS", "AW_PLATFORM=PC", "AW_PLATFORM_PC=1", "AW_RENDERER=DX11", "AW_RENDERER_DX11=1"})
		debugdir		( vSourceRoot .. "/Build/Content")
		
	configuration		( "PCGame32")
		architecture 	( "x32")
		defines			( {"ZEN_ENGINEGAME=1"})
		targetdir 		( vOutputRoot .. iif(vIsLib, "/" .. project().name .. "/Game32", "/../[Bin]/Windows32") )		
	configuration 		( "PCGame64")
		architecture 	( "x64")		
		defines			( {"ZEN_ENGINEGAME=1"})
		targetdir 		( vOutputRoot .. iif(vIsLib, "/" .. project().name .. "/Game64", "/../[Bin]/Windows64") )
	configuration 		( "PCTool32")
		architecture 	( "x32")
		defines			( {"ZEN_ENGINETOOL=1"})
		targetdir 		( vOutputRoot .. iif(vIsLib, "/" .. project().name .. "/Tool32", "/../[Bin]/Windows32") )
	configuration 		( "PCTool64")
		architecture 	( "x64")
		defines			( {"ZEN_ENGINETOOL=1"})
		targetdir 		( vOutputRoot .. iif(vIsLib, "/" .. project().name .. "/Tool64", "/../[Bin]/Windows64") )	
end

-- ============================================================================
-- 	Setup the Precompiled header of a project
-- 		aPathList	: List of root path of library
-- 		aPchFile	: Name of PCH header filename ("" if none is used)
-- ============================================================================
function Orion_ConfigurePCH(aPathList, aPchFile)
	
	-- Look for pch file
	if #aPchFile == 0 then return end
	
	vPchDir 		= ""
	vPchNameHeader 	= path.getbasename(aPchFile) .. ".h"	
	if os.isfile( vSourceRoot .. "/" .. aPchFile ) then
		vPchDir	= path.getdirectory(vSourceRoot .. "/" .. aPchFile)
	end
	i 				= 1	
	while i <= #aPathList and #vPchDir == 0 do 		
		vFilename = vSourceRoot .. "/" .. aPathList[i] .. "/" .. vPchNameHeader
		if os.isfile(vFilename) then
			vPchDir = path.getdirectory(vFilename)
		end
		i = i + 1
	end	
	if #vPchDir == 0 then return end
	includedirs 	( vPchDir )
		
	--Generate a cpp file used to compile pch	
	vPchNameSource 	= location() .. "/PrecompileHeader.cpp"
	if vPlatform == "Window" then
		vPchNameSource = string.gsub(vPchNameSource, "/", "\\")
	end		
	vCppFile 		= io.open (vPchNameSource, "w+")
	vCppFile:write	( "#include \"" .. vPchNameHeader .. "\"" )	
	vCppFile:close 	( )
	
	-- Configure pch
	files			( vPchNameSource )
	pchsource		( vPchNameSource )
	pchheader		( vPchNameHeader )	
	
end

-- ============================================================================
-- 	Common project creation code
-- 		aPathList		: List of root path of library
-- 		aPchFile		: Name of PCH header filename ("" if none is used)
-- ============================================================================
function Orion_AddProjectCommon(aPathList, aPchFile)			
	os.mkdir			( vOutputRoot .. "/" .. project().name )
	location 			( vOutputRoot .. "/" .. project().name )
	includedirs 		( {vSourceRoot, vSourceRoot .. "/Engine/Include"} )
	vpaths 				( {["*"] = "../../../Engine/Include" } )
	vpaths 				( {["*"] = "../../../Engine/libZenBase"} )
	vpaths 				( {["*"] = "../../../Engine/libZenCore"} )
	vpaths 				( {["*"] = "../../../Engine/libZenEngine"} )
	language 			( "C++" )	
	Orion_ConfigurePCH	( aPathList, aPchFile )
	
	for i, vPath in ipairs(aPathList) do 
		for j, vExt in ipairs(vSourceFiles) do 
			files( vSourceRoot .. "/" .. vPath .. vExt )
			--print("[" .. project().name .. "] Adding : " .. vPath .. vExt)
		end
	end
	
	Orion_ConfigureBuild()
end

-- ============================================================================
-- 	Add support for WxWidget, to a particular project
-- ============================================================================
function Orion_AddProjectWxWidget()
	vLibsDebug = {"wxmsw29ud_core", "wxbase29ud", "wxmsw29ud_aui", "wxmsw29ud_propgrid", "wxmsw29ud_adv", "wxjpegd", "wxpngd", "wxzlibd", "wxregexud", "wxexpatd", "wxtiffd"}
	vLibsRelease = {"wxmsw29u_core", "wxbase29u", "wxmsw29u_aui", "wxmsw29u_propgrid", "wxmsw29u_adv", "wxjpeg", "wxpng", "wxzlib", "wxregexu", "wxexpat", "wxtiff"}
	
	configuration		( {} )
		includedirs( {vSourceRoot .. "/Engine/ThirdParty/[wxWidgets]/include"} )
		includedirs( {vSourceRoot .. "/Engine/ThirdParty/[wxWidgets]/include/msvc"} )
	
	configuration		( "*32")
		libdirs 		(vSourceRoot .. "/Engine/ThirdParty/[wxWidgets]/wxWidgets-2_9_4(x32)/lib/vc_lib")
	configuration		( "*64")
		libdirs 		(vSourceRoot .. "/Engine/ThirdParty/[wxWidgets]/wxWidgets-2_9_4(x64)/lib/vc_lib")
	configuration		( "Debug" )
		links			( vLibsDebug )
	configuration 		( "Release" )
		links			( vLibsRelease )
	configuration 		( "Final" )
		links			( vLibsRelease )
	
end
