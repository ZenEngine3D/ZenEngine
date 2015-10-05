#include "zcCore.h"

namespace zen { namespace zenRes {

//! @todo Urgent move elsewhere, not a resource per see
zGfxDrawcall::zGfxDrawcall(const zGfxDrawcall& _Copy)
{
	*this = _Copy;
}

zGfxDrawcall::zGfxDrawcall()
{
}

zGfxDrawcall::zGfxDrawcall(zcGfx::Drawcall* _pDrawcall)
{
	*this = _pDrawcall;
}

zGfxDrawcall zGfxDrawcall::Create(const zGfxRenderPass& _rRenderPass, const zGfxMeshStrip& _rMeshStrip, float _fPriority)
{
	zcRes::GfxRenderPassRef rRenderpass	= _rRenderPass;
	zcRes::GfxMeshStripRef rMeshStrip	= _rMeshStrip;
	zGfxDrawcall rDrawcall				= zcGfx::Drawcall::Create(rRenderpass, rMeshStrip, _fPriority ).GetSafe();	
	return rDrawcall;
}

void zGfxDrawcall::Submit(zArrayDynamic<zenRes::zGfxDrawcall>& _aDrawcalls)
{
	zcMgr::GfxRender.Render( _aDrawcalls );
}

zGfxDrawcall& zGfxDrawcall::operator=(const zGfxDrawcall& _Copy)
{	
	Super::operator=(_Copy.Get());
	return *this;
}

zGfxDrawcall& zGfxDrawcall::operator=(zcGfx::Drawcall* _pCopy)
{	
	Super::operator=(_pCopy);
	return *this;
}

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

zGfxMesh zGfxMesh::Create(const zGfxMeshStrip* _aMeshStrip, zUInt _uMeshStripCount)
{
	//! @todo Optim: Use a heap allocator instead, for faster creation
	zArrayStatic<zResID> aMeshStripID;
	aMeshStripID.Copy<zGfxMeshStrip>(_aMeshStrip, _uMeshStripCount);
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
		_aDrawcallsOut[uOldCount+stripIdx] = zcGfx::Drawcall::Create(rRenderpass, aMeshStrips[stripIdx], _fPriority ).GetSafe();
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
	_aDrawcallsOut.Push( zcGfx::Drawcall::Create(rRenderpass, rMeshStrip, _fPriority ).GetSafe() );
}

zGfxView zGfxView::Create( const zGfxRenderTarget& _RTColor, const zGfxRenderTarget& _RTDepth, const zVec2U16& _vDim, const zVec2S16& _vOrigin )
{
	return zcExp::CreateGfxView(_RTColor.GetResID(), _RTDepth.GetResID(), _vDim, _vOrigin);
}

zGfxView zGfxView::Create( const zArrayBase<zGfxRenderTarget>& _aRTColor, const zGfxRenderTarget& _RTDepth, const zVec2U16& _vDim, const zVec2S16& _vOrigin )
{
	zArrayStatic<zResID> aRTColorID;
	aRTColorID.Copy<zGfxRenderTarget>(_aRTColor);
	return zcExp::CreateGfxView(aRTColorID, _RTDepth.GetResID(), _vDim, _vOrigin);
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

zGfxShaderBinding zGfxShaderBinding::Create(const zenRes::zGfxShader* _pShaders, zUInt _uCount)
{
	zArrayStatic<zResID> aShaderID;
	aShaderID.Copy<zenRes::zGfxShader>(_pShaders, _uCount);	
	return zcExp::CreateGfxShaderBinding(aShaderID);
}
//=================================================================================================
// GFX RenderPass
//! @todo Cleanup move this to own file
//=================================================================================================
zGfxRenderPass zGfxRenderPass::Create(const zString& _zStageName, const zGfxStateBlend& _rStateBlend, const zGfxStateDepthStencil& _rStateDepthStencil, const zGfxStateRasterizer& _rStateRaster, const zGfxView& _rView)
{
	return zcExp::CreateGfxRenderPass(_zStageName, _rStateBlend.GetResID(), _rStateDepthStencil.GetResID(), _rStateRaster.GetResID(), _rView.GetResID());
}

zGfxRenderPass zGfxRenderPass::Create(const zString& _zStageName, const ViewConfig& _rView, const zGfxTexture2d& _rTargetDepth, const zGfxStateDepthStencil& _rStateDepthStencil, const zGfxStateRasterizer& _rStateRaster)
{/*
	zArrayStatic<ViewConfig> aViews(1);
	aViews[0] = _rView;
	return zcExp::CreateGfxRenderPass(_zStageName, _rStateBlend.GetResID(), _rStateDepthStencil.GetResID(), _rStateRaster.GetResID(), _rView.GetResID());
	*/
	return NULL;
}
/*
struct ViewConfig
		{
			zGfxTexture2d	mTargetSurface;
			zGfxStateBlend	mStateBlend;
			zVec2S16		mvOrigin;
			zVec2U16		mvDim;
		};
		static zGfxRenderPass			Create(const zString& _zStageName, const zGfxStateBlend& _rStateBlend, const zGfxStateDepthStencil&	_rStateDepthStencil, const zGfxStateRasterizer& _rStateRaster, const zGfxView& _rView);
		static zGfxRenderPass			Create(const zString& _zStageName, const ViewConfig& _View, const zGfxTexture2d& _TargetDepth, const zGfxStateDepthStencil& _rStateDepthStencil, const zGfxStateRasterizer& _rStateRaster);
*/

//=================================================================================================
// GFX WINDOW
//=================================================================================================
//! @todo Missing: Convert all mpResouce cast to using Ref Object
zGfxView zGfxWindow::GetBackbuffer()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxWindow* pWindow = static_cast<zcRes::GfxWindow*>(mpResource);
	return pWindow->GetBackbuffer();
}

void zGfxWindow::Resize(const zVec2U16& _vSize)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxWindow* pWindow = static_cast<zcRes::GfxWindow*>(mpResource);
	return pWindow->Resize(_vSize);
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
void zGfxView::Clear( bool _bClearColor, const zVec4F& _vRGBA, bool _bClearDepth, float _fDepth, bool _bClearStencil, zU8 _uStencil )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxViewRef rView = mpResource;
	rView->Clear(_bClearColor, _vRGBA, _bClearDepth, _fDepth, _bClearStencil, _uStencil);
}

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
