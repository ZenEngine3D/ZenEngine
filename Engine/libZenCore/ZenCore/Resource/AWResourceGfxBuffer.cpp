#include "libZenCore.h"

namespace zen { namespace awres {

//=================================================================================================
// CREATES
//=================================================================================================
awGfxTexture2D awGfxTexture2D::Create(awconst::eTextureFormat _eFormat, awVec2U16 _vDim, awFlagResTexCreate _CreationFlags)
{
	static awArrayStatic<awU8> aEmpty(awUInt(0));
	return EExp::CreateGfxTexture2D(_eFormat, _CreationFlags, _vDim, aEmpty);
}

awGfxTexture2D awGfxTexture2D::Create(awconst::eTextureFormat _eFormat, awVec2U16 _vDim, const awArrayBase<awU8>& _aRawData, awFlagResTexCreate _CreationFlags)
{
	return EExp::CreateGfxTexture2D(_eFormat, _CreationFlags, _vDim, _aRawData);
}

awGfxRenderTarget awGfxRenderTarget::Create(awconst::eTextureFormat _eFormat, awVec2U16 _vDim, bool _bSrgb)
{
	return EExp::CreateGfxRenderTarget(_eFormat, _vDim, _bSrgb);
}

awGfxVertex awGfxVertex::Create(const awArrayBase<awGfxVertex::Stream>& _aStreams, awU32 _uCreationFlags)
{
	return EExp::CreateGfxVertex(_aStreams, _uCreationFlags);
}

awGfxIndex awGfxIndex::Create(const awArrayBase<awU16>& _Indices, awconst::ePrimitiveType _ePrimitiveType)
{
	return EExp::CreateGfxIndexBuffer(_Indices, _ePrimitiveType);
}

awGfxIndex awGfxIndex::Create(const awArrayBase<awU32>& _Indices, awconst::ePrimitiveType _ePrimitiveType)
{
	return EExp::CreateGfxIndexBuffer(_Indices, _ePrimitiveType);
}

//=================================================================================================
// GFX VERTEX
//=================================================================================================
awU8* awGfxVertex::Lock()
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxVertex* pVertex = static_cast<ERes::GfxVertex*>(mpResource);
	return pVertex->Lock();
}
	
void awGfxVertex::Unlock()
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxVertex* pVertex = static_cast<ERes::GfxVertex*>(mpResource);
	return pVertex->Unlock();
}

//=================================================================================================
// GFX INDEX
//=================================================================================================
awU8* awGfxIndex::Lock()
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxIndex* pIndex = static_cast<ERes::GfxIndex*>(mpResource);
	return pIndex->Lock();
}

void awGfxIndex::Unlock()
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxIndex* pIndex = static_cast<ERes::GfxIndex*>(mpResource);
	return pIndex->Unlock();
}

//=================================================================================================
// GFX TEXTURE2D
//=================================================================================================
const awVec2U16& awGfxTexture2D::GetDim()
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxTexture2D* pTexture = static_cast<ERes::GfxTexture2D*>(mpResource);
	return pTexture->GetDim();
}

//=================================================================================================
// GFX RENDER TARGET
//=================================================================================================
bool awGfxRenderTarget::IsDepth()
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxRenderTarget* pRenderTarget = static_cast<ERes::GfxRenderTarget*>(mpResource);
	return pRenderTarget->IsDepth();
}

const awVec2U16& awGfxRenderTarget::GetDim()
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxRenderTarget* pRenderTarget = static_cast<ERes::GfxRenderTarget*>(mpResource);
	return pRenderTarget->GetDim();
}

void awGfxRenderTarget::Clear(const awVec4F& _vRGBA)
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxRenderTarget* pRenderTarget = static_cast<ERes::GfxRenderTarget*>(mpResource);
	pRenderTarget->Clear(_vRGBA);
}

void awGfxRenderTarget::Clear(float _fDepth, awU8 _uStencil, bool _bClearDepth, bool _bClearStencil)
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxRenderTarget* pRenderTarget = static_cast<ERes::GfxRenderTarget*>(mpResource);
	pRenderTarget->Clear(_fDepth, _uStencil, _bClearDepth, _bClearStencil);
}

awGfxTexture2D awGfxRenderTarget::GetTexture2D()
{
	AWAssertMsg(mpResource, "No valid resource assigned");
	ERes::GfxRenderTarget* pRenderTarget = static_cast<ERes::GfxRenderTarget*>(mpResource);
	return pRenderTarget->GetTexture2D();
}

} } // namespace zen, awres

