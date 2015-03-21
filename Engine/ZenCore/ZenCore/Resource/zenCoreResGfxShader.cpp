#include "zcCore.h"
namespace zen { namespace zenRes {

//=================================================================================================
// CREATES
//=================================================================================================
zGfxShaderVertex zGfxShaderVertex::Create( const char* _zFilename, const char* _zEntryname )
{
	return zcExp::CreateGfxShaderVertex(_zFilename, _zEntryname);
}

zGfxShaderPixel zGfxShaderPixel::Create( const char* _zFilename, const char* _zEntryname )
{
	return zcExp::CreateGfxShaderPixel(_zFilename, _zEntryname);
}

zGfxShaderVertex zGfxShaderVertex::Create( const char* _zFilename, const char* _zEntryname, const zArrayBase<zShaderDefine>& _aDefines)
{
	return zcExp::CreateGfxShaderVertex(_zFilename, _zEntryname, _aDefines);
}

zGfxShaderPixel zGfxShaderPixel::Create( const char* _zFilename, const char* _zEntryname, const zArrayBase<zShaderDefine>& _aDefines)
{
	return zcExp::CreateGfxShaderPixel(_zFilename, _zEntryname, _aDefines);
}

}} // namespace zen / zenRes
