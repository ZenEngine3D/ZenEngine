
-- ============================================================================
-- 	Create a project for a Library target
-- 		aLibName 	: Library target name
-- 		aPrjGroup	: Solution group to store the project in
-- 		aPathList	: List of root path of library
-- 		aPchFile	: Name of PCH header filename ("" if none is used)
-- ============================================================================
function Orion_AddGameLib(aLibName, aPrjGroup, aPathList, aPchFile)	
	group					( aPrjGroup )
	project					( aLibName )
	removeplatforms 		( vPlatformTool )
	kind 					( "StaticLib" )
	Orion_AddProjectCommon	( vDefaultCppExt, aPathList, aPchFile )
end

-- ============================================================================
-- 	Create a project for a Library target
-- 		aLibName 	: Library target name
-- 		aPrjGroup	: Solution group to store the project in
-- 		aPathList	: List of root path of library
-- 		aPchFile	: Name of PCH header filename ("" if none is used)
-- ============================================================================
function Orion_AddToolLib(aLibName, aPrjGroup, aPathList, aPchFile)	
	group					( aPrjGroup )
	project					( aLibName )
	removeplatforms 		( vPlatformGame )
	kind 					( "StaticLib" )
	Orion_AddProjectCommon	( vDefaultCppExt, aPathList, aPchFile )
end

-- ============================================================================
-- 	Create a project for a Library target
-- 		aLibName 	: Library target name
-- 		aPrjGroup	: Solution group to store the project in
-- 		aPathList	: List of root path of library
-- 		aPchFile	: Name of PCH header filename ("" if none is used)
-- ============================================================================
function Orion_AddGameAndToolLib(aLibName, aPrjGroup, aPathList, aPchFile)	
	group					( aPrjGroup )
	project					( aLibName )
	kind 					( "StaticLib" )
	Orion_AddProjectCommon	( vDefaultCppExt, aPathList, aPchFile )
end

-- ============================================================================
-- 	Create a project for an Executable target
-- 		aExeName 	: Library target name
-- 		aPrjGroup	: Solution group to store the project in
-- 		aPathList	: List of root path of library
-- 		aPchName	: Name of PCH header filename ("" if none is used)
-- 		aLibs		: Array of additional libs to include
-- ============================================================================
function Orion_AddGameExe(aExeName, aPrjGroup, aPathList, aPchFile, aLibs)
	group					( aPrjGroup )
	project					( aExeName )
	removeplatforms 		( vPlatformTool )
	kind 					( "ConsoleApp" )
	links					( {vLibEngineGame, bLibEngineGameRender, aLibs} )
	Orion_AddProjectCommon	( vDefaultCppExt, aPathList, aPchFile )
end

-- ============================================================================
-- 	Create a project for an Executable target
-- 		aExeName 	: Library target name
-- 		aPrjGroup	: Solution group to store the project in
-- 		aPathList	: List of root path of library
-- 		aPchName	: Name of PCH header filename ("" if none is used)
-- 		aLibs		: Array of additional libs to include
-- ============================================================================
function Orion_AddToolUIExe(aExeName, aPrjGroup, aPathList, aPchFile, aLibs)
	group					( aPrjGroup )
	project					( aExeName )
	removeplatforms 		( vPlatformGame )
	kind 					( "ConsoleApp" )
	links					( {vLibEngineTool, bLibEngineToolRender, aLibs} )
	Orion_AddProjectCommon	(  vDefaultCppExt, aPathList, aPchFile )
	Orion_AddProjectWxWidget()	
end		
		
	
		

