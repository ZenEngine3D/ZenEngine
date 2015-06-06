#include "zcCore.h"

namespace zcRes
{

//=================================================================================================
GfxInputSignatureProxy_DX11::GfxInputSignatureProxy_DX11()
{
}

GfxInputSignatureProxy_DX11::~GfxInputSignatureProxy_DX11()
{
}

bool GfxInputSignatureProxy_DX11::Initialize(class GfxInputSignature& _Owner)
{		
	ZENDbgCode(mpOwner = &_Owner);
	const GfxInputSignature::ExportDataRef& rExportData = _Owner.GetExportData();	
	maDummyShaderCode									= rExportData->maDummyShaderCode;
	return true;
}

//=================================================================================================
GfxShaderVertexProxy_DX11::GfxShaderVertexProxy_DX11()
: mpVertexShader(NULL)
{
}

GfxShaderVertexProxy_DX11::~GfxShaderVertexProxy_DX11()
{
	if( mpVertexShader )
		mpVertexShader->Release();
	mpVertexShader = NULL;
}

bool GfxShaderVertexProxy_DX11::Initialize(class GfxShaderVertex& _Owner)
{		
	ZENDbgCode(mpOwner = &_Owner);
	const GfxShaderVertex::ExportDataRef& rExportData = _Owner.GetExportData();	
	HRESULT hr = EMgr::GfxRender.DX11GetDevice()->CreateVertexShader( rExportData->maCompiledShader.First(), rExportData->maCompiledShader.Size(), NULL, &mpVertexShader );
	return SUCCEEDED(hr);
}

//=================================================================================================
GfxShaderPixelProxy_DX11::GfxShaderPixelProxy_DX11()
: mpPixelShader(NULL)
{
}

GfxShaderPixelProxy_DX11::~GfxShaderPixelProxy_DX11()
{
	if( mpPixelShader )
		mpPixelShader->Release();
	mpPixelShader = NULL;
}

bool GfxShaderPixelProxy_DX11::Initialize(class GfxShaderPixel& _Owner)
{		
	ZENDbgCode(mpOwner = &_Owner);
	const GfxShaderPixel::ExportDataRef& rExportData = _Owner.GetExportData();	
	HRESULT hr = EMgr::GfxRender.DX11GetDevice()->CreatePixelShader( rExportData->maCompiledShader.First(), rExportData->maCompiledShader.Size(), NULL, &mpPixelShader );
	return SUCCEEDED(hr);
}



/*
//! ########################################################################################################
//! Delete all below
GfxShader_DX11::GfxShader_DX11( zcExp::SerialShader_DX11& _SerialItem, InstanceInfoBase& _CommonInstance )
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

GfxShaderParamDefRef GfxShader_DX11::GetShaderParamDef(zcExp::eShaderParamFreq _eShaderParam) const
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
*/

}

