vPlatformGame 	= {'PC64DX11','PC64DX12'}
vPlatformTool 	= {'PCTool64DX11','PCTool64DX12'}
vPlatformAll 	= {vPlatformGame, vPlatformTool}
solution( "Zen" )    
    location 			( vOutputRoot )
	configurations		{'Debug', 'Release', 'Final'}
	platforms 			( vPlatformAll )
	characterset 		( 'Unicode' )
	dofile("Engine/PRJEngine.lua")
	dofile("Tool/PRJTool.lua")