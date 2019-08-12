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

zGfxShaderVertex zGfxShaderVertex::Create( const char* _zFilename, const char* _zEntryname, const zArray<zShaderDefine>& _aDefines)
{
	return zcExp::CreateGfxShaderVertex(_zFilename, _zEntryname, _aDefines);
}

zGfxShaderPixel zGfxShaderPixel::Create( const char* _zFilename, const char* _zEntryname, const zArray<zShaderDefine>& _aDefines)
{
	return zcExp::CreateGfxShaderPixel(_zFilename, _zEntryname, _aDefines);
}

zGfxCBuffer zGfxCBuffer::Create(const zGfxShaderPixel& _rShader, zU8 _uCBufferSlot)
{
	return zcExp::CreateGfxCBuffer(_rShader.GetResID(), _uCBufferSlot);
}

zGfxCBuffer zGfxCBuffer::Create(const zGfxShaderPixel& _rShader, zHash32 _uCBufferName)
{
	return zcExp::CreateGfxCBuffer(_rShader.GetResID(), _uCBufferName);
}

zGfxCBuffer zGfxCBuffer::Create(const zGfxShaderVertex& _rShader, zU8 _uCBufferSlot)
{
	return zcExp::CreateGfxCBuffer(_rShader.GetResID(), _uCBufferSlot);
}

zGfxCBuffer zGfxCBuffer::Create(const zGfxShaderVertex& _rShader, zHash32 _uCBufferName)
{
	return zcExp::CreateGfxCBuffer(_rShader.GetResID(), _uCBufferName);
}

//=================================================================================================
// zGfxCBuffer
//=================================================================================================
void zGfxCBuffer::SetValue(const zenRes::zShaderParameter& _Value)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetValue(_Value);
}

void zGfxCBuffer::SetValue(zHash32 _hParamName, float _fValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetValue(_hParamName, _fValue);
}

void zGfxCBuffer::SetValue(zHash32 _hParamName, zU32 _uValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetValue(_hParamName, _uValue);
}

void zGfxCBuffer::SetValue(zHash32 _hParamName, const zVec2F& _vValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetValue(_hParamName, _vValue);
}

void zGfxCBuffer::SetValue(zHash32 _hParamName, const zVec3F& _vValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetValue(_hParamName, _vValue);
}

void zGfxCBuffer::SetValue(zHash32 _hParamName, const zVec4F& _vValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetValue(_hParamName, _vValue);
}

void zGfxCBuffer::SetValue(zHash32 _hParamName, const zenMath::Matrix& _matValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetValue(_hParamName, _matValue);
}

}} // namespace zen / zenRes
