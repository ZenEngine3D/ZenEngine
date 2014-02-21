-- debug.getinfo(4).source is the filename of the file who called dofile()
-- solutionName = path.getbasename(path.getabsolute(debug.getinfo(4).source .. "/.." ))
-- currentOS = path.getbasename(debug.getinfo(4).source);
vPlatformGame 	= {"PCGame32", "PCGame64"}
vPlatformTool 	= {"PCTool32", "PCTool64"}
vPlatformAll 	= {vPlatformGame, vPlatformTool}
solution( "{Orion}" )    
    location 			( vOutputRoot )
	configurations		( {"Debug", "Release", "Final"} )
	--platforms 		( {"x32", "x64"} )
	platforms 			( vPlatformAll )
	flags				( {"Unicode"} )
	
	dofile("Engine/PRJEngine.lua")