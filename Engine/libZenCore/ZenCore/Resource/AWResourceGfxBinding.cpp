#include "libZenCore.h"

namespace zen { namespace awres {

//=================================================================================================
// CREATES
//=================================================================================================
awGfxMesh awGfxMesh::Create(const awArrayBase<awGfxMeshStrip>& _aMeshStrip)
{
	//! @todo Use a heap allocator instead, for faster creation
	awArrayStatic<awResourceID>	aMeshStripID;
	aMeshStripID.Copy<awGfxMeshStrip>(_aMeshStrip);
	return EExp::CreateGfxMesh( aMeshStripID );
}

awGfxMesh awGfxMesh::Create(const awGfxMeshStrip* _aMeshStrip, awUInt _uMeshStripCount)
{
	//! @todo Use a heap allocator instead, for faster creation
	awArrayStatic<awResourceID>	aMeshStripID;
	aMeshStripID.Copy<awGfxMeshStrip>(_aMeshStrip, _uMeshStripCount);
	return EExp::CreateGfxMesh( aMeshStripID );
}

awGfxMeshStrip awGfxMeshStrip::Create(awGfxVertex _VertexBuffer, awGfxIndex _IndexBuffer, awGfxShaderBinding _rShaderBinding, const awArrayBase<awResourceID>& _aShaderParamID, const awArrayBase<awShaderTexture>& _aTexture, awU32 _uIndexFirst, awU32 _uIndexCount)
{
	return EExp::CreateGfxMeshStrip(_VertexBuffer, _IndexBuffer, _rShaderBinding.GetResID(), _uIndexFirst, _uIndexCount, _aShaderParamID, _aTexture);
}

awGfxMeshStrip awGfxMeshStrip::Create(awGfxVertex _VertexBuffer, awGfxIndex _IndexBuffer, awGfxShaderBinding _rShaderBinding, const awArrayBase<awResourceID>& _aShaderParamID, awU32 _uIndexFirst, awU32 _uIndexCount)
{	
	static const awArrayStatic<awShaderTexture>	aTexture(awUInt(0));
	return EExp::CreateGfxMeshStrip(_VertexBuffer, _IndexBuffer, _rShaderBinding.GetResID(), _uIndexFirst, _uIndexCount, _aShaderParamID, aTexture);
}

awGfxMeshStrip awGfxMeshStrip::Create(awGfxVertex _VertexBuffer, awGfxIndex _IndexBuffer, awGfxShaderBinding _rShaderBinding, awU32 _uIndexFirst, awU32 _uIndexCount)
{
	static const awArrayStatic<awResourceID>	aShaderParamID(awUInt(0));
	static const awArrayStatic<awShaderTexture>	aTexture(awUInt(0));
	return EExp::CreateGfxMeshStrip(_VertexBuffer, _IndexBuffer, _rShaderBinding.GetResID(), _uIndexFirst, _uIndexCount, aShaderParamID, aTexture);
}

awGfxSampler awGfxSampler::Create( awconst::eTextureFiltering _eFilterMin, awconst::eTextureFiltering _eFilterMag, awconst::eTextureWrap _eWrapU, awconst::eTextureWrap _eWrapV, float _fLodBias, const awVec4F& _vBorderColor )
{
	return EExp::CreateGfxSampler(_eFilterMin, _eFilterMag, _eWrapU, _eWrapV, _fLodBias, _vBorderColor);
}

awGfxBlend awGfxBlend::Create( awtype::awBlendDesc::awRTBlendDesc* _pxBlendDesc, awU8 _uRenderTargets, bool _bAlphaToCoverageEnable, bool _bIndependentBlendEnable )
{
	return EExp::CreateGfxBlend(_pxBlendDesc, _uRenderTargets, _bAlphaToCoverageEnable, _bIndependentBlendEnable);
}

awGfxDepthStencil awGfxDepthStencil::Create( bool _bDepthEnable, bool _bDepthWrite, bool _bStencilEnable, awU8 _uStencilReadMask, awU8 _uStencilWriteMask, awconst::eComparisonFunc _eDepthFunc, awtype::awDepthStencilDesc::DepthStencilOp _xFrontFace, awtype::awDepthStencilDesc::DepthStencilOp _xBackFace )
{
	return EExp::CreateGfxDepthStencil(_bDepthEnable, _bDepthWrite, _bStencilEnable, _uStencilReadMask, _uStencilWriteMask, _eDepthFunc, _xFrontFace, _xBackFace);
}

awGfxRasterizer awGfxRasterizer::Create( bool _bFrontCounterClockwise, bool _bDepthClipEnable, bool _bScissorEnable, bool _bMultisampleEnable, bool _bAntialiasedLineEnable, bool _bWireFrame, awconst::eCullMode _eCullMode, awS32 _iDepthBias, awF32 _fDepthBiasClamp, awF32 _fSlopeScaledDepthBias )
{
	return EExp::CreateGfxRasterizer(_bFrontCounterClockwise, _bDepthClipEnable, _bScissorEnable, _bMultisampleEnable, _bAntialiasedLineEnable, _bWireFrame, _eCullMode, _iDepthBias, _fDepthBiasClamp, _fSlopeScaledDepthBias);
}

awGfxView awGfxView::Create( const awGfxRenderTarget& _RTColor, const awGfxRenderTarget& _RTDepth, const awVec2U16& _vDim, const awVec2U16& _vOrigin )
{
	return EExp::CreateGfxView(_RTColor.GetResID(), _RTDepth.GetResID(), _vDim, _vOrigin);
}

awGfxView awGfxView::Create( const awArrayBase<awGfxRenderTarget>& _aRTColor, const awGfxRenderTarget& _RTDepth, const awVec2U16& _vDim, const awVec2U16& _vOrigin )
{
	awArrayStatic<awResourceID> aRTColorID;
	aRTColorID.Copy<awGfxRenderTarget>(_aRTColor);
	return EExp::CreateGfxView(aRTColorID, _RTDepth.GetResID(), _vDim, _vOrigin);
}

awGfxWindow awGfxWindow::Create( HWND _WindowHandle )
{
	return EExp::CreateGfxWindow(_WindowHandle);
}

awGfxShaderBinding awGfxShaderBinding::Create(const awArrayBase<awres::awGfxShader>& _aShaderID)
{
	awArrayStatic<awResourceID> aShaderID;
	aShaderID.Copy<awres::awGfxShader>(_aShaderID);	
	return EExp::CreateGfxShaderBinding(aShaderID);
}

awGfxShaderBinding awGfxShaderBinding::Create(const awres::awGfxShader* _pShaders, awUInt _uCount)
{
	awArrayStatic<awResourceID> aShaderID;
	aShaderID.Copy<awres::awGfxShader>(_pShaders, _uCount);	
	return EExp::CreateGfxShaderBinding(aShaderID);
}

//=================================================================================================
// GFX WINDOW
//=================================================================================================
//! @todo Convert all mpResouce cast to using Ref Object
awGfxView awGfxWindow::GetBackbuffer()
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxWindow* pWindow = static_cast<ERes::GfxWindow*>(mpResource);
	return pWindow->GetBackbuffer();
}

void awGfxWindow::Resize(const awVec2U16& _vSize)
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxWindow* pWindow = static_cast<ERes::GfxWindow*>(mpResource);
	return pWindow->Resize(_vSize);
}

void awGfxWindow::FrameBegin()
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxWindowRef& rWindow = *static_cast<ERes::GfxWindowRef*>(this);
	EMgr::GfxRender.FrameBegin(rWindow);
	EMgr::GfxState.PipelineReset();
}

void awGfxWindow::FrameEnd()
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxWindowRef& rWindow = *static_cast<ERes::GfxWindowRef*>(this);
	EMgr::GfxRender.FrameEnd();
}

//=================================================================================================
// GFX VIEW
//=================================================================================================
void awGfxView::Clear( bool _bClearColor, const awVec4F& _vRGBA, bool _bClearDepth, float _fDepth, bool _bClearStencil, awU8 _uStencil )
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxView* pView = static_cast<ERes::GfxView*>(mpResource);
	pView->Clear(_bClearColor, _vRGBA, _bClearDepth, _fDepth, _bClearStencil, _uStencil);
}

void awGfxView::ActivateView()
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxViewRef& rView = *static_cast<ERes::GfxViewRef*>(this);
	EMgr::GfxState.SetView( rView );
}

awVec2U16 awGfxView::GetDim()const
{
	const ERes::GfxViewRef& rView = *static_cast<const ERes::GfxViewRef*>(this);
	AWAssertMsg(rView.IsValid(), "No valid resource assigned");
	return rView->Get().mvDim;
}

//=================================================================================================
// GFX MESH
//=================================================================================================
void awGfxMesh::RenderMesh()
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshRef& rMesh = *static_cast<ERes::GfxMeshRef*>(this);
	EMgr::GfxRender.Render( rMesh );
}

void awGfxMesh::SetValue(const awShaderParameter& _Value)
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* rMesh = static_cast<ERes::GfxMesh*>(mpResource);
	rMesh->SetValue(_Value);
}
void awGfxMesh::SetValue(const awArrayBase<const awShaderParameter*>& _aValues)	
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_aValues);
}

void awGfxMesh::SetValue(const awHash32& _hParamName, const float& _fValue)
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _fValue);
}

void awGfxMesh::SetValue(const awHash32& _hParamName, const awVec2F& _vValue)
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void awGfxMesh::SetValue(const awHash32& _hParamName, const awVec3F& _vValue)
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void awGfxMesh::SetValue(const awHash32& _hParamName, const awVec4F& _vValue)
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void awGfxMesh::SetValue(const awHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _matValue);
}

void awGfxMesh::SetValue(const awHash32& _hTextureName, awGfxTexture2D _rTexture )
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture);
}

void awGfxMesh::SetValue(const awHash32& _hTextureName, awGfxSampler _rSampler )
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hTextureName, _rSampler);
}

void awGfxMesh::SetValue(const awHash32& _hTextureName, awGfxTexture2D _rTexture, awGfxSampler _rSampler )
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture, _rSampler);
}

//=================================================================================================
// GFX MESH STRIP
//=================================================================================================
void awGfxMeshStrip::RenderMeshStrip()
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStripRef& rMeshStrip = *static_cast<ERes::GfxMeshStripRef*>(this);
	EMgr::GfxRender.Render( rMeshStrip );
}

void awGfxMeshStrip::SetValue(const awShaderParameter& _Value)
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* rMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	rMesh->SetValue(_Value);
}
void awGfxMeshStrip::SetValue(const awArrayBase<const awShaderParameter*>& _aValues)	
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_aValues);
}

void awGfxMeshStrip::SetValue(const awHash32& _hParamName, const float& _fValue)
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _fValue);
}

void awGfxMeshStrip::SetValue(const awHash32& _hParamName, const awVec2F& _vValue)
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void awGfxMeshStrip::SetValue(const awHash32& _hParamName, const awVec3F& _vValue)
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void awGfxMeshStrip::SetValue(const awHash32& _hParamName, const awVec4F& _vValue)
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void awGfxMeshStrip::SetValue(const awHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _matValue);
}

void awGfxMeshStrip::SetValue(const awHash32& _hTextureName, awGfxTexture2D _rTexture )
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture);
}

void awGfxMeshStrip::SetValue(const awHash32& _hTextureName, awGfxSampler _rSampler )
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hTextureName, _rSampler);
}

void awGfxMeshStrip::SetValue(const awHash32& _hTextureName, awGfxTexture2D _rTexture, awGfxSampler _rSampler )
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture, _rSampler);
}
}} // namespace zen, awres
