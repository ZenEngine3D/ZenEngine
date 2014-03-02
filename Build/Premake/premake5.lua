vPlatform 		= "Window"
vSourceRoot 	= path.getabsolute( os.getcwd() .. "/../..")
vOutputRoot 	= path.getabsolute( vSourceRoot .. "/Build/[Projects]")

dofile("FunctionInternal.lua")
dofile("FunctionPublic.lua")
dofile("SLNZen.lua")

