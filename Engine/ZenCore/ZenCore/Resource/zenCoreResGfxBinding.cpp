#include "zcCore.h"
#include "ZenExternal/UI/zxUIImgui.h"

namespace zen { namespace zenRes {

//=================================================================================================
// CREATES
//=================================================================================================
zGfxMesh zGfxMesh::Create(const zArray<zGfxMeshStrip>& _aMeshStrip)
{
	//! @todo Optim: Use a heap allocator instead, for faster creation
	zArrayDyn<zResID> aMeshStripID;
	aMeshStripID.resize( _aMeshStrip.size() );
	for( zUInt idx(0), count(aMeshStripID.size()); idx<count; ++idx)
		aMeshStripID[idx] = _aMeshStrip[idx].GetResID();
	return zcExp::CreateGfxMesh( aMeshStripID );
}

zGfxMesh zGfxMesh::Create(std::initializer_list<zGfxMeshStrip> _aMeshStrip)
{
	zArrayDyn<zResID> aMeshStripID;
	const zGfxMeshStrip* aStripArray = _aMeshStrip.begin();
	aMeshStripID.resize( _aMeshStrip.size() );
	for( zUInt idx(0), count(aMeshStripID.size()); idx<count; ++idx)
		aMeshStripID[idx] = aStripArray[idx].GetResID();
	return zcExp::CreateGfxMesh( aMeshStripID );
}

zGfxMeshStrip zGfxMeshStrip::Create(const zGfxIndex& _IndexBuffer, const zGfxShaderBinding& _rShaderBinding, const zArray<zShaderResource>& _aResources, zU32 _uIndexFirst, zU32 _uIndexCount, zU32 _uVertexFirst)
{
	return zcExp::CreateGfxMeshStrip( _IndexBuffer.GetResID(), _rShaderBinding.GetResID(), _uIndexFirst, _uIndexCount, _uVertexFirst, _aResources);
}

zGfxMeshStrip zGfxMeshStrip::Create(const zGfxIndex& _IndexBuffer, const zGfxShaderBinding& _rShaderBinding, zU32 _uIndexFirst, zU32 _uIndexCount, zU32 _uVertexFirst)
{	
	static const zArrayDyn<zShaderResource> saResourceNone;
	return zcExp::CreateGfxMeshStrip( _IndexBuffer.GetResID(), _rShaderBinding.GetResID(), _uIndexFirst, _uIndexCount, _uVertexFirst, saResourceNone);
}

zGfxShaderBinding zGfxShaderBinding::Create(const zGfxShaderVertex& _VertexShader)
{
	zArrayDyn<zResID> aShaderID;
	aShaderID.resize(1);
	aShaderID[0] = _VertexShader.GetResID();
	return zcExp::CreateGfxShaderBinding(aShaderID);
}

zGfxShaderBinding zGfxShaderBinding::Create(const zGfxShaderVertex& _VertexShader, const zGfxShaderPixel& _PixelShader)
{
	zArrayDyn<zResID> aShaderID;
	aShaderID.resize(2);
	aShaderID[0] = _VertexShader.GetResID();
	aShaderID[1] = _PixelShader.GetResID();
	return zcExp::CreateGfxShaderBinding(aShaderID);
}

//=================================================================================================
// GFX WINDOW
//=================================================================================================
//! @todo Missing: Convert all mpResource cast to using Ref Object
zGfxTarget2D zGfxWindow::GetBackbuffer()
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	return mpResource->GetBackbuffer();
}

void zGfxWindow::Resize(const zVec2U16& _vSize)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	return mpResource->Resize(_vSize);
}

bool zGfxWindow::PerformResize()
{	
	zenAssertMsg(mpResource, "No valid resource assigned");
	return mpResource->PerformResize();		
}

void zGfxWindow::FrameBegin()
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->FrameBegin();
}

void zGfxWindow::FrameEnd()
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->FrameEnd();
}

//=================================================================================================
// GFX VIEW
//=================================================================================================
zVec2U16 zGfxView::GetDim()const
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	const zcRes::GfxView* pView = static_cast<const zcRes::GfxView*>(mpResource);	
	return pView->GetDim();
}

//=================================================================================================
// GFX SHADER BINDING
//=================================================================================================
void zGfxShaderBinding::CreateShaderParam(zArrayDyn<zenRes::zGfxCBuffer>& _aShaderParamOut)const
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->CreateShaderParam(_aShaderParamOut);
}

//=================================================================================================
// GFX MESH
//=================================================================================================
void zGfxMesh::SetValue(const zShaderParameter& _Value)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetValue(_Value);
}
void zGfxMesh::SetValue(const zArray<const zShaderParameter*>& _aValues)	
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetValue(_aValues);
}

void zGfxMesh::SetValue(zHash32 _hParamName, const float& _fValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMesh* pMesh = static_cast<zcRes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _fValue);
}

void zGfxMesh::SetValue(zHash32 _hParamName, const zVec2F& _vValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetValue(_hParamName, _vValue);
}

void zGfxMesh::SetValue(zHash32 _hParamName, const zVec3F& _vValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetValue(_hParamName, _vValue);
}

void zGfxMesh::SetValue(zHash32 _hParamName, const zVec4F& _vValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetValue(_hParamName, _vValue);
}

void zGfxMesh::SetValue(zHash32 _hParamName, const zenMath::Matrix& _matValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetValue(_hParamName, _matValue);
}

void zGfxMesh::SetResource(zHash32 _hTextureName, const zGfxTexture2D& _rTexture, zU16 _uIndex )
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetResource(_hTextureName, _rTexture, _uIndex);
}

void zGfxMesh::SetResource(zHash32 _hTextureName, const zGfxStateSampler& _rSampler, zU16 _uIndex )
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetResource(_hTextureName, _rSampler, _uIndex);
}

void zGfxMesh::SetResource(zHash32 _hBufferName, const zGfxBuffer& _rBuffer, zU16 _uIndex )
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetResource(_hBufferName, _rBuffer, _uIndex);
}

//=================================================================================================
// GFX MESH STRIP
//=================================================================================================
void zGfxMeshStrip::SetValue(const zShaderParameter& _Value)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetValue(_Value);
}
void zGfxMeshStrip::SetValue(const zArray<const zShaderParameter*>& _aValues)	
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetValue(_aValues);
}

void zGfxMeshStrip::SetValue(zHash32 _hParamName, const float& _fValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetValue(_hParamName, _fValue);
}

void zGfxMeshStrip::SetValue(zHash32 _hParamName, const zVec2F& _vValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetValue(_hParamName, _vValue);
}

void zGfxMeshStrip::SetValue(zHash32 _hParamName, const zVec3F& _vValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetValue(_hParamName, _vValue);
}

void zGfxMeshStrip::SetValue(zHash32 _hParamName, const zVec4F& _vValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetValue(_hParamName, _vValue);
}

void zGfxMeshStrip::SetValue(zHash32 _hParamName, const zenMath::Matrix& _matValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetValue(_hParamName, _matValue);
}

void zGfxMeshStrip::SetResource(zHash32 _hTextureName, const zGfxTexture2D& _rTexture, zU16 _uIndex )
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetResource(_hTextureName, _rTexture, _uIndex);
}

void zGfxMeshStrip::SetResource(zHash32 _hTextureName, const zGfxStateSampler& _rSampler, zU16 _uIndex )
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetResource(_hTextureName, _rSampler, _uIndex);
}

void zGfxMeshStrip::SetResource(zHash32 _hBufferName, const zGfxBuffer& _rBuffer, zU16 _uIndex )
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->SetResource(_hBufferName, _rBuffer, _uIndex);
}

}} // namespace zen, zenRes
