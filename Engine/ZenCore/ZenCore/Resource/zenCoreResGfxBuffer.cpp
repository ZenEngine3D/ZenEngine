#include "zcCore.h"

namespace zen { namespace zenRes {

//=================================================================================================
// CREATES
//=================================================================================================
zGfxTexture2D zGfxTexture2D::Create(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, zFlagResTexUse _CreationFlags)
{
	static zArrayStatic<zU8> aEmpty(zUInt(0));
	return zcExp::CreateGfxTexture2D(_eFormat, _CreationFlags, _vDim, aEmpty);
}

zGfxTexture2D zGfxTexture2D::Create(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, const zArrayBase<zU8>& _aRawData, zFlagResTexUse _CreationFlags)
{
	return zcExp::CreateGfxTexture2D(_eFormat, _CreationFlags, _vDim, _aRawData);
}

zGfxTarget2D zGfxTarget2D::Create(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, bool _bSrgb)
{
	return zcExp::CreateGfxRenderTarget(_eFormat, _vDim, _bSrgb);
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
// GFX INDEX
//=================================================================================================
zU8* zGfxIndex::Lock()
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	return mpResource->Lock();
}

void zGfxIndex::Unlock(const zenGfx::zCommandList& rContext)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->Unlock(rContext);
}

//=================================================================================================
// GFX TEXTURE2D
//=================================================================================================
const zVec2U16& zGfxTexture2D::GetDim()
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	zcRes::GfxTexture2D* pTexture = static_cast<zcRes::GfxTexture2D*>(mpResource);
	return pTexture->GetDim();
}

//=================================================================================================
// GFX RENDER TARGET
//=================================================================================================
bool zGfxTarget2D::IsDepth() const
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	return mpResource->IsDepth();
}

const zVec2U16& zGfxTarget2D::GetDim() const
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	return mpResource->GetDim();
}

zGfxTexture2D zGfxTarget2D::GetTexture2D()
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	return mpResource->GetTexture2D();
}

//=================================================================================================
// GFX BUFFER
//=================================================================================================
void* zGfxBuffer::Lock()
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	return mpResource->Lock();
}

void zGfxBuffer::Unlock(const zenGfx::zCommandList& rContext)
{
	zenAssertMsg(mpResource, "No valid resource assigned");
	mpResource->Unlock(rContext);
}

zGfxBuffer zGfxBuffer::Create(size_t _uElemSize, zU32 _uElemCount/*, zFlagResTexUse _UseFlags*/)
{
	return zcExp::CreateGfxBuffer(nullptr, 0, _uElemSize, _uElemCount/*, _UseFlags*/);
}

zGfxBuffer zGfxBuffer::Create(const zU8* _pData, size_t _uDataSize, size_t _uElemSize, zU32 _uElemCount/*, zFlagResTexUse _UseFlags*/)
{
	return zcExp::CreateGfxBuffer(_pData, _uDataSize, _uElemSize, _uElemCount/*, _UseFlags*/);
}

} } // namespace zen, zenRes

