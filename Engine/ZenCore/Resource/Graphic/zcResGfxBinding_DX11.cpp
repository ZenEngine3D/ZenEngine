#include "zcCore.h"

namespace zcRes
{

GfxInputStreamHAL_DX11::~GfxInputStreamHAL_DX11()
{
	if( mpInputLayout )
		mpInputLayout->Release();
	mpInputLayout = nullptr;
}

bool GfxInputStreamHAL_DX11::Initialize()
{
	bool bSuccess(FALSE);	
	mrVertexStream	= mVertexBufferID;
	mrSignature		= mShaderInputSignatureID;
	if( mrVertexStream.IsValid() && mrSignature.IsValid() )
	{
		HRESULT hr = zcMgr::GfxRender.DX11GetDevice()->CreateInputLayout(	mrVertexStream.HAL()->maElementDef.First(), 
																			mrVertexStream.HAL()->maElementDef.Count(),
																			mrSignature.HAL()->maDummyShaderCode.First(), 
																			mrSignature.HAL()->maDummyShaderCode.SizeMem(), 
																			&mpInputLayout );
		bSuccess = SUCCEEDED( hr );		
	}

	return bSuccess;
}

//=================================================================================================

bool GfxMeshHAL_DX11::Initialize()
{
	marGfxMeshStrip.SetCount( maMeshStripID.Count() );
	for(zUInt stripIdx(0), stripCount(marGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marGfxMeshStrip[stripIdx] = maMeshStripID[stripIdx];

	return true;
}

void GfxMeshHAL_DX11::SetValue(const zArrayBase<const zenRes::zShaderParameter*>& _aValues)
{
	for(zUInt stripIdx(0), stripCount(marGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marGfxMeshStrip[stripIdx]->SetValue(_aValues);
}
void GfxMeshHAL_DX11::SetValue(const zenRes::zShaderParameter& _Value)
{
	for(zUInt stripIdx(0), stripCount(marGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marGfxMeshStrip[stripIdx]->SetValue(_Value);
}
void GfxMeshHAL_DX11::SetValue(const zHash32& _hParamName, const float& _fValue)
{
	for(zUInt stripIdx(0), stripCount(marGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marGfxMeshStrip[stripIdx]->SetValue(_hParamName, _fValue);
}
void GfxMeshHAL_DX11::SetValue(const zHash32& _hParamName, const zVec2F& _vValue)
{
	for(zUInt stripIdx(0), stripCount(marGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marGfxMeshStrip[stripIdx]->SetValue(_hParamName, _vValue);
}
void GfxMeshHAL_DX11::SetValue(const zHash32& _hParamName, const zVec3F& _vValue)
{
	for(zUInt stripIdx(0), stripCount(marGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marGfxMeshStrip[stripIdx]->SetValue(_hParamName, _vValue);
}
void GfxMeshHAL_DX11::SetValue(const zHash32& _hParamName, const zVec4F& _vValue)
{
	for(zUInt stripIdx(0), stripCount(marGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marGfxMeshStrip[stripIdx]->SetValue(_hParamName, _vValue);
}
void GfxMeshHAL_DX11::SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	for(zUInt stripIdx(0), stripCount(marGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marGfxMeshStrip[stripIdx]->SetValue(_hParamName, _matValue);
}
void GfxMeshHAL_DX11::SetValue(const zHash32& _hTextureName, GfxTexture2dRef _rTexture )
{
	for(zUInt stripIdx(0), stripCount(marGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marGfxMeshStrip[stripIdx]->SetValue(_hTextureName, _rTexture);
}
void GfxMeshHAL_DX11::SetValue(const zHash32& _hTextureName, GfxSamplerRef _rSampler )
{
	for(zUInt stripIdx(0), stripCount(marGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marGfxMeshStrip[stripIdx]->SetValue(_hTextureName, _rSampler);
}
void GfxMeshHAL_DX11::SetValue(const zHash32& _hTextureName, GfxTexture2dRef _rTexture, GfxSamplerRef _rSampler )
{
	for(zUInt stripIdx(0), stripCount(marGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
		marGfxMeshStrip[stripIdx]->SetValue(_hTextureName, _rTexture, _rSampler);
}

//=================================================================================================

bool GfxMeshStripHAL_DX11::Initialize()
{
	mrIndexBuffer			= mIndexBufferID;
	mrInputStream			= mStreamBindingID;
	mrShaderBinding			= mShaderBindingID;

	marShaderParam.SetCount(maShaderParamID.Count());
	for(zUInt idx(0), count(marShaderParam.Count()); idx<count; ++idx)
		marShaderParam[idx] = maShaderParamID[idx];
		
	marTexture.SetCount( zenConst::keShaderStage__Count );
	marGfxSampler.SetCount( zenConst::keShaderStage__Count );	
	for(zUInt stageIdx(0), stageCount(maTextureID.Count()); stageIdx<stageCount; ++stageIdx )
	{	
		zenAssert( maTextureID[stageIdx].Count() == maSamplerID[stageIdx].Count() );
		marTexture[stageIdx].SetCount(maTextureID[stageIdx].Count());
		marGfxSampler[stageIdx].SetCount(maTextureID[stageIdx].Count());
		for(zUInt idx(0), count(marTexture[stageIdx].Count()); idx<count; ++idx)
		{
			marTexture[stageIdx][idx]		= maTextureID[stageIdx][idx];
			marGfxSampler[stageIdx][idx]	= maSamplerID[stageIdx][idx];
		}
	}
	//! @todo Missing: assign default texture for the one not assigned

	return true;
}

void GfxMeshStripHAL_DX11::SetValue(const zenRes::zShaderParameter& _Value)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mrShaderBinding.HAL()->mdBufferPerParam[_Value.mhName];
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 )
			marShaderParam[idxStage]->SetValue(_Value);
		
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStripHAL_DX11::SetValue(const zArrayBase<const zenRes::zShaderParameter*>& _aValues)
{
	for(zUInt valIdx(0), valCount(_aValues.Count()); valIdx<valCount; ++valIdx)
	{
		zUInt idxStage							= 0;
		const zenRes::zShaderParameter* pValue	= _aValues[valIdx];
		zU32 paramMask							= mrShaderBinding.HAL()->mdBufferPerParam[pValue->mhName];
		while( paramMask != 0 )
		{
			if( (paramMask&1)!=0 )
				marShaderParam[idxStage]->SetValue(*pValue);
			
			paramMask = paramMask>>1;
			++idxStage;
		}
	}
}

void GfxMeshStripHAL_DX11::SetValue(const zHash32& _hParamName, const float& _fValue)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mrShaderBinding.HAL()->mdBufferPerParam[_hParamName];
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 )
			marShaderParam[idxStage]->SetValue(_hParamName, _fValue);
		
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStripHAL_DX11::SetValue(const zHash32& _hParamName, const zVec2F& _vValue)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mrShaderBinding.HAL()->mdBufferPerParam[_hParamName];
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 )
			marShaderParam[idxStage]->SetValue(_hParamName, _vValue);
		
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStripHAL_DX11::SetValue(const zHash32& _hParamName, const zVec3F& _vValue)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mrShaderBinding.HAL()->mdBufferPerParam[_hParamName];
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 )
			marShaderParam[idxStage]->SetValue(_hParamName, _vValue);
		
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStripHAL_DX11::SetValue(const zHash32& _hParamName, const zVec4F& _vValue)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mrShaderBinding.HAL()->mdBufferPerParam[_hParamName];
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 )
			marShaderParam[idxStage]->SetValue(_hParamName, _vValue);
		
		paramMask = paramMask>>1;
		++idxStage;
	}	
}

void GfxMeshStripHAL_DX11::SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mrShaderBinding.HAL()->mdBufferPerParam[_hParamName];
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 )
			marShaderParam[idxStage]->SetValue(_hParamName, _matValue);
		
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStripHAL_DX11::SetValue(const zHash32& _hTextureName, GfxTexture2dRef _rTexture )
{		
	zcExp::ExportGfxShaderBinding::TextureSlot SlotInfos;
	if( mrShaderBinding.HAL()->mdStageSlotPerTexture.Get(_hTextureName, SlotInfos) )
	{
		for(zUInt stageIdx(0); stageIdx<zenConst::keShaderStage__Count; ++stageIdx)
		{
			zenWarningMsg( SlotInfos.muCount[stageIdx] <= 1, "Trying to set a single Texture to texture array.");
			if( SlotInfos.muCount[stageIdx] > 0 )
				marTexture[stageIdx][SlotInfos.muSlot[stageIdx]] = _rTexture;
		}
	}	
}

void GfxMeshStripHAL_DX11::SetValue(const zHash32& _hTextureName, GfxSamplerRef _rSampler )
{
	zcExp::ExportGfxShaderBinding::TextureSlot SlotInfos;
	if( mrShaderBinding.HAL()->mdStageSlotPerTexture.Get(_hTextureName, SlotInfos) )
	{
		for(zUInt stageIdx(0); stageIdx<zenConst::keShaderStage__Count; ++stageIdx)
		{
			zenWarningMsg( SlotInfos.muCount[stageIdx] <= 1, "Trying to set a single Texture to texture array.");
			if( SlotInfos.muCount[stageIdx] > 0 )
				marGfxSampler[stageIdx][SlotInfos.muSlot[stageIdx]] = _rSampler;				
		}
	}	
}

void GfxMeshStripHAL_DX11::SetValue(const zHash32& _hTextureName, GfxTexture2dRef _rTexture, GfxSamplerRef _rSampler )
{
	zcExp::ExportGfxShaderBinding::TextureSlot SlotInfos;
	if( mrShaderBinding.HAL()->mdStageSlotPerTexture.Get(_hTextureName, SlotInfos) )
	{
		for(zUInt stageIdx(0); stageIdx<zenConst::keShaderStage__Count; ++stageIdx)
		{
			zenWarningMsg( SlotInfos.muCount[stageIdx] <= 1, "Trying to set a single Texture to texture array.");
			if( SlotInfos.muCount[stageIdx] > 0 )
			{
				marTexture[stageIdx][SlotInfos.muSlot[stageIdx]] = _rTexture;
				marGfxSampler[stageIdx][SlotInfos.muSlot[stageIdx]] = _rSampler;
			}
		}
	}	
}

//=================================================================================================
bool GfxShaderBindingHAL_DX11::Initialize()
{
	for(zUInt idx(0); idx<zenConst::keShaderStage__Count; ++idx)
		if( maShaderID[idx].IsValid() )
			marShader[idx] =  maShaderID[idx];
			
	mdStageSlotPerTexture.Init(maTextureName.Count()*2);	
	mdStageSlotPerTexture.Import( maTextureName, maTextureBind );
	mdStageSlotPerTexture.SetDefaultValue(zcExp::ExportGfxShaderBinding::TextureSlot());

	mdBufferPerParam.Init(maParameterName.Count()*2);	
	mdBufferPerParam.Import( maParameterName, maParameterMask );
	mdBufferPerParam.SetDefaultValue(0);
	return true;
}

void GfxShaderBindingHAL_DX11::CreateShaderParam(zArrayStatic<zenRes::zGfxShaderParam>& _aShaderParamOut)const
{
	_aShaderParamOut.SetCount( maParamDefID.Count() );
	for(int idx(0), count(maParamDefID.Count()); idx<count; ++idx)
		_aShaderParamOut[idx] = zcExp::CreateGfxShaderParam( maParamDefID[idx] );
}

}
