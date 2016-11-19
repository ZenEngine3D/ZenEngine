vDefaultCppExt			= {".h", ".inl", ".cpp"}
vLibEngineGame 			= {"libZenBase", "libZenCore", "libZenEngine", "libZenExternal", "lib3rdImgui"  }
vLibEngineTool 			= {"libZenBase", "libZenCore", "libZenEngine", "libZenExternal", "lib3rdImgui", "lib3rdPugiXml" }

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

	--[[ Configurations ]]--
	language 			( "C++" )
	systemversion 		("10.0.14393.0")
	--rtti 				('Off')
	
	filter 'configurations:Debug'
		defines			{"DEBUG", "_DEBUG", "ZEN_BUILD_DEBUG"}
		flags 			{"FatalWarnings", "FatalCompileWarnings"}
		optimize		'Off'
		symbols			'On'
		targetsuffix 	'_%{cfg.platform}_Deb'
	filter 'configurations:Release'
		defines			{'NDEBUG', 'ZEN_BUILD_RELEASE'}
		flags 			{ }
		optimize		'On'
		symbols			'On'
		targetsuffix 	'_%{cfg.platform}_Rel'
	filter 'configurations:Final'
		defines			{'NDEBUG', 'ZEN_BUILD_FINAL'}
		flags 			{ }
		optimize		'On'
		symbols			'Off'
		targetsuffix 	'_%{cfg.platform}_Fin'
	filter {}
	
	--[[ Platforms ]]--	
	filter 'platforms:PC*' 
		system 			'Windows'				
		flags 			{'EnableSSE2'}		
		defines			{'WIN32', '_WINDOWS', 'ZEN_PLATFORM=PC', 'ZEN_PLATFORM_PC=1'}

	filter 'platforms:*DX11' 						
		defines			{'ZEN_RENDERER=DX11', 'ZEN_RENDERER_DX11=1'}
		--systemversion 	'8.1'
		
	filter 'platforms:*DX12' 						
		defines			{'ZEN_RENDERER=DX12', 'ZEN_RENDERER_DX12=1'}	
		--systemversion 	'10.0.14393.0'

	filter 'platforms:*64*'							
		architecture 	'x64'	
		
	filter 'platforms:PCTool*'						defines			{'ZEN_ENGINETOOL=1'}
	filter 'platforms:not PCTool*'					defines			{'ZEN_ENGINEGAME=1'}
	filter {}
	
	--[[ Output Dir ]]--
	objdir 	( vOutputRoot .. '/%{prj.name}/obj' )
	debugdir( vSourceRoot .. "/Build/Content" )
	filter { 'kind:ConsoleApp' }					targetdir(vOutputRoot .. '/../[Bin]/%{cfg.platform}')		
	filter { 'kind:SharedLib or StaticLib' }		targetdir('%{cfg.objdir}')
		
	--[[ Remove compiling of platform specific files not valid for current build ]]--	
	filter {'files:**_*', 'files:not **_Base.*'}	flags { "ExcludeFromBuild" }
	filter {'platforms:PC*', 'files:**_PC.*' }		removeflags { "ExcludeFromBuild" }
	filter {'platforms:*DX11', 'files:**_DX11.*' }	removeflags { "ExcludeFromBuild" }
	filter {'platforms:*DX12', 'files:**_DX12.*' }	removeflags { "ExcludeFromBuild" }
	
	filter {}
end

function Orion_AddGameLibs()	
	filter {'platforms:*DX11'}	links	{"d3d11", "d3dcompiler", "dxguid"}
	filter {'platforms:*DX12'}	links	{"d3d12", "d3dcompiler", "dxgi", "dxguid"}
	filter{} 					links	{vLibEngineGame}
end

function Orion_AddToolLibs()	
	filter {'platforms:*DX11'}	links	{"d3d11", "d3dcompiler", "dxguid"}
	filter {'platforms:*DX12'}	links	{"d3d12", "d3dcompiler", "dxgi", "dxguid"}
	filter{}					links	{vLibEngineTool}
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
	includedirs 	( {vSourceRoot, vSourceRoot .. "/Engine", vSourceRoot .. "/Engine/ZenApi"} )
	
	-- ToDo find more generalize way to do this
	vpaths 			( {["Api/*"] = vSourceRoot .. "/Engine/ZenApi/ZenBase/*" } )
	vpaths 			( {["Api/*"] = vSourceRoot .. "/Engine/ZenApi/ZenCore/*" } )
	vpaths 			( {["Api/*"] = vSourceRoot .. "/Engine/ZenApi/ZenEngine/*" } )
	vpaths 			( {["Api/*"] = vSourceRoot .. "/Engine/ZenBase/ZenBase/*" } )
	vpaths 			( {["Api/*"] = vSourceRoot .. "/Engine/ZenCore/ZenCore/*" } )
	vpaths 			( {["Api/*"] = vSourceRoot .. "/Engine/ZenEngine/ZenEngine/*" } )
	
	Orion_ConfigurePCH	( aPathList, aPchFile )	
	for i, vPath in ipairs(aPathList) do
		vpaths( {["*"] = vSourceRoot .. "/" .. vPath } )
		for j, vExt in ipairs(aFilesExt) do 
			files( vSourceRoot .. "/" .. vPath .. vExt )			
			--print("[" .. project().name .. "] Adding : " .. vPath .. vExt)
		end 
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
	
	includedirs		{vSourceRoot .. "/Engine/ThirdParty/[wxWidgets]/include"}
	includedirs		{vSourceRoot .. "/Engine/ThirdParty/[wxWidgets]/include/msvc"}
	defines			{"WXUSINGDLL=1", "wxMSVC_VERSION=140"}
	links			{vLibsRelease}
	filter 'platforms:PC*64'
		libdirs 		{vSourceRoot .. "/Engine/ThirdParty/[wxWidgets]/lib/vc140_x64_dll"}
	filter 'platforms:PC*'
		postbuildcommands { "xcopy \"" .. vSourceRoot .. "/Engine/ThirdParty/[wxWidgets]/lib/vc140_x64_dll\\*.dll\" \"$(TargetDir)\" /Y /D" } --xcopy doesn't support '/' with wirldcard, so last one is a '\'
	filter {}
end
