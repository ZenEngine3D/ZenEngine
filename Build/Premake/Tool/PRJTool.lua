-------------------------------------------------------------------------------
-- Zendo Editor
-------------------------------------------------------------------------------
Orion_AddToolExe("Zendo",		"Tool", 	{"Tool/Zendo/Code"},		"ZendoPCH.h", {}) 
	project( "Zendo" )
	includedirs( {vSourceRoot .. "/Engine/ThirdParty/wxWidgets/include"} )
	includedirs( {vSourceRoot .. "/Engine/ThirdParty/wxWidgets/include/msvc"} )
	