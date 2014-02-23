#include "libZenCore.h"

namespace ERes
{

GfxInputStream_DX11::GfxInputStream_DX11()
{
}

GfxInputStream_DX11::~GfxInputStream_DX11()
{
	mInstanceInfo.mpInputLayout->Release();
}

bool GfxInputStream_DX11::ResourceInit()
{
	bool bSuccess(FALSE);
	mInstanceInfo.mrVertex		= mInstanceInfo.mSerial.mVertexBufferID;
	mInstanceInfo.mrSignature	= mInstanceInfo.mSerial.mShaderInputSignatureID;
	if( mInstanceInfo.mrVertex.IsValid() && mInstanceInfo.mrSignature.IsValid() )
	{
		HRESULT hr = EMgr::GfxRender.DX11GetDevice()->CreateInputLayout( mInstanceInfo.mrVertex->Get().maElementDef.First(), mInstanceInfo.mrVertex->Get().maElementDef.Count(), 
			mInstanceInfo.mrSignature->Get().maDummyShaderCode.First(), mInstanceInfo.mrSignature->Get().maDummyShaderCode.Size(), &mInstanceInfo.mpInputLayout );
		bSuccess = SUCCEEDED( hr );		
	}	
	return bSuccess;
}

GfxShaderBinding_DX11::GfxShaderBinding_DX11()
: Super()
{	
}

bool GfxShaderBinding_DX11::ResourceInit()
{
	mInstanceInfo.maGfxShaderRef.Copy<zenResID>( mInstanceInfo.mSerial.maShaderID );	

	EExp::SerialGfxShaderBinding_DX11::TextureSlot emptySlot;
	mInstanceInfo.mdStageSlotPerTexture.Init(mInstanceInfo.maTextureName.Count()*2);
	mInstanceInfo.mdStageSlotPerTexture.SetDefaultValue(emptySlot);
	mInstanceInfo.mdStageSlotPerTexture.Import( mInstanceInfo.maTextureName, mInstanceInfo.maTextureBind );

	mInstanceInfo.mdCBuferPerParam.Init(mInstanceInfo.maParameterName.Count()*2);
	mInstanceInfo.mdCBuferPerParam.SetDefaultValue(0);
	mInstanceInfo.mdCBuferPerParam.Import( mInstanceInfo.maParameterName, mInstanceInfo.maParameterMask );

	return true;
}

GfxMeshStrip_DX11::GfxMeshStrip_DX11()
{
}

bool GfxMeshStrip_DX11::ResourceInit()
{
	mInstanceInfo.mrIndexBuffer		= mInstanceInfo.mSerial.mIndexBufferID;
	mInstanceInfo.mrInputStream		= mInstanceInfo.mSerial.mStreamBindingID;
	mInstanceInfo.mrShaderBinding	= mInstanceInfo.mSerial.mShaderBindingID;	
	mInstanceInfo.maShaderParamRef.Copy<zenResID>( mInstanceInfo.mSerial.maShaderParamID );
	
	mInstanceInfo.maTextureRef.SetCount( EExp::keShaderStage__Count );
	mInstanceInfo.maGfxSamplerRef.SetCount( EExp::keShaderStage__Count );	
	for(zenUInt stageIdx(0), stageCount(mInstanceInfo.mSerial.maTextureID.Count()); stageIdx<stageCount; ++stageIdx )
	{	
		AWAssert( mInstanceInfo.mSerial.maTextureID[stageIdx].Count() == mInstanceInfo.mSerial.maSamplerID[stageIdx].Count() );
		mInstanceInfo.maTextureRef[stageIdx].Copy<zenResID>( mInstanceInfo.mSerial.maTextureID[stageIdx] );
		mInstanceInfo.maGfxSamplerRef[stageIdx].Copy<zenResID>( mInstanceInfo.mSerial.maSamplerID[stageIdx] );
	}
	//! @todo assign default texture for the one not assigned

	return true;
}


void GfxMeshStrip_DX11::SetValue(const awres::awShaderParameter& _Value)
{
	zenUInt idxStage	= 0;
	zenU32 paramMask	= mInstanceInfo.mrShaderBinding->Get().mdCBuferPerParam[_Value.mhName];	
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			mInstanceInfo.maShaderParamRef[idxStage]->SetValue(_Value);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStrip_DX11::SetValue(const zenArrayBase<const awres::awShaderParameter*>& _aValues)
{
	for(zenUInt valIdx(0), valCount(_aValues.Count()); valIdx<valCount; ++valIdx)
	{
		zenUInt idxStage							= 0;
		const awres::awShaderParameter* pValue	= _aValues[valIdx];
		zenU32 paramMask							= mInstanceInfo.mrShaderBinding->Get().mdCBuferPerParam[pValue->mhName];
		while( paramMask != 0 )
		{
			if( (paramMask&1)!=0 ){
				mInstanceInfo.maShaderParamRef[idxStage]->SetValue(*pValue);
			}
			paramMask = paramMask>>1;
			++idxStage;
		}
	}
}

void GfxMeshStrip_DX11::SetValue(const zenHash32& _hParamName, const float& _fValue)
{
	zenUInt idxStage	= 0;
	zenU32 paramMask	= mInstanceInfo.mrShaderBinding->Get().mdCBuferPerParam[_hParamName];	
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			mInstanceInfo.maShaderParamRef[idxStage]->SetValue(_hParamName, _fValue);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStrip_DX11::SetValue(const zenHash32& _hParamName, const zenVec2F& _vValue)
{
	zenUInt idxStage	= 0;
	zenU32 paramMask	= mInstanceInfo.mrShaderBinding->Get().mdCBuferPerParam[_hParamName];	
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			mInstanceInfo.maShaderParamRef[idxStage]->SetValue(_hParamName, _vValue);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStrip_DX11::SetValue(const zenHash32& _hParamName, const zenVec3F& _vValue)
{
	zenUInt idxStage	= 0;
	zenU32 paramMask	= mInstanceInfo.mrShaderBinding->Get().mdCBuferPerParam[_hParamName];	
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			mInstanceInfo.maShaderParamRef[idxStage]->SetValue(_hParamName, _vValue);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStrip_DX11::SetValue(const zenHash32& _hParamName, const zenVec4F& _vValue)
{
	zenUInt idxStage	= 0;
	zenU32 paramMask	= mInstanceInfo.mrShaderBinding->Get().mdCBuferPerParam[_hParamName];	
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			mInstanceInfo.maShaderParamRef[idxStage]->SetValue(_hParamName, _vValue);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}	
}

void GfxMeshStrip_DX11::SetValue(const zenHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	zenUInt idxStage	= 0;
	zenU32 paramMask	= mInstanceInfo.mrShaderBinding->Get().mdCBuferPerParam[_hParamName];	
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			mInstanceInfo.maShaderParamRef[idxStage]->SetValue(_hParamName, _matValue);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStrip_DX11::SetValue(const zenHash32& _hTextureName, GfxTexture2DRef _rTexture )
{		
	EExp::SerialGfxShaderBinding_DX11::TextureSlot SlotInfos;
	if( mInstanceInfo.mrShaderBinding->Get().mdStageSlotPerTexture.Get(_hTextureName, SlotInfos) )
	{
		for(zenUInt stageIdx(0); stageIdx<EExp::keShaderStage__Count; ++stageIdx)
		{
			AWWarningMsg( SlotInfos.muCount[stageIdx] <= 1, "Trying to set a single Texture to texture array.");
			if( SlotInfos.muCount[stageIdx] > 0 )
				mInstanceInfo.maTextureRef[stageIdx][SlotInfos.muSlot[stageIdx]] = _rTexture;
		}
	}	
}

void GfxMeshStrip_DX11::SetValue(const zenHash32& _hTextureName, GfxSamplerRef _rSampler )
{
	EExp::SerialGfxShaderBinding_DX11::TextureSlot SlotInfos;
	if( mInstanceInfo.mrShaderBinding->Get().mdStageSlotPerTexture.Get(_hTextureName, SlotInfos) )
	{
		for(zenUInt stageIdx(0); stageIdx<EExp::keShaderStage__Count; ++stageIdx)
		{
			AWWarningMsg( SlotInfos.muCount[stageIdx] <= 1, "Trying to set a single Texture to texture array.");
			if( SlotInfos.muCount[stageIdx] > 0 )
				mInstanceInfo.maGfxSamplerRef[stageIdx][SlotInfos.muSlot[stageIdx]] = _rSampler;				
		}
	}	
}

void GfxMeshStrip_DX11::SetValue(const zenHash32& _hTextureName, GfxTexture2DRef _rTexture, GfxSamplerRef _rSampler )
{
	EExp::SerialGfxShaderBinding_DX11::TextureSlot SlotInfos;
	if( mInstanceInfo.mrShaderBinding->Get().mdStageSlotPerTexture.Get(_hTextureName, SlotInfos) )
	{
		for(zenUInt stageIdx(0); stageIdx<EExp::keShaderStage__Count; ++stageIdx)
		{
			AWWarningMsg( SlotInfos.muCount[stageIdx] <= 1, "Trying to set a single Texture to texture array.");
			if( SlotInfos.muCount[stageIdx] > 0 )
			{
				mInstanceInfo.maTextureRef[stageIdx][SlotInfos.muSlot[stageIdx]] = _rTexture;
				mInstanceInfo.maGfxSamplerRef[stageIdx][SlotInfos.muSlot[stageIdx]] = _rSampler;
			}
		}
	}	
}

//=================================================================================================

GfxMesh_DX11::GfxMesh_DX11()
: Super()
{	
}

bool GfxMesh_DX11::ResourceInit()
{
	mInstanceInfo.maGfxMeshStripRef.Copy<zenResID>( mInstanceInfo.mSerial.maMeshStripID );	
	return true;
}

void GfxMesh_DX11::SetValue(const zenArrayBase<const awres::awShaderParameter*>& _aValues)
{
	for(zenUInt stripIdx(0), stripCount(mInstanceInfo.maGfxMeshStripRef.Count()); stripIdx<stripCount; ++stripIdx)
		mInstanceInfo.maGfxMeshStripRef[stripIdx]->SetValue(_aValues);
}
void GfxMesh_DX11::SetValue(const awres::awShaderParameter& _Value)
{
	for(zenUInt stripIdx(0), stripCount(mInstanceInfo.maGfxMeshStripRef.Count()); stripIdx<stripCount; ++stripIdx)
		mInstanceInfo.maGfxMeshStripRef[stripIdx]->SetValue(_Value);
}
void GfxMesh_DX11::SetValue(const zenHash32& _hParamName, const float& _fValue)
{
	for(zenUInt stripIdx(0), stripCount(mInstanceInfo.maGfxMeshStripRef.Count()); stripIdx<stripCount; ++stripIdx)
		mInstanceInfo.maGfxMeshStripRef[stripIdx]->SetValue(_hParamName, _fValue);
}
void GfxMesh_DX11::SetValue(const zenHash32& _hParamName, const zenVec2F& _vValue)
{
	for(zenUInt stripIdx(0), stripCount(mInstanceInfo.maGfxMeshStripRef.Count()); stripIdx<stripCount; ++stripIdx)
		mInstanceInfo.maGfxMeshStripRef[stripIdx]->SetValue(_hParamName, _vValue);
}
void GfxMesh_DX11::SetValue(const zenHash32& _hParamName, const zenVec3F& _vValue)
{
	for(zenUInt stripIdx(0), stripCount(mInstanceInfo.maGfxMeshStripRef.Count()); stripIdx<stripCount; ++stripIdx)
		mInstanceInfo.maGfxMeshStripRef[stripIdx]->SetValue(_hParamName, _vValue);
}
void GfxMesh_DX11::SetValue(const zenHash32& _hParamName, const zenVec4F& _vValue)
{
	for(zenUInt stripIdx(0), stripCount(mInstanceInfo.maGfxMeshStripRef.Count()); stripIdx<stripCount; ++stripIdx)
		mInstanceInfo.maGfxMeshStripRef[stripIdx]->SetValue(_hParamName, _vValue);
}
void GfxMesh_DX11::SetValue(const zenHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	for(zenUInt stripIdx(0), stripCount(mInstanceInfo.maGfxMeshStripRef.Count()); stripIdx<stripCount; ++stripIdx)
		mInstanceInfo.maGfxMeshStripRef[stripIdx]->SetValue(_hParamName, _matValue);
}
void GfxMesh_DX11::SetValue(const zenHash32& _hTextureName, GfxTexture2DRef _rTexture )
{
	for(zenUInt stripIdx(0), stripCount(mInstanceInfo.maGfxMeshStripRef.Count()); stripIdx<stripCount; ++stripIdx)
		mInstanceInfo.maGfxMeshStripRef[stripIdx]->SetValue(_hTextureName, _rTexture);
}
void GfxMesh_DX11::SetValue(const zenHash32& _hTextureName, GfxSamplerRef _rSampler )
{
	for(zenUInt stripIdx(0), stripCount(mInstanceInfo.maGfxMeshStripRef.Count()); stripIdx<stripCount; ++stripIdx)
		mInstanceInfo.maGfxMeshStripRef[stripIdx]->SetValue(_hTextureName, _rSampler);
}
void GfxMesh_DX11::SetValue(const zenHash32& _hTextureName, GfxTexture2DRef _rTexture, GfxSamplerRef _rSampler )
{
	for(zenUInt stripIdx(0), stripCount(mInstanceInfo.maGfxMeshStripRef.Count()); stripIdx<stripCount; ++stripIdx)
		mInstanceInfo.maGfxMeshStripRef[stripIdx]->SetValue(_hTextureName, _rTexture, _rSampler);
}


}
