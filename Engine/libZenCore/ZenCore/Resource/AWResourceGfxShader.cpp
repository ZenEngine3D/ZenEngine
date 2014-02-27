#include "libZenCore.h"
namespace zen { namespace zenRes {

//=================================================================================================
// CREATES
//=================================================================================================
GfxShaderVertex GfxShaderVertex::Create( const char* _zFilename, const char* _zEntryname )
{
	return EExp::CreateGfxShaderVertex(_zFilename, _zEntryname);
}

GfxShaderPixel GfxShaderPixel::Create( const char* _zFilename, const char* _zEntryname )
{
	return EExp::CreateGfxShaderPixel(_zFilename, _zEntryname);
}

GfxShaderVertex GfxShaderVertex::Create( const char* _zFilename, const char* _zEntryname, const zenArrayBase<awShaderDefine>& _aDefines)
{
	return EExp::CreateGfxShaderVertex(_zFilename, _zEntryname, _aDefines);
}

GfxShaderPixel GfxShaderPixel::Create( const char* _zFilename, const char* _zEntryname, const zenArrayBase<awShaderDefine>& _aDefines)
{
	return EExp::CreateGfxShaderPixel(_zFilename, _zEntryname, _aDefines);
}

}} // namespace zen / zenRes
