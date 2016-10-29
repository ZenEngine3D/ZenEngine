vDefaultCppExt			= {".h", ".inl", ".cpp"}
vLibEngineGame 			= {"libZenBase", "libZenCore", "libZenEngine", "libZenExternal", "lib3rdImgui"  }
vLibEngineTool 			= {"libZenBase", "libZenCore", "libZenEngine", "libZenExternal", "lib3rdImgui", "lib3rdPugiXml" }
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
		defines			( {"DEBUG", "_DEBUG", "ZEN_BUILD_DEBUG"} )
		flags 			( {"Symbols","FatalWarnings"} )
		optimize		( "Off" )
		targetsuffix 	( "_Deb" )
	configuration 		( "Release" )
		defines			( {"NDEBUG", "ZEN_BUILD_RELEASE"} )
		flags 			( {"Symbols"} )
		optimize		( "On" )
		targetsuffix 	( "_Rel")
	configuration 		( "Final" )
		defines			( {"NDEBUG", "ZEN_BUILD_FINAL"} )
		optimize		( "Full" )
	
	--[[ Platform ]]--
	configuration		( "PC*")
		system 			( "Windows")
		defines			( {"WIN32","_WINDOWS", "ZEN_PLATFORM=PC", "ZEN_PLATFORM_PC=1", "ZEN_RENDERER=DX11", "ZEN_RENDERER_DX11=1"})
		debugdir		( vSourceRoot .. "/Build/Content")
	--[[	
	configuration		( "PCGame32")
		architecture 	( "x32")
		defines			( {"ZEN_ENGINEGAME=1"})
		targetdir 		( vOutputRoot .. iif(vIsLib, "/" .. project().name .. "/Game32", "/../[Bin]/Windows32") )
	configuration 		( "PCTool32")
		architecture 	( "x32")
		defines			( {"ZEN_ENGINETOOL=1"})
		targetdir 		( vOutputRoot .. iif(vIsLib, "/" .. project().name .. "/Tool32", "/../[Bin]/Windows32") )
	]]--	
	configuration 		( "PCGame64")
		architecture 	( "x64")		
		defines			( {"ZEN_ENGINEGAME=1"})
		targetdir 		( vOutputRoot .. iif(vIsLib, "/" .. project().name .. "/Game64", "/../[Bin]/Windows64") )	
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
		vPathStriped = path.getdirectory(aPathList[i])
		vFilename = vSourceRoot .. "/" .. vPathStriped .. "/" .. vPchNameHeader
		if os.isfile(vFilename) then
			vPchDir = path.getdirectory(vFilename)
		end
		i = i + 1
	end	
	if #vPchDir == 0 then return end
	includedirs 	( vPchDir )
		
	--Generate a cpp file used to compile pch	
	vLocation = vOutputRoot .. "/" .. project().name
	vPchNameSource 	= vLocation .. "/PrecompileHeader.cpp"
	if vPlatform == "Window" then
		vPchNameSource = string.gsub(vPchNameSource, "/", "\\")
	end		
	vCppFile 	= io.open (vPchNameSource, "w+")
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
-- 		aFilesExt		: List of supported files extensions to add
-- ============================================================================
function Orion_AddProjectCommon(aFilesExt, aPathList, aPchFile )			
	os.mkdir		( vOutputRoot .. "/" .. project().name ) -- Needed for pch file creation
	location 		( vOutputRoot .. "/" .. project().name )
	objdir			( vOutputRoot .. "/" .. project().name .. "/obj" )
	includedirs 	( {vSourceRoot, vSourceRoot .. "/Engine", vSourceRoot .. "/Engine/ZenApi"} )
	-- ToDo find more generalize way to do this
	vpaths 			( {["Api/*"] = "../../../Engine/ZenApi/ZenBase" } )
	vpaths 			( {["Api/*"] = "../../../Engine/ZenApi/ZenCore" } )
	vpaths 			( {["Api/*"] = "../../../Engine/ZenApi/ZenEngine" } )
	language 		( "C++" )	
	Orion_ConfigurePCH	( aPathList, aPchFile )
	
	for i, vPath in ipairs(aPathList) do 		
		for j, vExt in ipairs(aFilesExt) do 
			files( vSourceRoot .. "/" .. vPath .. vExt )			
			--print("[" .. project().name .. "] Adding : " .. vPath .. vExt)
		end
		vpaths( {["*"] = vSourceRoot .. "/" .. vPath .. "/**" } )
	end
	
	Orion_ConfigureBuild()
end

-- ============================================================================
-- 	Add support for WxWidget, to a particular project
--  Needs to get those file from "https://github.com/wxWidgets/wxWidgets/releases/tag/v3.1.0" : 
--		wxMSW-3.1.0_vc140_x64_Dev.7z
--		wxWidgets-3.1.0-headers.7z
--		wxMSW-3.1.0_vc140_x64_ReleaseDLL.7z
--		wxMSW-3.1.0_vc140_x64_ReleasePDB.7z
-- ============================================================================
function Orion_AddProjectWxWidget()
	vLibsDebug = {"wxmsw31ud_core", "wxbase31ud", "wxmsw31ud_aui", "wxmsw31ud_propgrid", "wxmsw31ud_adv", "wxjpegd", "wxpngd", "wxzlibd", "wxregexud", "wxexpatd", "wxtiffd"}
	vLibsRelease = {"wxmsw31u_core", "wxbase31u", "wxmsw31u_aui", "wxmsw31u_propgrid", "wxmsw31u_adv", "wxjpeg", "wxpng", "wxzlib", "wxregexu", "wxexpat", "wxtiff"}
	
	configuration		( {} )
		includedirs		( {vSourceRoot .. "/Engine/ThirdParty/[wxWidgets]/include"} )
		includedirs		( {vSourceRoot .. "/Engine/ThirdParty/[wxWidgets]/include/msvc"} )	
		defines			( {"WXUSINGDLL=1", "wxMSVC_VERSION=140"} )		
	configuration		( "*64")
		libdirs 		(vSourceRoot .. "/Engine/ThirdParty/[wxWidgets]/lib/vc140_x64_dll")
	configuration( "PC*" )	
		postbuildcommands { "xcopy \"" .. vSourceRoot .. "/Engine/ThirdParty/[wxWidgets]/lib/vc140_x64_dll\\*.dll\" \"$(TargetDir)\" /Y /D" } --xcopy doesn't support '/' with wirldcard, so last one is a '\'
	configuration		( "Debug" )
		links			( vLibsRelease )
	configuration 		( "Release" )
		links			( vLibsRelease )
	configuration 		( "Final" )
		links			( vLibsRelease )	
end
