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
	mInstanceInfo.maGfxShaderRef.Copy<zResID>( mInstanceInfo.mSerial.maShaderID );	

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
	mInstanceInfo.maShaderParamRef.Copy<zResID>( mInstanceInfo.mSerial.maShaderParamID );
	
	mInstanceInfo.maTextureRef.SetCount( EExp::keShaderStage__Count );
	mInstanceInfo.maGfxSamplerRef.SetCount( EExp::keShaderStage__Count );	
	for(zUInt stageIdx(0), stageCount(mInstanceInfo.mSerial.maTextureID.Count()); stageIdx<stageCount; ++stageIdx )
	{	
		ZENAssert( mInstanceInfo.mSerial.maTextureID[stageIdx].Count() == mInstanceInfo.mSerial.maSamplerID[stageIdx].Count() );
		mInstanceInfo.maTextureRef[stageIdx].Copy<zResID>( mInstanceInfo.mSerial.maTextureID[stageIdx] );
		mInstanceInfo.maGfxSamplerRef[stageIdx].Copy<zResID>( mInstanceInfo.mSerial.maSamplerID[stageIdx] );
	}
	//! @todo Missing: assign default texture for the one not assigned

	return true;
}


void GfxMeshStrip_DX11::SetValue(const zenRes::zShaderParameter& _Value)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mInstanceInfo.mrShaderBinding->Get().mdCBuferPerParam[_Value.mhName];	
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			mInstanceInfo.maShaderParamRef[idxStage]->SetValue(_Value);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStrip_DX11::SetValue(const zArrayBase<const zenRes::zShaderParameter*>& _aValues)
{
	for(zUInt valIdx(0), valCount(_aValues.Count()); valIdx<valCount; ++valIdx)
	{
		zUInt idxStage							= 0;
		const zenRes::zShaderParameter* pValue	= _aValues[valIdx];
		zU32 paramMask							= mInstanceInfo.mrShaderBinding->Get().mdCBuferPerParam[pValue->mhName];
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

void GfxMeshStrip_DX11::SetValue(const zHash32& _hParamName, const float& _fValue)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mInstanceInfo.mrShaderBinding->Get().mdCBuferPerParam[_hParamName];	
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			mInstanceInfo.maShaderParamRef[idxStage]->SetValue(_hParamName, _fValue);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStrip_DX11::SetValue(const zHash32& _hParamName, const zVec2F& _vValue)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mInstanceInfo.mrShaderBinding->Get().mdCBuferPerParam[_hParamName];	
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			mInstanceInfo.maShaderParamRef[idxStage]->SetValue(_hParamName, _vValue);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStrip_DX11::SetValue(const zHash32& _hParamName, const zVec3F& _vValue)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mInstanceInfo.mrShaderBinding->Get().mdCBuferPerParam[_hParamName];	
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			mInstanceInfo.maShaderParamRef[idxStage]->SetValue(_hParamName, _vValue);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStrip_DX11::SetValue(const zHash32& _hParamName, const zVec4F& _vValue)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mInstanceInfo.mrShaderBinding->Get().mdCBuferPerParam[_hParamName];	
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			mInstanceInfo.maShaderParamRef[idxStage]->SetValue(_hParamName, _vValue);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}	
}

void GfxMeshStrip_DX11::SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	zUInt idxStage	= 0;
	zU32 paramMask	= mInstanceInfo.mrShaderBinding->Get().mdCBuferPerParam[_hParamName];	
	while( paramMask != 0 )
	{
		if( (paramMask&1)!=0 ){
			mInstanceInfo.maShaderParamRef[idxStage]->SetValue(_hParamName, _matValue);
		}
		paramMask = paramMask>>1;
		++idxStage;
	}
}

void GfxMeshStrip_DX11::SetValue(const zHash32& _hTextureName, GfxTexture2DRef _rTexture )
{		
	EExp::SerialGfxShaderBinding_DX11::TextureSlot SlotInfos;
	if( mInstanceInfo.mrShaderBinding->Get().mdStageSlotPerTexture.Get(_hTextureName, SlotInfos) )
	{
		for(zUInt stageIdx(0); stageIdx<EExp::keShaderStage__Count; ++stageIdx)
		{
			ZENWarningMsg( SlotInfos.muCount[stageIdx] <= 1, "Trying to set a single Texture to texture array.");
			if( SlotInfos.muCount[stageIdx] > 0 )
				mInstanceInfo.maTextureRef[stageIdx][SlotInfos.muSlot[stageIdx]] = _rTexture;
		}
	}	
}

void GfxMeshStrip_DX11::SetValue(const zHash32& _hTextureName, GfxSamplerRef _rSampler )
{
	EExp::SerialGfxShaderBinding_DX11::TextureSlot SlotInfos;
	if( mInstanceInfo.mrShaderBinding->Get().mdStageSlotPerTexture.Get(_hTextureName, SlotInfos) )
	{
		for(zUInt stageIdx(0); stageIdx<EExp::keShaderStage__Count; ++stageIdx)
		{
			ZENWarningMsg( SlotInfos.muCount[stageIdx] <= 1, "Trying to set a single Texture to texture array.");
			if( SlotInfos.muCount[stageIdx] > 0 )
				mInstanceInfo.maGfxSamplerRef[stageIdx][SlotInfos.muSlot[stageIdx]] = _rSampler;				
		}
	}	
}

void GfxMeshStrip_DX11::SetValue(const zHash32& _hTextureName, GfxTexture2DRef _rTexture, GfxSamplerRef _rSampler )
{
	EExp::SerialGfxShaderBinding_DX11::TextureSlot SlotInfos;
	if( mInstanceInfo.mrShaderBinding->Get().mdStageSlotPerTexture.Get(_hTextureName, SlotInfos) )
	{
		for(zUInt stageIdx(0); stageIdx<EExp::keShaderStage__Count; ++stageIdx)
		{
			ZENWarningMsg( SlotInfos.muCount[stageIdx] <= 1, "Trying to set a single Texture to texture array.");
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
	mInstanceInfo.maGfxMeshStripRef.Copy<zResID>( mInstanceInfo.mSerial.maMeshStripID );	
	return true;
}

void GfxMesh_DX11::SetValue(const zArrayBase<const zenRes::zShaderParameter*>& _aValues)
{
	for(zUInt stripIdx(0), stripCount(mInstanceInfo.maGfxMeshStripRef.Count()); stripIdx<stripCount; ++stripIdx)
		mInstanceInfo.maGfxMeshStripRef[stripIdx]->SetValue(_aValues);
}
void GfxMesh_DX11::SetValue(const zenRes::zShaderParameter& _Value)
{
	for(zUInt stripIdx(0), stripCount(mInstanceInfo.maGfxMeshStripRef.Count()); stripIdx<stripCount; ++stripIdx)
		mInstanceInfo.maGfxMeshStripRef[stripIdx]->SetValue(_Value);
}
void GfxMesh_DX11::SetValue(const zHash32& _hParamName, const float& _fValue)
{
	for(zUInt stripIdx(0), stripCount(mInstanceInfo.maGfxMeshStripRef.Count()); stripIdx<stripCount; ++stripIdx)
		mInstanceInfo.maGfxMeshStripRef[stripIdx]->SetValue(_hParamName, _fValue);
}
void GfxMesh_DX11::SetValue(const zHash32& _hParamName, const zVec2F& _vValue)
{
	for(zUInt stripIdx(0), stripCount(mInstanceInfo.maGfxMeshStripRef.Count()); stripIdx<stripCount; ++stripIdx)
		mInstanceInfo.maGfxMeshStripRef[stripIdx]->SetValue(_hParamName, _vValue);
}
void GfxMesh_DX11::SetValue(const zHash32& _hParamName, const zVec3F& _vValue)
{
	for(zUInt stripIdx(0), stripCount(mInstanceInfo.maGfxMeshStripRef.Count()); stripIdx<stripCount; ++stripIdx)
		mInstanceInfo.maGfxMeshStripRef[stripIdx]->SetValue(_hParamName, _vValue);
}
void GfxMesh_DX11::SetValue(const zHash32& _hParamName, const zVec4F& _vValue)
{
	for(zUInt stripIdx(0), stripCount(mInstanceInfo.maGfxMeshStripRef.Count()); stripIdx<stripCount; ++stripIdx)
		mInstanceInfo.maGfxMeshStripRef[stripIdx]->SetValue(_hParamName, _vValue);
}
void GfxMesh_DX11::SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue)
{
	for(zUInt stripIdx(0), stripCount(mInstanceInfo.maGfxMeshStripRef.Count()); stripIdx<stripCount; ++stripIdx)
		mInstanceInfo.maGfxMeshStripRef[stripIdx]->SetValue(_hParamName, _matValue);
}
void GfxMesh_DX11::SetValue(const zHash32& _hTextureName, GfxTexture2DRef _rTexture )
{
	for(zUInt stripIdx(0), stripCount(mInstanceInfo.maGfxMeshStripRef.Count()); stripIdx<stripCount; ++stripIdx)
		mInstanceInfo.maGfxMeshStripRef[stripIdx]->SetValue(_hTextureName, _rTexture);
}
void GfxMesh_DX11::SetValue(const zHash32& _hTextureName, GfxSamplerRef _rSampler )
{
	for(zUInt stripIdx(0), stripCount(mInstanceInfo.maGfxMeshStripRef.Count()); stripIdx<stripCount; ++stripIdx)
		mInstanceInfo.maGfxMeshStripRef[stripIdx]->SetValue(_hTextureName, _rSampler);
}
void GfxMesh_DX11::SetValue(const zHash32& _hTextureName, GfxTexture2DRef _rTexture, GfxSamplerRef _rSampler )
{
	for(zUInt stripIdx(0), stripCount(mInstanceInfo.maGfxMeshStripRef.Count()); stripIdx<stripCount; ++stripIdx)
		mInstanceInfo.maGfxMeshStripRef[stripIdx]->SetValue(_hTextureName, _rTexture, _rSampler);
}


}
