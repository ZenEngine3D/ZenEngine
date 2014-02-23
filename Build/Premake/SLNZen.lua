vPlatformGame 	= {"PCGame32", "PCGame64"}
vPlatformTool 	= {"PCTool32", "PCTool64"}
vPlatformAll 	= {vPlatformGame, vPlatformTool}

solution( "Zen" )    
    location 			( vOutputRoot )
	configurations		( {"Debug", "Release", "Final"} )
	platforms 			( vPlatformAll )
	flags				( {"Unicode"} )
	
	dofile("Engine/PRJEngine.lua")