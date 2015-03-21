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

zGfxMesh zGfxMesh::Create(const zGfxMeshStrip* _aMeshStrip, zUInt _uMeshStripCount)
{
	//! @todo Optim: Use a heap allocator instead, for faster creation
	zArrayStatic<zResID> aMeshStripID;
	aMeshStripID.Copy<zGfxMeshStrip>(_aMeshStrip, _uMeshStripCount);
	return zcExp::CreateGfxMesh( aMeshStripID );
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

zGfxSampler zGfxSampler::Create( zenConst::eTextureFiltering _eFilterMin, zenConst::eTextureFiltering _eFilterMag, zenConst::eTextureWrap _eWrapU, zenConst::eTextureWrap _eWrapV, float _fLodBias, const zVec4F& _vBorderColor )
{
	return zcExp::CreateGfxSampler(_eFilterMin, _eFilterMag, _eWrapU, _eWrapV, _fLodBias, _vBorderColor);
}

zGfxBlend zGfxBlend::Create( zenType::awBlendDesc::awRTBlendDesc* _pxBlendDesc, zU8 _uRenderTargets, bool _bAlphaToCoverageEnable, bool _bIndependentBlendEnable )
{
	return zcExp::CreateGfxBlend(_pxBlendDesc, _uRenderTargets, _bAlphaToCoverageEnable, _bIndependentBlendEnable);
}

zGfxDepthStencil zGfxDepthStencil::Create( bool _bDepthEnable, bool _bDepthWrite, bool _bStencilEnable, zU8 _uStencilReadMask, zU8 _uStencilWriteMask, zenConst::eComparisonFunc _eDepthFunc, zenType::awDepthStencilDesc::DepthStencilOp _xFrontFace, zenType::awDepthStencilDesc::DepthStencilOp _xBackFace )
{
	return zcExp::CreateGfxDepthStencil(_bDepthEnable, _bDepthWrite, _bStencilEnable, _uStencilReadMask, _uStencilWriteMask, _eDepthFunc, _xFrontFace, _xBackFace);
}

zGfxRasterizer zGfxRasterizer::Create( bool _bFrontCounterClockwise, bool _bDepthClipEnable, bool _bScissorEnable, bool _bMultisampleEnable, bool _bAntialiasedLineEnable, bool _bWireFrame, zenConst::eCullMode _eCullMode, zI32 _iDepthBias, float _fDepthBiasClamp, float _fSlopeScaledDepthBias )
{
	return zcExp::CreateGfxRasterizer(_bFrontCounterClockwise, _bDepthClipEnable, _bScissorEnable, _bMultisampleEnable, _bAntialiasedLineEnable, _bWireFrame, _eCullMode, _iDepthBias, _fDepthBiasClamp, _fSlopeScaledDepthBias);
}

zGfxView zGfxView::Create( const zGfxRenderTarget& _RTColor, const zGfxRenderTarget& _RTDepth, const zVec2U16& _vDim, const zVec2U16& _vOrigin )
{
	return zcExp::CreateGfxView(_RTColor.GetResID(), _RTDepth.GetResID(), _vDim, _vOrigin);
}

zGfxView zGfxView::Create( const zArrayBase<zGfxRenderTarget>& _aRTColor, const zGfxRenderTarget& _RTDepth, const zVec2U16& _vDim, const zVec2U16& _vOrigin )
{
	zArrayStatic<zResID> aRTColorID;
	aRTColorID.Copy<zGfxRenderTarget>(_aRTColor);
	return zcExp::CreateGfxView(aRTColorID, _RTDepth.GetResID(), _vDim, _vOrigin);
}

zGfxWindow zGfxWindow::Create( HWND _WindowHandle )
{
	return zcExp::CreateGfxWindow(_WindowHandle);
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
// GFX WINDOW
//=================================================================================================
//! @todo Missing: Convert all mpResouce cast to using Ref Object
zGfxView zGfxWindow::GetBackbuffer()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxWindow* pWindow = static_cast<zcRes::zGfxWindow*>(mpResource);
	return pWindow->GetBackbuffer();
}

void zGfxWindow::Resize(const zVec2U16& _vSize)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxWindow* pWindow = static_cast<zcRes::zGfxWindow*>(mpResource);
	return pWindow->Resize(_vSize);
}

void zGfxWindow::FrameBegin()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxWindowRef& rWindow = *static_cast<zcRes::GfxWindowRef*>(this);
	EMgr::GfxRender.FrameBegin(rWindow);
	EMgr::GfxState.PipelineReset();
}

void zGfxWindow::FrameEnd()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxWindowRef& rWindow = *static_cast<zcRes::GfxWindowRef*>(this);
	EMgr::GfxRender.FrameEnd();
}

//=================================================================================================
// GFX VIEW
//=================================================================================================
void zGfxView::Clear( bool _bClearColor, const zVec4F& _vRGBA, bool _bClearDepth, float _fDepth, bool _bClearStencil, zU8 _uStencil )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxView* pView = static_cast<zcRes::zGfxView*>(mpResource);
	pView->Clear(_bClearColor, _vRGBA, _bClearDepth, _fDepth, _bClearStencil, _uStencil);
}

void zGfxView::ActivateView()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxViewRef& rView = *static_cast<zcRes::GfxViewRef*>(this);
	EMgr::GfxState.SetView( rView );
}

zVec2U16 zGfxView::GetDim()const
{
	const zcRes::GfxViewRef& rView = *static_cast<const zcRes::GfxViewRef*>(this);
	ZENAssertMsg(rView.IsValid(), "No valid resource assigned");
	return rView->Get().mvDim;
}

//=================================================================================================
// GFX MESH
//=================================================================================================
void zGfxMesh::RenderMesh()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshRef& rMesh = *static_cast<zcRes::GfxMeshRef*>(this);
	EMgr::GfxRender.Render( rMesh );
}

void zGfxMesh::SetValue(const zShaderParameter& _Value)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxMesh* rMesh = static_cast<zcRes::zGfxMesh*>(mpResource);
	rMesh->SetValue(_Value);
}
void zGfxMesh::SetValue(const zArrayBase<const zShaderParameter*>& _aValues)	
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxMesh* pMesh = static_cast<zcRes::zGfxMesh*>(mpResource);
	pMesh->SetValue(_aValues);
}

void zGfxMesh::SetValue(const zHash32& _hParamName, const float& _fValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxMesh* pMesh = static_cast<zcRes::zGfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _fValue);
}

void zGfxMesh::SetValue(const zHash32& _hParamName, const zVec2F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxMesh* pMesh = static_cast<zcRes::zGfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void zGfxMesh::SetValue(const zHash32& _hParamName, const zVec3F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxMesh* pMesh = static_cast<zcRes::zGfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void zGfxMesh::SetValue(const zHash32& _hParamName, const zVec4F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxMesh* pMesh = static_cast<zcRes::zGfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void zGfxMesh::SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxMesh* pMesh = static_cast<zcRes::zGfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _matValue);
}

void zGfxMesh::SetValue(const zHash32& _hTextureName, zGfxTexture2D _rTexture )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxMesh* pMesh = static_cast<zcRes::zGfxMesh*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture);
}

void zGfxMesh::SetValue(const zHash32& _hTextureName, zGfxSampler _rSampler )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxMesh* pMesh = static_cast<zcRes::zGfxMesh*>(mpResource);
	pMesh->SetValue(_hTextureName, _rSampler);
}

void zGfxMesh::SetValue(const zHash32& _hTextureName, zGfxTexture2D _rTexture, zGfxSampler _rSampler )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxMesh* pMesh = static_cast<zcRes::zGfxMesh*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture, _rSampler);
}

//=================================================================================================
// GFX MESH STRIP
//=================================================================================================
void zGfxMeshStrip::RenderMeshStrip()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxMeshStripRef& rMeshStrip = *static_cast<zcRes::GfxMeshStripRef*>(this);
	EMgr::GfxRender.Render( rMeshStrip );
}

void zGfxMeshStrip::SetValue(const zShaderParameter& _Value)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxMeshStrip* rMesh = static_cast<zcRes::zGfxMeshStrip*>(mpResource);
	rMesh->SetValue(_Value);
}
void zGfxMeshStrip::SetValue(const zArrayBase<const zShaderParameter*>& _aValues)	
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxMeshStrip* pMesh = static_cast<zcRes::zGfxMeshStrip*>(mpResource);
	pMesh->SetValue(_aValues);
}

void zGfxMeshStrip::SetValue(const zHash32& _hParamName, const float& _fValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxMeshStrip* pMesh = static_cast<zcRes::zGfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _fValue);
}

void zGfxMeshStrip::SetValue(const zHash32& _hParamName, const zVec2F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxMeshStrip* pMesh = static_cast<zcRes::zGfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void zGfxMeshStrip::SetValue(const zHash32& _hParamName, const zVec3F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxMeshStrip* pMesh = static_cast<zcRes::zGfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void zGfxMeshStrip::SetValue(const zHash32& _hParamName, const zVec4F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxMeshStrip* pMesh = static_cast<zcRes::zGfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void zGfxMeshStrip::SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxMeshStrip* pMesh = static_cast<zcRes::zGfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _matValue);
}

void zGfxMeshStrip::SetValue(const zHash32& _hTextureName, zGfxTexture2D _rTexture )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxMeshStrip* pMesh = static_cast<zcRes::zGfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture);
}

void zGfxMeshStrip::SetValue(const zHash32& _hTextureName, zGfxSampler _rSampler )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxMeshStrip* pMesh = static_cast<zcRes::zGfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hTextureName, _rSampler);
}

void zGfxMeshStrip::SetValue(const zHash32& _hTextureName, zGfxTexture2D _rTexture, zGfxSampler _rSampler )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::zGfxMeshStrip* pMesh = static_cast<zcRes::zGfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture, _rSampler);
}
}} // namespace zen, zenRes
