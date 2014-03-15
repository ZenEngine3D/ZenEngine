#include "libZenCore.h"
namespace zen { namespace zenRes {

//=================================================================================================
// CREATES
//=================================================================================================
zGfxShaderVertex zGfxShaderVertex::Create( const char* _zFilename, const char* _zEntryname )
{
	return EExp::CreateGfxShaderVertex(_zFilename, _zEntryname);
}

zGfxShaderPixel zGfxShaderPixel::Create( const char* _zFilename, const char* _zEntryname )
{
	return EExp::CreateGfxShaderPixel(_zFilename, _zEntryname);
}

zGfxShaderVertex zGfxShaderVertex::Create( const char* _zFilename, const char* _zEntryname, const zArrayBase<zShaderDefine>& _aDefines)
{
	return EExp::CreateGfxShaderVertex(_zFilename, _zEntryname, _aDefines);
}

zGfxShaderPixel zGfxShaderPixel::Create( const char* _zFilename, const char* _zEntryname, const zArrayBase<zShaderDefine>& _aDefines)
{
	return EExp::CreateGfxShaderPixel(_zFilename, _zEntryname, _aDefines);
}

}} // namespace zen / zenRes
