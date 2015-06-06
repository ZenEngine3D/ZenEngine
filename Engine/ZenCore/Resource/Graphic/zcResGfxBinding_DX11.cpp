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
	const GfxInputStream::ExportDataRef& rExportData = _Owner.GetExportData();
	ZENAssert(rExportData.IsValid());
	ZENDbgCode(mpOwner = &_Owner);

	bool bSuccess(FALSE);
	mrVertexProxy	= GetResourceProxy<GfxVertexRef>(rExportData->mVertexBufferID);
	if( mrVertexProxy.IsValid() )
	{
		
		mrSignatureProxy		= GetResourceProxy<GfxInputSignatureRef>(rExportData->mShaderInputSignatureID);
		if( mrSignatureProxy.IsValid() )
		{
			//! @todo make sure proxy not accessed gamethread
			HRESULT hr = EMgr::GfxRender.DX11GetDevice()->CreateInputLayout( mrVertexProxy->maElementDef.First(), mrVertexProxy->maElementDef.Count(), 
				mrSignatureProxy->maDummyShaderCode.First(), mrSignatureProxy->maDummyShaderCode.Size(), &mpInputLayout );
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
	const GfxMesh::ExportDataRef& rExportData = _Owner.GetExportData();
	ZENAssert(rExportData.IsValid());
	ZENDbgCode(mpOwner = &_Owner);

	marProxGfxMeshStrip.SetCount( rExportData->maMeshStripID.Count() );
	for(zUInt stripIdx(0), stripCount(marProxGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marProxGfxMeshStrip[stripIdx] = GetResourceProxy<GfxMeshStripRef>(rExportData->maMeshStripID[stripIdx]);
		
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
void GfxMeshProxy_DX11::SetValue(const zHash32& _hTextureName, GfxTexture2dProxyRef _rTexture )
{
	for(zUInt stripIdx(0), stripCount(marProxGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marProxGfxMeshStrip[stripIdx]->SetValue(_hTextureName, _rTexture);
}
void GfxMeshProxy_DX11::SetValue(const zHash32& _hTextureName, GfxSamplerProxyRef _rSampler )
{
	for(zUInt stripIdx(0), stripCount(marProxGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marProxGfxMeshStrip[stripIdx]->SetValue(_hTextureName, _rSampler);
}
void GfxMeshProxy_DX11::SetValue(const zHash32& _hTextureName, GfxTexture2dProxyRef _rTexture, GfxSamplerProxyRef _rSampler )
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
	const GfxMeshStrip::ExportDataRef& rExportData = _Owner.GetExportData();
	ZENAssert(rExportData.IsValid());
	ZENDbgCode(mpOwner = &_Owner);

	mrIndexBufferProxy		= GetResourceProxy<GfxIndexRef>(rExportData->mIndexBufferID);
	mrInputStreamProxy		= GetResourceProxy<GfxInputStreamRef>(rExportData->mStreamBindingID);
	mrShaderBindingProxy		= GetResourceProxy<GfxShaderBindingRef>(rExportData->mShaderBindingID);
	muIndexFirst		= rExportData->muIndexFirst;
	muIndexCount		= rExportData->muIndexCount;
	marShaderParamProxy.SetCount(rExportData->maShaderParamID.Count());
	for(zUInt idx(0), count(marShaderParamProxy.Count()); idx<count; ++idx)
		marShaderParamProxy[idx] = GetResourceProxy<GfxShaderParamRef>(rExportData->maShaderParamID[idx]);
		
	marTextureProxy.SetCount( zenConst::keShaderStage__Count );
	marGfxSamplerProxy.SetCount( zenConst::keShaderStage__Count );	
	for(zUInt stageIdx(0), stageCount(rExportData->maTextureID.Count()); stageIdx<stageCount; ++stageIdx )
	{	
		ZENAssert( rExportData->maTextureID[stageIdx].Count() == rExportData->maSamplerID[stageIdx].Count() );
		marTextureProxy[stageIdx].SetCount(rExportData->maTextureID[stageIdx].Count());
		marGfxSamplerProxy[stageIdx].SetCount(rExportData->maTextureID[stageIdx].Count());
		for(zUInt idx(0), count(marTextureProxy[stageIdx].Count()); idx<count; ++idx)
		{
			marTextureProxy[stageIdx][idx]		= GetResourceProxy<GfxTexture2dRef>(rExportData->maTextureID[stageIdx][idx]);
			marGfxSamplerProxy[stageIdx][idx]	= GetResourceProxy<GfxSamplerRef>(rExportData->maSamplerID[stageIdx][idx]);
		}
	}
	//! @todo Missing: assign default texture for the one not assigned

	return true;
}

void GfxMeshStripProxy_DX11::SetValue(const zenRes::zShaderParameter& _Value)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mrShaderBindingProxy->mdBufferPerParam[_Value.mhName];	
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			marShaderParamProxy[idxStage]->SetValue(_Value);
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
		zU32 paramMask							= mrShaderBindingProxy->mdBufferPerParam[pValue->mhName];
		while( paramMask != 0 )
		{
			if( (paramMask&1)!=0 ){
				marShaderParamProxy[idxStage]->SetValue(*pValue);
			}
			paramMask = paramMask>>1;
			++idxStage;
		}
	}
}

void GfxMeshStripProxy_DX11::SetValue(const zHash32& _hParamName, const float& _fValue)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mrShaderBindingProxy->mdBufferPerParam[_hParamName];	
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			marShaderParamProxy[idxStage]->SetValue(_hParamName, _fValue);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStripProxy_DX11::SetValue(const zHash32& _hParamName, const zVec2F& _vValue)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mrShaderBindingProxy->mdBufferPerParam[_hParamName];	
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			marShaderParamProxy[idxStage]->SetValue(_hParamName, _vValue);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStripProxy_DX11::SetValue(const zHash32& _hParamName, const zVec3F& _vValue)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mrShaderBindingProxy->mdBufferPerParam[_hParamName];	
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			marShaderParamProxy[idxStage]->SetValue(_hParamName, _vValue);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStripProxy_DX11::SetValue(const zHash32& _hParamName, const zVec4F& _vValue)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mrShaderBindingProxy->mdBufferPerParam[_hParamName];	
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			marShaderParamProxy[idxStage]->SetValue(_hParamName, _vValue);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}	
}

void GfxMeshStripProxy_DX11::SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mrShaderBindingProxy->mdBufferPerParam[_hParamName];	
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			marShaderParamProxy[idxStage]->SetValue(_hParamName, _matValue);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStripProxy_DX11::SetValue(const zHash32& _hTextureName, GfxTexture2dProxyRef _rTexture )
{		
	GfxShaderBindingExportData::TextureSlot SlotInfos;
	if( mrShaderBindingProxy->mdStageSlotPerTexture.Get(_hTextureName, SlotInfos) )
	{
		for(zUInt stageIdx(0); stageIdx<zenConst::keShaderStage__Count; ++stageIdx)
		{
			ZENWarningMsg( SlotInfos.muCount[stageIdx] <= 1, "Trying to set a single Texture to texture array.");
			if( SlotInfos.muCount[stageIdx] > 0 )
				marTextureProxy[stageIdx][SlotInfos.muSlot[stageIdx]] = _rTexture;
		}
	}	
}

void GfxMeshStripProxy_DX11::SetValue(const zHash32& _hTextureName, GfxSamplerProxyRef _rSampler )
{
	GfxShaderBindingExportData::TextureSlot SlotInfos;
	if( mrShaderBindingProxy->mdStageSlotPerTexture.Get(_hTextureName, SlotInfos) )
	{
		for(zUInt stageIdx(0); stageIdx<zenConst::keShaderStage__Count; ++stageIdx)
		{
			ZENWarningMsg( SlotInfos.muCount[stageIdx] <= 1, "Trying to set a single Texture to texture array.");
			if( SlotInfos.muCount[stageIdx] > 0 )
				marGfxSamplerProxy[stageIdx][SlotInfos.muSlot[stageIdx]] = _rSampler;				
		}
	}	
}

void GfxMeshStripProxy_DX11::SetValue(const zHash32& _hTextureName, GfxTexture2dProxyRef _rTexture, GfxSamplerProxyRef _rSampler )
{
	GfxShaderBindingExportData::TextureSlot SlotInfos;
	if( mrShaderBindingProxy->mdStageSlotPerTexture.Get(_hTextureName, SlotInfos) )
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
	const GfxShaderBinding::ExportDataRef& rExportData = _Owner.GetExportData();
	ZENAssert(rExportData.IsValid());
	ZENDbgCode(mpOwner = &_Owner);

	mrProxShaderVertex	= GetResourceProxy<GfxShaderVertexRef>(rExportData->maShaderID[zenConst::keShaderStage_Vertex]);
	mrProxShaderPixel	= GetResourceProxy<GfxShaderPixelRef>(rExportData->maShaderID[zenConst::keShaderStage_Pixel]);
	
	GfxShaderBindingExportData::TextureSlot emptySlot;
	mdStageSlotPerTexture.Init(rExportData->maTextureName.Count()*2);
	mdStageSlotPerTexture.SetDefaultValue(emptySlot);
	mdStageSlotPerTexture.Import( rExportData->maTextureName, rExportData->maTextureBind );

	mdBufferPerParam.Init(rExportData->maParameterName.Count()*2);
	mdBufferPerParam.SetDefaultValue(0);
	mdBufferPerParam.Import( rExportData->maParameterName, rExportData->maParameterMask );

	maStagePerParamDef = rExportData->maStagePerParamDef;
	return true;
}


}
