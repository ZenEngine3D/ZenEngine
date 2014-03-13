#include "libZenCore.h"

namespace ERes
{

GfxInputSignature_DX11::GfxInputSignature_DX11()
{
}

GfxShader_DX11::GfxShader_DX11( EExp::SerialShader_DX11& _SerialItem, InstanceInfoBase& _CommonInstance )
: mBaseInstance(_CommonInstance)
, mBaseSerialItem(_SerialItem)
{
}

bool GfxShader_DX11::ResourceInit()
{
	mBaseInstance.mdTextureSamplerSlot.Init(8);
	zUInt uCount = mBaseSerialItem.maParamDefID.Count();
	mBaseInstance.maGfxShaderParamDefRef.SetCount( uCount );
	for(zUInt idx=0; idx<uCount; ++idx)
		mBaseInstance.maGfxShaderParamDefRef[idx] = mBaseSerialItem.maParamDefID[idx];

	mBaseInstance.mdTextureSamplerSlot.Import(mBaseSerialItem.maTextureSamplerName, mBaseSerialItem.maTextureSamplerSlot);
	return true;
}

GfxShaderParamDefRef GfxShader_DX11::GetShaderParamDef(EExp::eShaderParamFreq _eShaderParam) const
{
	return mBaseInstance.maGfxShaderParamDefRef.Count() > static_cast<zUInt>(_eShaderParam) ? mBaseInstance.maGfxShaderParamDefRef[_eShaderParam] : NULL;
}

GfxShaderVertex_DX11::GfxShaderVertex_DX11()
: Super(mInstanceInfo,mInstanceInfo)
{	
}

bool GfxShaderVertex_DX11::ResourceInit()
{ 
	HRESULT hr(-1);
	if( Super::ResourceInit() )
	{
		mInstanceInfo.mGfxInputSignatureRef = mInstanceInfo.mSerial.mShaderInputSignatureID;
		hr = EMgr::GfxRender.DX11GetDevice()->CreateVertexShader( mInstanceInfo.mSerialCommon.maCompiledShader.First(), mInstanceInfo.mSerialCommon.maCompiledShader.Size(), NULL, &mInstanceInfo.mpVertexShader );	
	}
	
	return SUCCEEDED(hr);
}

GfxShaderVertex_DX11::~GfxShaderVertex_DX11()
{
	mInstanceInfo.mpVertexShader->Release();
}

GfxShaderPixel_DX11::GfxShaderPixel_DX11()
: GfxShader_DX11(mInstanceInfo,mInstanceInfo)
{	
}

bool GfxShaderPixel_DX11::ResourceInit()
{ 
	HRESULT hr(-1);
	if( Super::ResourceInit() )
		hr = EMgr::GfxRender.DX11GetDevice()->CreatePixelShader( mInstanceInfo.mSerialCommon.maCompiledShader.First(), mInstanceInfo.mSerialCommon.maCompiledShader.Size(), NULL, &mInstanceInfo.mpPixelShader );

	return SUCCEEDED(hr);
}

GfxShaderPixel_DX11::~GfxShaderPixel_DX11()
{
	mInstanceInfo.mpPixelShader->Release();
}

}

