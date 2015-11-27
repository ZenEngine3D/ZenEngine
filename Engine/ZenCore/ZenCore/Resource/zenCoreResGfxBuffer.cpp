#include "zcCore.h"

namespace zen { namespace zenRes {

//=================================================================================================
// CREATES
//=================================================================================================
zGfxTexture2d zGfxTexture2d::Create(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, zFlagResTexUse _CreationFlags)
{
	static zArrayStatic<zU8> aEmpty(zUInt(0));
	return zcExp::CreateGfxTexture2D(_eFormat, _CreationFlags, _vDim, aEmpty);
}

zGfxTexture2d zGfxTexture2d::Create(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, const zArrayBase<zU8>& _aRawData, zFlagResTexUse _CreationFlags)
{
	return zcExp::CreateGfxTexture2D(_eFormat, _CreationFlags, _vDim, _aRawData);
}

zGfxRenderTarget zGfxRenderTarget::Create(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, bool _bSrgb)
{
	return zcExp::CreateGfxRenderTarget(_eFormat, _vDim, _bSrgb);
}

zGfxVertex zGfxVertex::Create(const zArrayBase<zGfxVertex::Stream>& _aStreams, zFlagResUse _ResourceUse)
{
	return zcExp::CreateGfxVertex(_aStreams, _ResourceUse);
}

zGfxIndex zGfxIndex::Create(const zArrayBase<zU16>& _Indices, zenConst::ePrimitiveType _ePrimitiveType)
{
	return zcExp::CreateGfxIndexBuffer(_Indices, _ePrimitiveType);
}

zGfxIndex zGfxIndex::Create(const zArrayBase<zU32>& _Indices, zenConst::ePrimitiveType _ePrimitiveType)
{
	return zcExp::CreateGfxIndexBuffer(_Indices, _ePrimitiveType);
}

//=================================================================================================
// GFX VERTEX
//=================================================================================================
//! @todo safe return array for bound safety
zU8* zGfxVertex::Lock()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxVertex* pVertex = static_cast<zcRes::GfxVertex*>(mpResource);
	return pVertex->Lock();
}
	
void zGfxVertex::Unlock()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxVertex* pVertex = static_cast<zcRes::GfxVertex*>(mpResource);
	return pVertex->Unlock();
}

//=================================================================================================
// GFX INDEX
//=================================================================================================
zU8* zGfxIndex::Lock()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxIndex* pIndex = static_cast<zcRes::GfxIndex*>(mpResource);
	return pIndex->Lock();
}

void zGfxIndex::Unlock()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxIndex* pIndex = static_cast<zcRes::GfxIndex*>(mpResource);
	return pIndex->Unlock();
}

//=================================================================================================
// GFX TEXTURE2D
//=================================================================================================
const zVec2U16& zGfxTexture2d::GetDim()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxTexture2d* pTexture = static_cast<zcRes::GfxTexture2d*>(mpResource);
	return pTexture->GetDim();
}

//=================================================================================================
// GFX RENDER TARGET
//=================================================================================================
bool zGfxRenderTarget::IsDepth()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxRenderTarget* pRenderTarget = static_cast<zcRes::GfxRenderTarget*>(mpResource);
	return pRenderTarget->IsDepth();
}

const zVec2U16& zGfxRenderTarget::GetDim()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxRenderTarget* pRenderTarget = static_cast<zcRes::GfxRenderTarget*>(mpResource);
	return pRenderTarget->GetDim();
}

void zGfxRenderTarget::Clear(const zVec4F& _vRGBA)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxRenderTarget* pRenderTarget = static_cast<zcRes::GfxRenderTarget*>(mpResource);
	pRenderTarget->Clear(_vRGBA);
}

void zGfxRenderTarget::Clear(float _fDepth, zU8 _uStencil, bool _bClearDepth, bool _bClearStencil)
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxRenderTarget* pRenderTarget = static_cast<zcRes::GfxRenderTarget*>(mpResource);
	pRenderTarget->Clear(_fDepth, _uStencil, _bClearDepth, _bClearStencil);
}

zGfxTexture2d zGfxRenderTarget::GetTexture2D()
{
	ZENAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxRenderTarget* pRenderTarget = static_cast<zcRes::GfxRenderTarget*>(mpResource);
	return pRenderTarget->GetTexture2D();
}

} } // namespace zen, zenRes

