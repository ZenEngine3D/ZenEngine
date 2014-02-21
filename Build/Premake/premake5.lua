vPlatform 		= "Window"
vSourceRoot 	= path.getabsolute( os.getcwd() .. "/../..")
vOutputRoot 	= path.getabsolute( vSourceRoot .. "/Build/{Solution}")

dofile("FunctionInternal.lua")
dofile("FunctionPublic.lua")
dofile("SLNOrion.lua")
