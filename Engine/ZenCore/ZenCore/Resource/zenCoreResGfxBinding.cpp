#include "zcCore.h"

namespace zen { namespace zenRes {

//=================================================================================================
// CREATES
//=================================================================================================
zGfxMesh zGfxMesh::Create(const zArrayBase<zGfxMeshStrip>& _aMeshStrip)
{
	//! @todo Optim: Use a heap allocator instead, for faster creation
	zArrayStatic<zResID> aMeshStripID;
	aMeshStripID.Copy<zGfxMeshStrip>(_aMeshStrip);
	return zcExp::CreateGfxMesh( aMeshStripID );
}

void zGfxMesh::Draw(zenRes::zGfxRenderPass& _rRenderpass, float _fPriority, zArrayDynamic<zenRes::zGfxDrawcall>& _aDrawcallsOut)
{
	zcRes::GfxMeshRef rMesh									= *this;
	zcRes::GfxRenderPassRef rRenderpass						= _rRenderpass;
	const zArrayStatic<zcRes::GfxMeshStripRef>& aMeshStrips	= rMesh->GetMeshStrips();
	const zUInt uOldCount									= _aDrawcallsOut.Count();
	const zUInt uStripCount									= aMeshStrips.Count();
	_aDrawcallsOut.SetCount( uOldCount + uStripCount );
	for(zUInt stripIdx(0); stripIdx<uStripCount; ++stripIdx)
		_aDrawcallsOut[uOldCount+stripIdx] = zcGfx::Drawcall::Create(rRenderpass, _fPriority, aMeshStrips[stripIdx] ).GetSafe();
}

zGfxMeshStrip zGfxMeshStrip::Create(const zGfxVertex& _VertexBuffer, const zGfxIndex& _IndexBuffer, const zGfxShaderBinding& _rShaderBinding, const zArrayBase<zResID>& _aShaderParamID, const zArrayBase<zShaderTexture>& _aTexture, zU32 _uIndexFirst, zU32 _uIndexCount)
{
	return zcExp::CreateGfxMeshStrip(_VertexBuffer, _IndexBuffer, _rShaderBinding.GetResID(), _uIndexFirst, _uIndexCount, _aShaderParamID, _aTexture);
}

zGfxMeshStrip zGfxMeshStrip::Create(const zGfxVertex& _VertexBuffer, const zGfxIndex& _IndexBuffer, const zGfxShaderBinding& _rShaderBinding, const zArrayBase<zResID>& _aShaderParamID, zU32 _uIndexFirst, zU32 _uIndexCount)
{	
	static const zArrayStatic<zShaderTexture>	aTexture(zUInt(0));
	return zcExp::CreateGfxMeshStrip(_VertexBuffer, _IndexBuffer, _rShaderBinding.GetResID(), _uIndexFirst, _uIndexCount, _aShaderParamID, aTexture);
}

zGfxMeshStrip zGfxMeshStrip::Create(const zGfxVertex& _VertexBuffer, const zGfxIndex& _IndexBuffer, const zGfxShaderBinding& _rShaderBinding, zU32 _uIndexFirst, zU32 _uIndexCount)
{
	static const zArrayStatic<zResID>	aShaderParamID(zUInt(0));
	static const zArrayStatic<zShaderTexture>	aTexture(zUInt(0));
	return zcExp::CreateGfxMeshStrip(_VertexBuffer, _IndexBuffer, _rShaderBinding.GetResID(), _uIndexFirst, _uIndexCount, aShaderParamID, aTexture);
}

void zGfxMeshStrip::Draw(zenRes::zGfxRenderPass& _rRenderpass, float _fPriority, zArrayDynamic<zenRes::zGfxDrawcall>& _aDrawcallsOut)
{
	zcRes::GfxRenderPassRef rRenderpass	= _rRenderpass;
	zcRes::GfxMeshStripRef rMeshStrip	= *this;
	_aDrawcallsOut.Push( zcGfx::Drawcall::Create(rRenderpass, _fPriority, rMeshStrip ).GetSafe() );
}

zGfxWindow zGfxWindow::Create( HWND _WindowHandle )
{
	zEngineRef<zcRes::GfxWindowResData> rResData	= zenNewDefault zcRes::GfxWindowResData();
	rResData->mResID								= zcMgr::Export.GetNewResourceID( zenConst::keResType_GfxWindow );
	rResData->mhWindow								= _WindowHandle;
	return zcRes::GfxWindow::RuntimeCreate(rResData);
}

zGfxShaderBinding zGfxShaderBinding::Create(const zArrayBase<zenRes::zGfxShader>& _aShaderID)
{
	zArrayStatic<zResID> aShaderID;
	aShaderID.Copy<zenRes::zGfxShader>(_aShaderID);	
	return zcExp::CreateGfxShaderBinding(aShaderID);
}

//=================================================================================================
// GFX WINDOW
//=================================================================================================
//! @todo Missing: Convert all mpResouce cast to using Ref Object
zGfxRenderTarget zGfxWindow::GetBackbuffer()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxWindowRef rWindow = mpResource;	
	return rWindow->GetBackbuffer();
}

void zGfxWindow::Resize(const zVec2U16& _vSize)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxWindow* pWindow = static_cast<zcRes::GfxWindow*>(mpResource);
	return pWindow->Resize(_vSize);
}

bool zGfxWindow::HasPendingResize()
{
	zcRes::GfxWindowRef rWindow = mpResource;
	return !rWindow->GetProxy()->mvPendingResize.IsNull();
}

void zGfxWindow::FrameBegin()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxWindow* pWindow = static_cast<zcRes::GfxWindow*>(mpResource);
	zcMgr::GfxRender.FrameBegin(pWindow);
}

void zGfxWindow::FrameEnd()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxWindow* pWindow = static_cast<zcRes::GfxWindow*>(mpResource);
	zcMgr::GfxRender.FrameEnd();
}

//=================================================================================================
// GFX VIEW
//=================================================================================================
zVec2U16 zGfxView::GetDim()const
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	const zcRes::GfxView* pView = static_cast<const zcRes::GfxView*>(mpResource);	
	return pView->GetDim();
}

//=================================================================================================
// GFX MESH
//=================================================================================================
void zGfxMesh::SetValue(const zShaderParameter& _Value)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMesh* rMesh = static_cast<zcRes::GfxMesh*>(mpResource);
	rMesh->SetValue(_Value);
}
void zGfxMesh::SetValue(const zArrayBase<const zShaderParameter*>& _aValues)	
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMesh* pMesh = static_cast<zcRes::GfxMesh*>(mpResource);
	pMesh->SetValue(_aValues);
}

void zGfxMesh::SetValue(const zHash32& _hParamName, const float& _fValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMesh* pMesh = static_cast<zcRes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _fValue);
}

void zGfxMesh::SetValue(const zHash32& _hParamName, const zVec2F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMesh* pMesh = static_cast<zcRes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void zGfxMesh::SetValue(const zHash32& _hParamName, const zVec3F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMesh* pMesh = static_cast<zcRes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void zGfxMesh::SetValue(const zHash32& _hParamName, const zVec4F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMesh* pMesh = static_cast<zcRes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void zGfxMesh::SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMesh* pMesh = static_cast<zcRes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _matValue);
}

void zGfxMesh::SetValue(const zHash32& _hTextureName, zGfxTexture2d _rTexture )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMesh* pMesh = static_cast<zcRes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture);
}

void zGfxMesh::SetValue(const zHash32& _hTextureName, zGfxSampler _rSampler )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMesh* pMesh = static_cast<zcRes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hTextureName, _rSampler);
}

void zGfxMesh::SetValue(const zHash32& _hTextureName, zGfxTexture2d _rTexture, zGfxSampler _rSampler )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMesh* pMesh = static_cast<zcRes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture, _rSampler);
}

//=================================================================================================
// GFX MESH STRIP
//=================================================================================================
void zGfxMeshStrip::SetValue(const zShaderParameter& _Value)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshStrip* rMesh = static_cast<zcRes::GfxMeshStrip*>(mpResource);
	rMesh->SetValue(_Value);
}
void zGfxMeshStrip::SetValue(const zArrayBase<const zShaderParameter*>& _aValues)	
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshStrip* pMesh = static_cast<zcRes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_aValues);
}

void zGfxMeshStrip::SetValue(const zHash32& _hParamName, const float& _fValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshStrip* pMesh = static_cast<zcRes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _fValue);
}

void zGfxMeshStrip::SetValue(const zHash32& _hParamName, const zVec2F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshStrip* pMesh = static_cast<zcRes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void zGfxMeshStrip::SetValue(const zHash32& _hParamName, const zVec3F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshStrip* pMesh = static_cast<zcRes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void zGfxMeshStrip::SetValue(const zHash32& _hParamName, const zVec4F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshStrip* pMesh = static_cast<zcRes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void zGfxMeshStrip::SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshStrip* pMesh = static_cast<zcRes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _matValue);
}

void zGfxMeshStrip::SetValue(const zHash32& _hTextureName, zGfxTexture2d _rTexture )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshStrip* pMesh = static_cast<zcRes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture);
}

void zGfxMeshStrip::SetValue(const zHash32& _hTextureName, zGfxSampler _rSampler )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshStrip* pMesh = static_cast<zcRes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hTextureName, _rSampler);
}

void zGfxMeshStrip::SetValue(const zHash32& _hTextureName, zGfxTexture2d _rTexture, zGfxSampler _rSampler )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshStrip* pMesh = static_cast<zcRes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture, _rSampler);
}
}} // namespace zen, zenRes
