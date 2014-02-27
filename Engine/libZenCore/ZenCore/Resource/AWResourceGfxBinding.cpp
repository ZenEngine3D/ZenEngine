#include "libZenCore.h"

namespace zen { namespace zenRes {

//=================================================================================================
// CREATES
//=================================================================================================
GfxMesh GfxMesh::Create(const zenArrayBase<GfxMeshStrip>& _aMeshStrip)
{
	//! @todo Use a heap allocator instead, for faster creation
	zenArrayStatic<zenResID>	aMeshStripID;
	aMeshStripID.Copy<GfxMeshStrip>(_aMeshStrip);
	return EExp::CreateGfxMesh( aMeshStripID );
}

GfxMesh GfxMesh::Create(const GfxMeshStrip* _aMeshStrip, zenUInt _uMeshStripCount)
{
	//! @todo Use a heap allocator instead, for faster creation
	zenArrayStatic<zenResID>	aMeshStripID;
	aMeshStripID.Copy<GfxMeshStrip>(_aMeshStrip, _uMeshStripCount);
	return EExp::CreateGfxMesh( aMeshStripID );
}

GfxMeshStrip GfxMeshStrip::Create(GfxVertex _VertexBuffer, GfxIndex _IndexBuffer, GfxShaderBinding _rShaderBinding, const zenArrayBase<zenResID>& _aShaderParamID, const zenArrayBase<awShaderTexture>& _aTexture, zenU32 _uIndexFirst, zenU32 _uIndexCount)
{
	return EExp::CreateGfxMeshStrip(_VertexBuffer, _IndexBuffer, _rShaderBinding.GetResID(), _uIndexFirst, _uIndexCount, _aShaderParamID, _aTexture);
}

GfxMeshStrip GfxMeshStrip::Create(GfxVertex _VertexBuffer, GfxIndex _IndexBuffer, GfxShaderBinding _rShaderBinding, const zenArrayBase<zenResID>& _aShaderParamID, zenU32 _uIndexFirst, zenU32 _uIndexCount)
{	
	static const zenArrayStatic<awShaderTexture>	aTexture(zenUInt(0));
	return EExp::CreateGfxMeshStrip(_VertexBuffer, _IndexBuffer, _rShaderBinding.GetResID(), _uIndexFirst, _uIndexCount, _aShaderParamID, aTexture);
}

GfxMeshStrip GfxMeshStrip::Create(GfxVertex _VertexBuffer, GfxIndex _IndexBuffer, GfxShaderBinding _rShaderBinding, zenU32 _uIndexFirst, zenU32 _uIndexCount)
{
	static const zenArrayStatic<zenResID>	aShaderParamID(zenUInt(0));
	static const zenArrayStatic<awShaderTexture>	aTexture(zenUInt(0));
	return EExp::CreateGfxMeshStrip(_VertexBuffer, _IndexBuffer, _rShaderBinding.GetResID(), _uIndexFirst, _uIndexCount, aShaderParamID, aTexture);
}

GfxSampler GfxSampler::Create( awconst::eTextureFiltering _eFilterMin, awconst::eTextureFiltering _eFilterMag, awconst::eTextureWrap _eWrapU, awconst::eTextureWrap _eWrapV, float _fLodBias, const zenVec4F& _vBorderColor )
{
	return EExp::CreateGfxSampler(_eFilterMin, _eFilterMag, _eWrapU, _eWrapV, _fLodBias, _vBorderColor);
}

GfxBlend GfxBlend::Create( zenType::awBlendDesc::awRTBlendDesc* _pxBlendDesc, zenU8 _uRenderTargets, bool _bAlphaToCoverageEnable, bool _bIndependentBlendEnable )
{
	return EExp::CreateGfxBlend(_pxBlendDesc, _uRenderTargets, _bAlphaToCoverageEnable, _bIndependentBlendEnable);
}

GfxDepthStencil GfxDepthStencil::Create( bool _bDepthEnable, bool _bDepthWrite, bool _bStencilEnable, zenU8 _uStencilReadMask, zenU8 _uStencilWriteMask, awconst::eComparisonFunc _eDepthFunc, zenType::awDepthStencilDesc::DepthStencilOp _xFrontFace, zenType::awDepthStencilDesc::DepthStencilOp _xBackFace )
{
	return EExp::CreateGfxDepthStencil(_bDepthEnable, _bDepthWrite, _bStencilEnable, _uStencilReadMask, _uStencilWriteMask, _eDepthFunc, _xFrontFace, _xBackFace);
}

GfxRasterizer GfxRasterizer::Create( bool _bFrontCounterClockwise, bool _bDepthClipEnable, bool _bScissorEnable, bool _bMultisampleEnable, bool _bAntialiasedLineEnable, bool _bWireFrame, awconst::eCullMode _eCullMode, zenS32 _iDepthBias, zenF32 _fDepthBiasClamp, zenF32 _fSlopeScaledDepthBias )
{
	return EExp::CreateGfxRasterizer(_bFrontCounterClockwise, _bDepthClipEnable, _bScissorEnable, _bMultisampleEnable, _bAntialiasedLineEnable, _bWireFrame, _eCullMode, _iDepthBias, _fDepthBiasClamp, _fSlopeScaledDepthBias);
}

GfxView GfxView::Create( const GfxRenderTarget& _RTColor, const GfxRenderTarget& _RTDepth, const zenVec2U16& _vDim, const zenVec2U16& _vOrigin )
{
	return EExp::CreateGfxView(_RTColor.GetResID(), _RTDepth.GetResID(), _vDim, _vOrigin);
}

GfxView GfxView::Create( const zenArrayBase<GfxRenderTarget>& _aRTColor, const GfxRenderTarget& _RTDepth, const zenVec2U16& _vDim, const zenVec2U16& _vOrigin )
{
	zenArrayStatic<zenResID> aRTColorID;
	aRTColorID.Copy<GfxRenderTarget>(_aRTColor);
	return EExp::CreateGfxView(aRTColorID, _RTDepth.GetResID(), _vDim, _vOrigin);
}

GfxWindow GfxWindow::Create( HWND _WindowHandle )
{
	return EExp::CreateGfxWindow(_WindowHandle);
}

GfxShaderBinding GfxShaderBinding::Create(const zenArrayBase<zenRes::GfxShader>& _aShaderID)
{
	zenArrayStatic<zenResID> aShaderID;
	aShaderID.Copy<zenRes::GfxShader>(_aShaderID);	
	return EExp::CreateGfxShaderBinding(aShaderID);
}

GfxShaderBinding GfxShaderBinding::Create(const zenRes::GfxShader* _pShaders, zenUInt _uCount)
{
	zenArrayStatic<zenResID> aShaderID;
	aShaderID.Copy<zenRes::GfxShader>(_pShaders, _uCount);	
	return EExp::CreateGfxShaderBinding(aShaderID);
}

//=================================================================================================
// GFX WINDOW
//=================================================================================================
//! @todo Convert all mpResouce cast to using Ref Object
GfxView GfxWindow::GetBackbuffer()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxWindow* pWindow = static_cast<ERes::GfxWindow*>(mpResource);
	return pWindow->GetBackbuffer();
}

void GfxWindow::Resize(const zenVec2U16& _vSize)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxWindow* pWindow = static_cast<ERes::GfxWindow*>(mpResource);
	return pWindow->Resize(_vSize);
}

void GfxWindow::FrameBegin()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxWindowRef& rWindow = *static_cast<ERes::GfxWindowRef*>(this);
	EMgr::GfxRender.FrameBegin(rWindow);
	EMgr::GfxState.PipelineReset();
}

void GfxWindow::FrameEnd()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxWindowRef& rWindow = *static_cast<ERes::GfxWindowRef*>(this);
	EMgr::GfxRender.FrameEnd();
}

//=================================================================================================
// GFX VIEW
//=================================================================================================
void GfxView::Clear( bool _bClearColor, const zenVec4F& _vRGBA, bool _bClearDepth, float _fDepth, bool _bClearStencil, zenU8 _uStencil )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxView* pView = static_cast<ERes::GfxView*>(mpResource);
	pView->Clear(_bClearColor, _vRGBA, _bClearDepth, _fDepth, _bClearStencil, _uStencil);
}

void GfxView::ActivateView()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxViewRef& rView = *static_cast<ERes::GfxViewRef*>(this);
	EMgr::GfxState.SetView( rView );
}

zenVec2U16 GfxView::GetDim()const
{
	const ERes::GfxViewRef& rView = *static_cast<const ERes::GfxViewRef*>(this);
	ZENAssertMsg(rView.IsValid(), "No valid resource assigned");
	return rView->Get().mvDim;
}

//=================================================================================================
// GFX MESH
//=================================================================================================
void GfxMesh::RenderMesh()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshRef& rMesh = *static_cast<ERes::GfxMeshRef*>(this);
	EMgr::GfxRender.Render( rMesh );
}

void GfxMesh::SetValue(const awShaderParameter& _Value)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* rMesh = static_cast<ERes::GfxMesh*>(mpResource);
	rMesh->SetValue(_Value);
}
void GfxMesh::SetValue(const zenArrayBase<const awShaderParameter*>& _aValues)	
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_aValues);
}

void GfxMesh::SetValue(const zenHash32& _hParamName, const float& _fValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _fValue);
}

void GfxMesh::SetValue(const zenHash32& _hParamName, const zenVec2F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void GfxMesh::SetValue(const zenHash32& _hParamName, const zenVec3F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void GfxMesh::SetValue(const zenHash32& _hParamName, const zenVec4F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void GfxMesh::SetValue(const zenHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hParamName, _matValue);
}

void GfxMesh::SetValue(const zenHash32& _hTextureName, GfxTexture2D _rTexture )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture);
}

void GfxMesh::SetValue(const zenHash32& _hTextureName, GfxSampler _rSampler )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hTextureName, _rSampler);
}

void GfxMesh::SetValue(const zenHash32& _hTextureName, GfxTexture2D _rTexture, GfxSampler _rSampler )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMesh* pMesh = static_cast<ERes::GfxMesh*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture, _rSampler);
}

//=================================================================================================
// GFX MESH STRIP
//=================================================================================================
void GfxMeshStrip::RenderMeshStrip()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStripRef& rMeshStrip = *static_cast<ERes::GfxMeshStripRef*>(this);
	EMgr::GfxRender.Render( rMeshStrip );
}

void GfxMeshStrip::SetValue(const awShaderParameter& _Value)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* rMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	rMesh->SetValue(_Value);
}
void GfxMeshStrip::SetValue(const zenArrayBase<const awShaderParameter*>& _aValues)	
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_aValues);
}

void GfxMeshStrip::SetValue(const zenHash32& _hParamName, const float& _fValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _fValue);
}

void GfxMeshStrip::SetValue(const zenHash32& _hParamName, const zenVec2F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void GfxMeshStrip::SetValue(const zenHash32& _hParamName, const zenVec3F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void GfxMeshStrip::SetValue(const zenHash32& _hParamName, const zenVec4F& _vValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _vValue);
}

void GfxMeshStrip::SetValue(const zenHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hParamName, _matValue);
}

void GfxMeshStrip::SetValue(const zenHash32& _hTextureName, GfxTexture2D _rTexture )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture);
}

void GfxMeshStrip::SetValue(const zenHash32& _hTextureName, GfxSampler _rSampler )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hTextureName, _rSampler);
}

void GfxMeshStrip::SetValue(const zenHash32& _hTextureName, GfxTexture2D _rTexture, GfxSampler _rSampler )
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxMeshStrip* pMesh = static_cast<ERes::GfxMeshStrip*>(mpResource);
	pMesh->SetValue(_hTextureName, _rTexture, _rSampler);
}
}} // namespace zen, zenRes
