#include "libZenCore.h"

namespace zen { namespace zenRes {

//=================================================================================================
// CREATES
//=================================================================================================
GfxTexture2D GfxTexture2D::Create(awconst::eTextureFormat _eFormat, zenVec2U16 _vDim, awFlagResTexCreate _CreationFlags)
{
	static zenArrayStatic<zenU8> aEmpty(zenUInt(0));
	return EExp::CreateGfxTexture2D(_eFormat, _CreationFlags, _vDim, aEmpty);
}

GfxTexture2D GfxTexture2D::Create(awconst::eTextureFormat _eFormat, zenVec2U16 _vDim, const zenArrayBase<zenU8>& _aRawData, awFlagResTexCreate _CreationFlags)
{
	return EExp::CreateGfxTexture2D(_eFormat, _CreationFlags, _vDim, _aRawData);
}

GfxRenderTarget GfxRenderTarget::Create(awconst::eTextureFormat _eFormat, zenVec2U16 _vDim, bool _bSrgb)
{
	return EExp::CreateGfxRenderTarget(_eFormat, _vDim, _bSrgb);
}

GfxVertex GfxVertex::Create(const zenArrayBase<GfxVertex::Stream>& _aStreams, zenU32 _uCreationFlags)
{
	return EExp::CreateGfxVertex(_aStreams, _uCreationFlags);
}

GfxIndex GfxIndex::Create(const zenArrayBase<zenU16>& _Indices, awconst::ePrimitiveType _ePrimitiveType)
{
	return EExp::CreateGfxIndexBuffer(_Indices, _ePrimitiveType);
}

GfxIndex GfxIndex::Create(const zenArrayBase<zenU32>& _Indices, awconst::ePrimitiveType _ePrimitiveType)
{
	return EExp::CreateGfxIndexBuffer(_Indices, _ePrimitiveType);
}

//=================================================================================================
// GFX VERTEX
//=================================================================================================
zenU8* GfxVertex::Lock()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxVertex* pVertex = static_cast<ERes::GfxVertex*>(mpResource);
	return pVertex->Lock();
}
	
void GfxVertex::Unlock()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxVertex* pVertex = static_cast<ERes::GfxVertex*>(mpResource);
	return pVertex->Unlock();
}

//=================================================================================================
// GFX INDEX
//=================================================================================================
zenU8* GfxIndex::Lock()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxIndex* pIndex = static_cast<ERes::GfxIndex*>(mpResource);
	return pIndex->Lock();
}

void GfxIndex::Unlock()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxIndex* pIndex = static_cast<ERes::GfxIndex*>(mpResource);
	return pIndex->Unlock();
}

//=================================================================================================
// GFX TEXTURE2D
//=================================================================================================
const zenVec2U16& GfxTexture2D::GetDim()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxTexture2D* pTexture = static_cast<ERes::GfxTexture2D*>(mpResource);
	return pTexture->GetDim();
}

//=================================================================================================
// GFX RENDER TARGET
//=================================================================================================
bool GfxRenderTarget::IsDepth()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxRenderTarget* pRenderTarget = static_cast<ERes::GfxRenderTarget*>(mpResource);
	return pRenderTarget->IsDepth();
}

const zenVec2U16& GfxRenderTarget::GetDim()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxRenderTarget* pRenderTarget = static_cast<ERes::GfxRenderTarget*>(mpResource);
	return pRenderTarget->GetDim();
}

void GfxRenderTarget::Clear(const zenVec4F& _vRGBA)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxRenderTarget* pRenderTarget = static_cast<ERes::GfxRenderTarget*>(mpResource);
	pRenderTarget->Clear(_vRGBA);
}

void GfxRenderTarget::Clear(float _fDepth, zenU8 _uStencil, bool _bClearDepth, bool _bClearStencil)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxRenderTarget* pRenderTarget = static_cast<ERes::GfxRenderTarget*>(mpResource);
	pRenderTarget->Clear(_fDepth, _uStencil, _bClearDepth, _bClearStencil);
}

GfxTexture2D GfxRenderTarget::GetTexture2D()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxRenderTarget* pRenderTarget = static_cast<ERes::GfxRenderTarget*>(mpResource);
	return pRenderTarget->GetTexture2D();
}

} } // namespace zen, zenRes

