#include "libZenCore.h"

namespace zen { namespace zenRes {

//=================================================================================================
// CREATES
//=================================================================================================
zGfxTexture2D zGfxTexture2D::Create(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, zenFlagResTexCreate _CreationFlags)
{
	static zArrayStatic<zU8> aEmpty(zUInt(0));
	return EExp::CreateGfxTexture2D(_eFormat, _CreationFlags, _vDim, aEmpty);
}

zGfxTexture2D zGfxTexture2D::Create(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, const zArrayBase<zU8>& _aRawData, zenFlagResTexCreate _CreationFlags)
{
	return EExp::CreateGfxTexture2D(_eFormat, _CreationFlags, _vDim, _aRawData);
}

zGfxRenderTarget zGfxRenderTarget::Create(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, bool _bSrgb)
{
	return EExp::CreateGfxRenderTarget(_eFormat, _vDim, _bSrgb);
}

zGfxVertex zGfxVertex::Create(const zArrayBase<zGfxVertex::Stream>& _aStreams, zU32 _uCreationFlags)
{
	return EExp::CreateGfxVertex(_aStreams, _uCreationFlags);
}

zGfxIndex zGfxIndex::Create(const zArrayBase<zU16>& _Indices, zenConst::ePrimitiveType _ePrimitiveType)
{
	return EExp::CreateGfxIndexBuffer(_Indices, _ePrimitiveType);
}

zGfxIndex zGfxIndex::Create(const zArrayBase<zU32>& _Indices, zenConst::ePrimitiveType _ePrimitiveType)
{
	return EExp::CreateGfxIndexBuffer(_Indices, _ePrimitiveType);
}

//=================================================================================================
// GFX VERTEX
//=================================================================================================
zU8* zGfxVertex::Lock()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::zGfxVertex* pVertex = static_cast<ERes::zGfxVertex*>(mpResource);
	return pVertex->Lock();
}
	
void zGfxVertex::Unlock()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::zGfxVertex* pVertex = static_cast<ERes::zGfxVertex*>(mpResource);
	return pVertex->Unlock();
}

//=================================================================================================
// GFX INDEX
//=================================================================================================
zU8* zGfxIndex::Lock()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::zGfxIndex* pIndex = static_cast<ERes::zGfxIndex*>(mpResource);
	return pIndex->Lock();
}

void zGfxIndex::Unlock()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::zGfxIndex* pIndex = static_cast<ERes::zGfxIndex*>(mpResource);
	return pIndex->Unlock();
}

//=================================================================================================
// GFX TEXTURE2D
//=================================================================================================
const zVec2U16& zGfxTexture2D::GetDim()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::zGfxTexture2D* pTexture = static_cast<ERes::zGfxTexture2D*>(mpResource);
	return pTexture->GetDim();
}

//=================================================================================================
// GFX RENDER TARGET
//=================================================================================================
bool zGfxRenderTarget::IsDepth()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::zGfxRenderTarget* pRenderTarget = static_cast<ERes::zGfxRenderTarget*>(mpResource);
	return pRenderTarget->IsDepth();
}

const zVec2U16& zGfxRenderTarget::GetDim()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::zGfxRenderTarget* pRenderTarget = static_cast<ERes::zGfxRenderTarget*>(mpResource);
	return pRenderTarget->GetDim();
}

void zGfxRenderTarget::Clear(const zVec4F& _vRGBA)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::zGfxRenderTarget* pRenderTarget = static_cast<ERes::zGfxRenderTarget*>(mpResource);
	pRenderTarget->Clear(_vRGBA);
}

void zGfxRenderTarget::Clear(float _fDepth, zU8 _uStencil, bool _bClearDepth, bool _bClearStencil)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::zGfxRenderTarget* pRenderTarget = static_cast<ERes::zGfxRenderTarget*>(mpResource);
	pRenderTarget->Clear(_fDepth, _uStencil, _bClearDepth, _bClearStencil);
}

zGfxTexture2D zGfxRenderTarget::GetTexture2D()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	ERes::zGfxRenderTarget* pRenderTarget = static_cast<ERes::zGfxRenderTarget*>(mpResource);
	return pRenderTarget->GetTexture2D();
}

} } // namespace zen, zenRes

