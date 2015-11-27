#include "zcCore.h"

namespace zcRes
{

GfxInputStreamProxy_DX11::GfxInputStreamProxy_DX11()
: mpInputLayout(NULL)
{
}

GfxInputStreamProxy_DX11::~GfxInputStreamProxy_DX11()
{
	if( mpInputLayout )
		mpInputLayout->Release();
	mpInputLayout = NULL;
}

bool GfxInputStreamProxy_DX11::Initialize(class GfxInputStream& _Owner)
{
	const GfxInputStream::ResDataRef& rResData = _Owner.GetResData();
	ZENAssert(rResData.IsValid());
	ZENDbgCode(mpOwner = &_Owner);

	bool bSuccess(FALSE);	
	mrVertexProxy	= rResData->mVertexBufferID;
	if( mrVertexProxy.IsValid() )
	{		
		mrSignatureProxy = rResData->mShaderInputSignatureID;
		if( mrSignatureProxy.IsValid() )
		{
			//! @todo make sure proxy not accessed gamethread
			HRESULT hr = zcMgr::GfxRender.DX11GetDevice()->CreateInputLayout( mrVertexProxy->GetProxy()->maElementDef.First(), mrVertexProxy->GetProxy()->maElementDef.Count(),
				mrSignatureProxy->GetProxy()->maDummyShaderCode.First(), mrSignatureProxy->GetProxy()->maDummyShaderCode.SizeMem(), &mpInputLayout );
			bSuccess = SUCCEEDED( hr );		
		}
	}	
	return bSuccess;
}

//=================================================================================================

GfxMeshProxy_DX11::GfxMeshProxy_DX11()
{	
}

GfxMeshProxy_DX11::~GfxMeshProxy_DX11()
{
}

bool GfxMeshProxy_DX11::Initialize(class GfxMesh& _Owner)
{
	const GfxMesh::ResDataRef& rResData = _Owner.GetResData();
	ZENAssert(rResData.IsValid());
	ZENDbgCode(mpOwner = &_Owner);

	marProxGfxMeshStrip.SetCount( rResData->maMeshStripID.Count() );
	for(zUInt stripIdx(0), stripCount(marProxGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marProxGfxMeshStrip[stripIdx] = rResData->maMeshStripID[stripIdx];

	//! @todo clean workaround until proxy/base class is sorted out
	_Owner.marGfxMeshStrip.SetCount( rResData->maMeshStripID.Count() );
	for(zUInt stripIdx(0), stripCount(marProxGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		_Owner.marGfxMeshStrip[stripIdx] = rResData->maMeshStripID[stripIdx];

	return true;
}

void GfxMeshProxy_DX11::SetValue(const zArrayBase<const zenRes::zShaderParameter*>& _aValues)
{
	for(zUInt stripIdx(0), stripCount(marProxGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marProxGfxMeshStrip[stripIdx]->SetValue(_aValues);
}
void GfxMeshProxy_DX11::SetValue(const zenRes::zShaderParameter& _Value)
{
	for(zUInt stripIdx(0), stripCount(marProxGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marProxGfxMeshStrip[stripIdx]->SetValue(_Value);
}
void GfxMeshProxy_DX11::SetValue(const zHash32& _hParamName, const float& _fValue)
{
	for(zUInt stripIdx(0), stripCount(marProxGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marProxGfxMeshStrip[stripIdx]->SetValue(_hParamName, _fValue);
}
void GfxMeshProxy_DX11::SetValue(const zHash32& _hParamName, const zVec2F& _vValue)
{
	for(zUInt stripIdx(0), stripCount(marProxGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marProxGfxMeshStrip[stripIdx]->SetValue(_hParamName, _vValue);
}
void GfxMeshProxy_DX11::SetValue(const zHash32& _hParamName, const zVec3F& _vValue)
{
	for(zUInt stripIdx(0), stripCount(marProxGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marProxGfxMeshStrip[stripIdx]->SetValue(_hParamName, _vValue);
}
void GfxMeshProxy_DX11::SetValue(const zHash32& _hParamName, const zVec4F& _vValue)
{
	for(zUInt stripIdx(0), stripCount(marProxGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marProxGfxMeshStrip[stripIdx]->SetValue(_hParamName, _vValue);
}
void GfxMeshProxy_DX11::SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	for(zUInt stripIdx(0), stripCount(marProxGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marProxGfxMeshStrip[stripIdx]->SetValue(_hParamName, _matValue);
}
void GfxMeshProxy_DX11::SetValue(const zHash32& _hTextureName, GfxTexture2dRef _rTexture )
{
	for(zUInt stripIdx(0), stripCount(marProxGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marProxGfxMeshStrip[stripIdx]->SetValue(_hTextureName, _rTexture);
}
void GfxMeshProxy_DX11::SetValue(const zHash32& _hTextureName, GfxSamplerRef _rSampler )
{
	for(zUInt stripIdx(0), stripCount(marProxGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marProxGfxMeshStrip[stripIdx]->SetValue(_hTextureName, _rSampler);
}
void GfxMeshProxy_DX11::SetValue(const zHash32& _hTextureName, GfxTexture2dRef _rTexture, GfxSamplerRef _rSampler )
{
	for(zUInt stripIdx(0), stripCount(marProxGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marProxGfxMeshStrip[stripIdx]->SetValue(_hTextureName, _rTexture, _rSampler);
}

//=================================================================================================

GfxMeshStripProxy_DX11::GfxMeshStripProxy_DX11()
{
}

GfxMeshStripProxy_DX11::~GfxMeshStripProxy_DX11()
{
}

bool GfxMeshStripProxy_DX11::Initialize(class GfxMeshStrip& _Owner)
{
	const GfxMeshStrip::ResDataRef& rResData = _Owner.GetResData();
	ZENAssert(rResData.IsValid());
	ZENDbgCode(mpOwner = &_Owner);

	mrIndexBufferProxy		= rResData->mIndexBufferID;
	mrInputStreamProxy		= rResData->mStreamBindingID;
	mrShaderBindingProxy	= rResData->mShaderBindingID;
	muVertexFirst			= rResData->muVertexFirst;
	muIndexFirst			= rResData->muIndexFirst;
	muIndexCount			= rResData->muIndexCount;
	marShaderParamProxy.SetCount(rResData->maShaderParamID.Count());
	for(zUInt idx(0), count(marShaderParamProxy.Count()); idx<count; ++idx)
		marShaderParamProxy[idx] = rResData->maShaderParamID[idx];
		
	marTextureProxy.SetCount( zenConst::keShaderStage__Count );
	marGfxSamplerProxy.SetCount( zenConst::keShaderStage__Count );	
	for(zUInt stageIdx(0), stageCount(rResData->maTextureID.Count()); stageIdx<stageCount; ++stageIdx )
	{	
		ZENAssert( rResData->maTextureID[stageIdx].Count() == rResData->maSamplerID[stageIdx].Count() );
		marTextureProxy[stageIdx].SetCount(rResData->maTextureID[stageIdx].Count());
		marGfxSamplerProxy[stageIdx].SetCount(rResData->maTextureID[stageIdx].Count());
		for(zUInt idx(0), count(marTextureProxy[stageIdx].Count()); idx<count; ++idx)
		{
			marTextureProxy[stageIdx][idx]		= rResData->maTextureID[stageIdx][idx];
			marGfxSamplerProxy[stageIdx][idx]	= rResData->maSamplerID[stageIdx][idx];
		}
	}
	//! @todo Missing: assign default texture for the one not assigned

	return true;
}

void GfxMeshStripProxy_DX11::SetValue(const zenRes::zShaderParameter& _Value)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mrShaderBindingProxy->GetProxy()->mdBufferPerParam[_Value.mhName];
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			marShaderParamProxy[idxStage]->GetProxy()->SetValue(_Value);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStripProxy_DX11::SetValue(const zArrayBase<const zenRes::zShaderParameter*>& _aValues)
{
	for(zUInt valIdx(0), valCount(_aValues.Count()); valIdx<valCount; ++valIdx)
	{
		zUInt idxStage							= 0;
		const zenRes::zShaderParameter* pValue	= _aValues[valIdx];
		zU32 paramMask							= mrShaderBindingProxy->GetProxy()->mdBufferPerParam[pValue->mhName];
		while( paramMask != 0 )
		{
			if( (paramMask&1)!=0 ){
				marShaderParamProxy[idxStage]->GetProxy()->SetValue(*pValue);
			}
			paramMask = paramMask>>1;
			++idxStage;
		}
	}
}

void GfxMeshStripProxy_DX11::SetValue(const zHash32& _hParamName, const float& _fValue)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mrShaderBindingProxy->GetProxy()->mdBufferPerParam[_hParamName];
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			marShaderParamProxy[idxStage]->GetProxy()->SetValue(_hParamName, _fValue);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStripProxy_DX11::SetValue(const zHash32& _hParamName, const zVec2F& _vValue)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mrShaderBindingProxy->GetProxy()->mdBufferPerParam[_hParamName];
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			marShaderParamProxy[idxStage]->GetProxy()->SetValue(_hParamName, _vValue);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStripProxy_DX11::SetValue(const zHash32& _hParamName, const zVec3F& _vValue)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mrShaderBindingProxy->GetProxy()->mdBufferPerParam[_hParamName];
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			marShaderParamProxy[idxStage]->GetProxy()->SetValue(_hParamName, _vValue);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStripProxy_DX11::SetValue(const zHash32& _hParamName, const zVec4F& _vValue)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mrShaderBindingProxy->GetProxy()->mdBufferPerParam[_hParamName];
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			marShaderParamProxy[idxStage]->GetProxy()->SetValue(_hParamName, _vValue);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}	
}

void GfxMeshStripProxy_DX11::SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mrShaderBindingProxy->GetProxy()->mdBufferPerParam[_hParamName];
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			marShaderParamProxy[idxStage]->GetProxy()->SetValue(_hParamName, _matValue);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStripProxy_DX11::SetValue(const zHash32& _hTextureName, GfxTexture2dRef _rTexture )
{		
	GfxShaderBindingResData::TextureSlot SlotInfos;
	if( mrShaderBindingProxy->GetProxy()->mdStageSlotPerTexture.Get(_hTextureName, SlotInfos) )
	{
		for(zUInt stageIdx(0); stageIdx<zenConst::keShaderStage__Count; ++stageIdx)
		{
			ZENWarningMsg( SlotInfos.muCount[stageIdx] <= 1, "Trying to set a single Texture to texture array.");
			if( SlotInfos.muCount[stageIdx] > 0 )
				marTextureProxy[stageIdx][SlotInfos.muSlot[stageIdx]] = _rTexture;
		}
	}	
}

void GfxMeshStripProxy_DX11::SetValue(const zHash32& _hTextureName, GfxSamplerRef _rSampler )
{
	GfxShaderBindingResData::TextureSlot SlotInfos;
	if( mrShaderBindingProxy->GetProxy()->mdStageSlotPerTexture.Get(_hTextureName, SlotInfos) )
	{
		for(zUInt stageIdx(0); stageIdx<zenConst::keShaderStage__Count; ++stageIdx)
		{
			ZENWarningMsg( SlotInfos.muCount[stageIdx] <= 1, "Trying to set a single Texture to texture array.");
			if( SlotInfos.muCount[stageIdx] > 0 )
				marGfxSamplerProxy[stageIdx][SlotInfos.muSlot[stageIdx]] = _rSampler;				
		}
	}	
}

void GfxMeshStripProxy_DX11::SetValue(const zHash32& _hTextureName, GfxTexture2dRef _rTexture, GfxSamplerRef _rSampler )
{
	GfxShaderBindingResData::TextureSlot SlotInfos;
	if( mrShaderBindingProxy->GetProxy()->mdStageSlotPerTexture.Get(_hTextureName, SlotInfos) )
	{
		for(zUInt stageIdx(0); stageIdx<zenConst::keShaderStage__Count; ++stageIdx)
		{
			ZENWarningMsg( SlotInfos.muCount[stageIdx] <= 1, "Trying to set a single Texture to texture array.");
			if( SlotInfos.muCount[stageIdx] > 0 )
			{
				marTextureProxy[stageIdx][SlotInfos.muSlot[stageIdx]] = _rTexture;
				marGfxSamplerProxy[stageIdx][SlotInfos.muSlot[stageIdx]] = _rSampler;
			}
		}
	}	
}

//=================================================================================================

GfxShaderBindingProxy_DX11::GfxShaderBindingProxy_DX11()
{	
}

GfxShaderBindingProxy_DX11::~GfxShaderBindingProxy_DX11()
{
}

bool GfxShaderBindingProxy_DX11::Initialize(class GfxShaderBinding& _Owner)
{
	const GfxShaderBinding::ResDataRef& rResData = _Owner.GetResData();
	ZENAssert(rResData.IsValid());
	ZENDbgCode(mpOwner = &_Owner);

	mrProxShaderVertex	= rResData->maShaderID[zenConst::keShaderStage_Vertex];
	mrProxShaderPixel	= rResData->maShaderID[zenConst::keShaderStage_Pixel];
	
	GfxShaderBindingResData::TextureSlot emptySlot;
	mdStageSlotPerTexture.Init(rResData->maTextureName.Count()*2);
	mdStageSlotPerTexture.SetDefaultValue(emptySlot);
	mdStageSlotPerTexture.Import( rResData->maTextureName, rResData->maTextureBind );

	mdBufferPerParam.Init(rResData->maParameterName.Count()*2);
	mdBufferPerParam.SetDefaultValue(0);
	mdBufferPerParam.Import( rResData->maParameterName, rResData->maParameterMask );

	maStagePerParamDef = rResData->maStagePerParamDef;
	return true;
}


}
