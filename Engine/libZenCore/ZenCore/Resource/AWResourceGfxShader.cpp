#include "libZenCore.h"
namespace zen { namespace awres {

//=================================================================================================
// CREATES
//=================================================================================================
awGfxShaderVertex awGfxShaderVertex::Create( const char* _zFilename, const char* _zEntryname )
{
	return EExp::CreateGfxShaderVertex(_zFilename, _zEntryname);
}

awGfxShaderPixel awGfxShaderPixel::Create( const char* _zFilename, const char* _zEntryname )
{
	return EExp::CreateGfxShaderPixel(_zFilename, _zEntryname);
}

awGfxShaderVertex awGfxShaderVertex::Create( const char* _zFilename, const char* _zEntryname, const zenArrayBase<awShaderDefine>& _aDefines)
{
	return EExp::CreateGfxShaderVertex(_zFilename, _zEntryname, _aDefines);
}

awGfxShaderPixel awGfxShaderPixel::Create( const char* _zFilename, const char* _zEntryname, const zenArrayBase<awShaderDefine>& _aDefines)
{
	return EExp::CreateGfxShaderPixel(_zFilename, _zEntryname, _aDefines);
}

}} // namespace zen / awres
