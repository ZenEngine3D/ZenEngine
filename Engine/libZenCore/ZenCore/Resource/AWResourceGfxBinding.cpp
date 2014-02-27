#include "libZenCore.h"

namespace zen { namespace awres {

//=================================================================================================
// CREATES
//=================================================================================================
awGfxMesh awGfxMesh::Create(const zenArrayBase<awGfxMeshStrip>& _aMeshStrip)
{
	//! @todo Use a heap allocator instead, for faster creation
	zenArrayStatic<zenResID>	aMeshStripID;
	aMeshStripID.Copy<awGfxMeshStrip>(_aMeshStrip);
	return EExp::CreateGfxMesh( aMeshStripID );
}

awGfxMesh awGfxMesh::Create(const awGfxMeshStrip* _aMeshStrip, zenUInt _uMeshStripCount)
{
	//! @todo Use a heap allocator instead, for faster creation
	zenArrayStatic<zenResID>	aMeshStripID;
	aMeshStripID.Copy<awGfxMeshStrip>(_aMeshStrip, _uMeshStripCount);
	return EExp::CreateGfxMesh( aMeshStripID );
}

awGfxMeshStrip awGfxMeshStrip::Create(awGfxVertex _VertexBuffer, awGfxIndex _IndexBuffer, awGfxShaderBinding _rShaderBinding, const zenArrayBase<zenResID>& _aShaderParamID, const zenArrayBase<awShaderTexture>& _aTexture, zenU32 _uIndexFirst, zenU32 _uIndexCount)
{
	return EExp::CreateGfxMeshStrip(_VertexBuffer, _IndexBuffer, _rShaderBinding.GetResID(), _uIndexFirst, _uIndexCount, _aShaderParamID, _aTexture);
}

awGfxMeshStrip awGfxMeshStrip::Create(awGfxVertex _VertexBuffer, awGfxIndex _IndexBuffer, awGfxShaderBinding _rShaderBinding, const zenArrayBase<zenResID>& _aShaderParamID, zenU32 _uIndexFirst, zenU32 _uIndexCount)
{	
	static const zenArrayStatic<awShaderTexture>	aTexture(zenUInt(0));
	return EExp::CreateGfxMeshStrip(_VertexBuffer, _IndexBuffer, _rShaderBinding.GetResID(), _uIndexFirst, _uIndexCount, _aShaderParamID, aTexture);
}

awGfxMeshStrip awGfxMeshStrip::Create(awGfxVertex _VertexBuffer, awGfxIndex _IndexBuffer, awGfxShaderBinding _rShaderBinding, zenU32 _uIndexFirst, zenU32 _uIndexCount)
{
	static const zenArrayStatic<zenResID>	aShaderParamID(zenUInt(0));
	static const zenArrayStatic<awShaderTexture>	aTexture(zenUInt(0));
	return EExp::CreateGfxMeshStrip(_VertexBuffer, _IndexBuffer, _rShaderBinding.GetResID(), _uIndexFirst, _uIndexCount, aShaderParamID, aTexture);
}

awGfxSampler awGfxSampler::Create( awconst::eTextureFiltering _eFilterMin, awconst::eTextureFiltering _eFilterMag, awconst::eTextureWrap _eWrapU, awconst::eTextureWrap _eWrapV, float _fLodBias, const zenVec4F& _vBorderColor )
{
	return EExp::CreateGfxSampler(_eFilterMin, _eFilterMag, _eWrapU, _eWrapV, _fLodBias, _vBorderColor);
}

awGfxBlend awGfxBlend::Create( zenType::awBlendDesc::awRTBlendDesc* _pxBlendDesc, zenU8 _uRenderTargets, bool _bAlphaToCoverageEnable, bool _bIndependentBlendEnable )
{
	return EExp::CreateGfxBlend(_pxBlendDesc, _uRenderTargets, _bAlphaToCoverageEnable, _bIndependentBlendEnable);
}

awGfxDepthStencil awGfxDepthStencil::Create( bool _bDepthEnable, bool _bDepthWrite, bool _bStencilEnable, zenU8 _uStencilReadMask, zenU8 _uStencilWriteMask, awconst::eComparisonFunc _eDepthFunc, zenType::awDepthStencilDesc::DepthStencilOp _xFrontFace, zenType::awDepthStencilDesc::DepthStencilOp _xBackFace )
{
	return EExp::CreateGfxDepthStencil(_bDepthEnable, _bDepthWrite, _bStencilEnable, _uStencilReadMask, _uStencilWriteMask, _eDepthFunc, _xFrontFace, _xBackFace);
}

awGfxRasterizer awGfxRasterizer::Create( bool _bFrontCounterClockwise, bool _bDepthClipEnable, bool _bScissorEnable, bool _bMultisampleEnable, bool _bAntialiasedLineEnable, bool _bWireFrame, awconst::eCullMode _eCullMode, zenS32 _iDepthBias, zenF32 _fDepthBiasClamp, zenF32 _fSlopeScaledDepthBias )
{
	return EExp::CreateGfxRasterizer(_bFrontCounterClockwise, _bDepthClipEnable, _bScissorEnable, _bMultisampleEnable, _bAntialiasedLineEnable, _bWireFrame, _eCullMode, _iDepthBias, _fDepthBiasClamp, _fSlopeScaledDepthBias);
}

awGfxView awGfxView::Create( const awGfxRenderTarget& _RTColor, const awGfxRenderTarget& _RTDepth, const zenVec2U16& _vDim, const zenVec2U16& _vOrigin )
{
	return EExp::CreateGfxView(_RTColor.GetResID(), _RTDepth.GetResID(), _vDim, _vOrigin);
}

awGfxView awGfxView::Create( const zenArrayBase<awGfxRenderTarget>& _aRTColor, const awGfxRenderTarget& _RTDepth, const zenVec2U16& _vDim, const zenVec2U16& _vOrigin )
{
	zenArrayStatic<zenResID> aRTColorID;
	aRTColorID.Copy<awGfxRenderTarget>(_aRTColor);
	return EExp::CreateGfxView(aRTColorID, _RTDepth.GetResID(), _vDim, _vOrigin);
}

awGfxWindow awGfxWindow::Create( HWND _WindowHandle )
{
	return EExp::CreateGfxWindow(_WindowHandle);
}

awGfxShaderBinding awGfxShaderBinding::Create(const zenArrayBase<awres::awGfxShader>& _aShaderID)
{
	zenArrayStatic<zenResID> aShaderID;
	aShaderID.Copy<awres::awGfxShader>(_aShaderID);	
	return EExp::CreateGfxShaderBinding(aShaderID);
}

awGfxShaderBinding awGfxShaderBinding::Create(const awres::awGfxShader* _pShaders, zenUInt _uCount)
{
	zenArrayStatic<zenResID> aShaderID;
	aShaderID.Copy<awres::awGfxShader>(_pShaders, _uCount);	
	return EExp::CreateGfxShaderBinding(aShaderID);
}

//=================================================================================================
// GFX WINDOW
//=================================================================================================
//! @todo Convert all mpResouce cast to using Ref Object
awGfxView awGfxWindow::GetBackbuffer()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxWindow* pWindow = static_cast<ERes::GfxWindow*>(mpResource);
	return pWindow->GetBackbuffer();
}

void awGfxWindow::Resize(const zenVec2U16& _vSize)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxWindow* pWindow = static_cast<ERes::GfxWindow*>(mpResource);
	return pWindow->Resize(_vSize);
}

void awGfxWindow::FrameBegin()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxWindowRef& rWindow = *static_cast<ERes::GfxWindowRef*>(this);
	EMgr::GfxRender.FrameBegin(rWindow);
	EMgr::GfxState.PipelineReset();
}

void awGfxWindow::FrameEnd()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxWindowRef& rWindow = *static_cast<ERes::GfxWindowRef*>(this);
	EMgr::GfxRender.FrameEnd();
}

//=================================================================================================
// GFX VIEW
//=================================================================================================
void awGfxView::Clear( bool _bClearColor, const zenVec4F& _vRGBA, bool _bClearDepth, float _fDepth, bool _bClearStencil, zenU8 _uStencil )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxView* pView = static_cast<ERes::GfxView*>(mpResource);
	pView->Clear(_bClearColor, _vRGBA, _bClearDepth, _fDepth, _bClearStencil, _uStencil);
}

void awGfxView::ActivateView()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxViewRef& rView = *static_cast<ERes::GfxViewRef*>(this);
	EMgr::GfxState.SetView( rView );
}

zenVec2U16 awGfxView::GetDim()const
{
	const ERes::GfxViewRef& rView = *static_cast<const ERes::GfxViewRef*>(this);
	ZENAssertMsg(rView.IsValid(), "No valid resource assigned");
	return rView->Get().mvDim;
}

//=================================================================================================
// GFX MESH
//=================================================================================================
void awGfxMesh::RenderMesh()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshRef& rMesh = *static_cast<ERes::GfxMeshRef*>(this);
	EMgr::GfxRender.Render( rMesh );
}

void awGfxMesh::SetValue(const awShaderParameter& _Value)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* rMesh = static_cast<ERes::GfxMesh*>(mpResource);
	rMesh->SetValue(_Value);
}
void awGfxMesh::SetValue(const zenArrayBase<const awShaderParameter*>& _aValues)	
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_aValues);
}

void awGfxMesh::SetValue(const zenHash32& _hParamName, const float& _fValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _fValue);
}

void awGfxMesh::SetValue(const zenHash32& _hParamName, const zenVec2F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void awGfxMesh::SetValue(const zenHash32& _hParamName, const zenVec3F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void awGfxMesh::SetValue(const zenHash32& _hParamName, const zenVec4F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void awGfxMesh::SetValue(const zenHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _matValue);
}

void awGfxMesh::SetValue(const zenHash32& _hTextureName, awGfxTexture2D _rTexture )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture);
}

void awGfxMesh::SetValue(const zenHash32& _hTextureName, awGfxSampler _rSampler )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hTextureName, _rSampler);
}

void awGfxMesh::SetValue(const zenHash32& _hTextureName, awGfxTexture2D _rTexture, awGfxSampler _rSampler )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture, _rSampler);
}

//=================================================================================================
// GFX MESH STRIP
//=================================================================================================
void awGfxMeshStrip::RenderMeshStrip()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStripRef& rMeshStrip = *static_cast<ERes::GfxMeshStripRef*>(this);
	EMgr::GfxRender.Render( rMeshStrip );
}

void awGfxMeshStrip::SetValue(const awShaderParameter& _Value)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* rMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	rMesh->SetValue(_Value);
}
void awGfxMeshStrip::SetValue(const zenArrayBase<const awShaderParameter*>& _aValues)	
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_aValues);
}

void awGfxMeshStrip::SetValue(const zenHash32& _hParamName, const float& _fValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _fValue);
}

void awGfxMeshStrip::SetValue(const zenHash32& _hParamName, const zenVec2F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void awGfxMeshStrip::SetValue(const zenHash32& _hParamName, const zenVec3F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void awGfxMeshStrip::SetValue(const zenHash32& _hParamName, const zenVec4F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void awGfxMeshStrip::SetValue(const zenHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _matValue);
}

void awGfxMeshStrip::SetValue(const zenHash32& _hTextureName, awGfxTexture2D _rTexture )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture);
}

void awGfxMeshStrip::SetValue(const zenHash32& _hTextureName, awGfxSampler _rSampler )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hTextureName, _rSampler);
}

void awGfxMeshStrip::SetValue(const zenHash32& _hTextureName, awGfxTexture2D _rTexture, awGfxSampler _rSampler )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture, _rSampler);
}
}} // namespace zen, awres
