#include "zcCore.h"
#include "Engine/ZenExternal/UI/zxUIImgui.h"

namespace zen { namespace zenRes {

//=================================================================================================
// CREATES
//=================================================================================================
zGfxMesh zGfxMesh::Create(const zArrayBase<zGfxMeshStrip>& _aMeshStrip)
{
	//! @todo Optim: Use a heap allocator instead, for faster creation
	zArrayStatic<zResID> aMeshStripID;
	aMeshStripID.SetCount( _aMeshStrip.Count() );
	for( zUInt idx(0), count(aMeshStripID.Count()); idx<count; ++idx)
		aMeshStripID[idx] = _aMeshStrip[idx].GetResID();
	return zcExp::CreateGfxMesh( aMeshStripID );
}
/*
void zGfxMesh::Draw(zenRes::zGfxRenderPass& _rRenderpass, float _fPriority, zArrayDynamic<zenGfx::zCommand>& _aDrawcallsOut, const zVec4U16& _vScreenScissor)
{
	zcRes::GfxMeshRef rMesh									= *this;
	zcRes::GfxRenderPassRef rRenderpass						= _rRenderpass;
	const zArrayStatic<zcRes::GfxMeshStripRef>& aMeshStrips	= rMesh->GetMeshStrips();
	const zUInt uOldCount									= _aDrawcallsOut.Count();
	const zUInt uStripCount									= aMeshStrips.Count();
	_aDrawcallsOut.SetCount( uOldCount + uStripCount );
	for(zUInt stripIdx(0); stripIdx<uStripCount; ++stripIdx)
		_aDrawcallsOut[uOldCount+stripIdx] = zcGfx::CommandDraw::Create(rRenderpass, _fPriority, aMeshStrips[stripIdx], 0, 0xFFFFFFFF, _vScreenScissor).GetSafe();
}
*/
zGfxMeshStrip zGfxMeshStrip::Create(const zGfxVertex& _VertexBuffer, const zGfxIndex& _IndexBuffer, const zGfxShaderBinding& _rShaderBinding, const zArrayBase<zGfxShaderParam>& _arShaderParam, const zArrayBase<zShaderTexture>& _aTexture, zU32 _uIndexFirst, zU32 _uIndexCount, zU32 _uVertexFirst)
{
	zArrayStatic<zResID> aShaderParamID(zUInt(_arShaderParam.Count()));
	for(int idx(0), count(_arShaderParam.Count()); idx<count; ++idx)
		aShaderParamID[idx] = _arShaderParam[idx].GetResID();
	return zcExp::CreateGfxMeshStrip(_VertexBuffer.GetResID(), _IndexBuffer.GetResID(), _rShaderBinding.GetResID(), _uIndexFirst, _uIndexCount, _uVertexFirst, aShaderParamID, _aTexture);
}

zGfxMeshStrip zGfxMeshStrip::Create(const zGfxVertex& _VertexBuffer, const zGfxIndex& _IndexBuffer, const zGfxShaderBinding& _rShaderBinding, const zArrayBase<zGfxShaderParam>& _arShaderParam, zU32 _uIndexFirst, zU32 _uIndexCount, zU32 _uVertexFirst)
{	
	static const zArrayStatic<zShaderTexture> aTexture(zUInt(0));
	zArrayStatic<zResID> aShaderParamID(zUInt(_arShaderParam.Count()));
	for(zUInt idx(0), count(_arShaderParam.Count()); idx<count; ++idx)
		aShaderParamID[idx] = _arShaderParam[idx].GetResID();
	return zcExp::CreateGfxMeshStrip(_VertexBuffer.GetResID(), _IndexBuffer.GetResID(), _rShaderBinding.GetResID(), _uIndexFirst, _uIndexCount, _uVertexFirst, aShaderParamID, aTexture);
}

zGfxMeshStrip zGfxMeshStrip::Create(const zGfxVertex& _VertexBuffer, const zGfxIndex& _IndexBuffer, const zGfxShaderBinding& _rShaderBinding, zU32 _uIndexFirst, zU32 _uIndexCount, zU32 _uVertexFirst)
{
	static const zArrayStatic<zResID>			aShaderParamID(zUInt(0));
	static const zArrayStatic<zShaderTexture>	aTexture(zUInt(0));
	return zcExp::CreateGfxMeshStrip(_VertexBuffer.GetResID(), _IndexBuffer.GetResID(), _rShaderBinding.GetResID(), _uIndexFirst, _uIndexCount, _uVertexFirst, aShaderParamID, aTexture);
}
/*
void zGfxMeshStrip::Draw(zenRes::zGfxRenderPass& _rRenderpass, float _fPriority, zArrayDynamic<zenGfx::zCommand>& _aDrawcallsOut, zU32 _uIndexFirst, zU32 _uIndexCount, const zVec4U16& _vScreenScissor )
{
	zcRes::GfxRenderPassRef rRenderpass	= _rRenderpass;
	zcRes::GfxMeshStripRef rMeshStrip	= *this;
	_aDrawcallsOut.Push(zcGfx::CommandDraw::Create(rRenderpass, _fPriority, rMeshStrip, _uIndexFirst, _uIndexCount, _vScreenScissor).GetSafe());
}*/
/*
zGfxWindow zGfxWindow::Create( HWND _WindowHandle )
{
	return zcRes::GfxWindow::RuntimeCreate(_WindowHandle);
}
*/
zGfxShaderBinding zGfxShaderBinding::Create(const zGfxShaderVertex& _VertexShader)
{
	zArrayStatic<zResID> aShaderID;
	aShaderID.SetCount(1);
	aShaderID[0] = _VertexShader.GetResID();
	return zcExp::CreateGfxShaderBinding(aShaderID);
}

zGfxShaderBinding zGfxShaderBinding::Create(const zGfxShaderVertex& _VertexShader, const zGfxShaderPixel& _PixelShader)
{
	zArrayStatic<zResID> aShaderID;
	aShaderID.SetCount(2);
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
	zcRes::GfxWindow* pWindow = static_cast<zcRes::GfxWindow*>(mpResource);
	pWindow->FrameBegin();
}

void zGfxWindow::FrameEnd()
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxWindow* pWindow = static_cast<zcRes::GfxWindow*>(mpResource);
	pWindow->FrameEnd();
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
void zGfxShaderBinding::CreateShaderParam(zArrayStatic<zenRes::zGfxShaderParam>& _aShaderParamOut)const
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
	zcRes::GfxMesh* rMesh = static_cast<zcRes::GfxMesh*>(mpResource);
	rMesh->SetValue(_Value);
}
void zGfxMesh::SetValue(const zArrayBase<const zShaderParameter*>& _aValues)	
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMesh* pMesh = static_cast<zcRes::GfxMesh*>(mpResource);
	pMesh->SetValue(_aValues);
}

void zGfxMesh::SetValue(const zHash32& _hParamName, const float& _fValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMesh* pMesh = static_cast<zcRes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _fValue);
}

void zGfxMesh::SetValue(const zHash32& _hParamName, const zVec2F& _vValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMesh* pMesh = static_cast<zcRes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void zGfxMesh::SetValue(const zHash32& _hParamName, const zVec3F& _vValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMesh* pMesh = static_cast<zcRes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void zGfxMesh::SetValue(const zHash32& _hParamName, const zVec4F& _vValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMesh* pMesh = static_cast<zcRes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void zGfxMesh::SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMesh* pMesh = static_cast<zcRes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _matValue);
}

void zGfxMesh::SetValue(const zHash32& _hTextureName, const zGfxTexture2d& _rTexture )
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMesh* pMesh = static_cast<zcRes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture);
}

void zGfxMesh::SetValue(const zHash32& _hTextureName, const zGfxSampler& _rSampler )
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMesh* pMesh = static_cast<zcRes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hTextureName, _rSampler);
}

void zGfxMesh::SetValue(const zHash32& _hTextureName, const zGfxTexture2d& _rTexture, const zGfxSampler& _rSampler )
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMesh* pMesh = static_cast<zcRes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture, _rSampler);
}

//=================================================================================================
// GFX MESH STRIP
//=================================================================================================
void zGfxMeshStrip::SetValue(const zShaderParameter& _Value)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshStrip* rMesh = static_cast<zcRes::GfxMeshStrip*>(mpResource);
	rMesh->SetValue(_Value);
}
void zGfxMeshStrip::SetValue(const zArrayBase<const zShaderParameter*>& _aValues)	
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshStrip* pMesh = static_cast<zcRes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_aValues);
}

void zGfxMeshStrip::SetValue(const zHash32& _hParamName, const float& _fValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshStrip* pMesh = static_cast<zcRes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _fValue);
}

void zGfxMeshStrip::SetValue(const zHash32& _hParamName, const zVec2F& _vValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshStrip* pMesh = static_cast<zcRes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void zGfxMeshStrip::SetValue(const zHash32& _hParamName, const zVec3F& _vValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshStrip* pMesh = static_cast<zcRes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void zGfxMeshStrip::SetValue(const zHash32& _hParamName, const zVec4F& _vValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshStrip* pMesh = static_cast<zcRes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void zGfxMeshStrip::SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshStrip* pMesh = static_cast<zcRes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _matValue);
}

void zGfxMeshStrip::SetValue(const zHash32& _hTextureName, const zGfxTexture2d& _rTexture )
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshStrip* pMesh = static_cast<zcRes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture);
}

void zGfxMeshStrip::SetValue(const zHash32& _hTextureName, const zGfxSampler& _rSampler )
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshStrip* pMesh = static_cast<zcRes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hTextureName, _rSampler);
}

void zGfxMeshStrip::SetValue(const zHash32& _hTextureName, const zGfxTexture2d& _rTexture, const zGfxSampler& _rSampler )
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshStrip* pMesh = static_cast<zcRes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture, _rSampler);
}

}} // namespace zen, zenRes
