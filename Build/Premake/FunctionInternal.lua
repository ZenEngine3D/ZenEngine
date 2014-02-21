vSourceFiles			= {"**.h", "**.inl", "**.cpp"}
vLibEngineGame 			= {"libEngineCore", "libEngineEngine", "libEngineFramework"  }
vLibEngineTool 			= {"libEngineCore", "libEngineEngine", "libEngineFramework", "libEngine3rdParty" }
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
		targetsuffix 	("_Rel")
	configuration 		( "Final" )
		defines			( {"NDEBUG", "AW_BUILD_FINAL"} )
		optimize		( "Full" )
	
	--[[ Platform ]]--
	configuration		({"PC*"})
		system 			("Windows")
		defines			({"WIN32","_WINDOWS", "AW_PLATFORM=PC", "AW_PLATFORM_PC=1", "AW_RENDERER=DX11", "AW_RENDERER_DX11=1"})

	--[[ Game vs Tool config ]]--
	configuration		({"PCGame32"})
		architecture 	("x32")
		defines			({"AW_ENGINEGAME=1"})
	configuration 		({"PCGame64"})
		architecture 	("x64")		
		defines			({"AW_ENGINEGAME=1"})
	configuration 		({"PCTool32"})
		architecture 	("x32")
		defines			({"AW_ENGINETOOL=1"})
	configuration 		({"PCTool64"})
		architecture 	("x64")
		defines			({"AW_ENGINETOOL=1"})
		
	--for i, vPath in ipairs(aPathList) do		
	--	excludes( vPath .. vFile )
	--end			
end

-- ============================================================================
-- 	Setup the Precompiled header of a project
-- 		aLibName 	: Library target name
-- 		aPathList	: List of root path of library
-- 		aPchFile	: Name of PCH header filename ("" if none is used)
-- ============================================================================
function Orion_ConfigurePCH(aLibName, aPathList, aPchFile)
	
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
-- 		aProjectName 	: Library target name
-- 		aPathList		: List of root path of library
-- 		aPchFile		: Name of PCH header filename ("" if none is used)
-- ============================================================================
function Orion_AddProjectCommon(aProjectName, aPathList, aPchFile)			
	os.mkdir			( vOutputRoot .. "/" .. aProjectName )
	location 			( vOutputRoot .. "/" .. aProjectName )
	includedirs 		( {vSourceRoot, vSourceRoot .. "/AWEngine/Include"} )
	language 			( "C++" )	
	Orion_ConfigurePCH	( aProjectName, aPathList, aPchFile )
	
	for i, vPath in ipairs(aPathList) do 
		for j, vExt in ipairs(vSourceFiles) do 
			files( vSourceRoot .. "/" .. vPath .. vExt )
			--print("[" .. aProjectName .. "] Adding : " .. vPath .. vExt)
		end
	end
	
	Orion_ConfigureBuild()
end


