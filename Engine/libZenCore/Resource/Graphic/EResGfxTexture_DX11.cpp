#include "libZenCore.h"

namespace ERes
{
	GfxTexture2D_DX11::GfxTexture2D_DX11()
	: Resource()
	{
		mInstanceInfo.mpTextureBuffer	= NULL;
		mInstanceInfo.mpTextureView		= NULL;
	}

	GfxTexture2D_DX11::~GfxTexture2D_DX11()
	{
		if( mInstanceInfo.mpTextureBuffer )	
			mInstanceInfo.mpTextureBuffer->Release();
		if( mInstanceInfo.mpTextureView )		
			mInstanceInfo.mpTextureView->Release();
	}

	bool GfxTexture2D_DX11::ResourceInit()
	{
		D3D11_TEXTURE2D_DESC bufferDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
		D3D11_SUBRESOURCE_DATA aInitData[16];
		ZeroMemory( &bufferDesc, sizeof(bufferDesc) );
		ZeroMemory( &viewDesc, sizeof(viewDesc) );
		ZeroMemory( aInitData, sizeof(aInitData) );		
		bool bIsDepth							= EMgr::GfxRender.IsDepth(mInstanceInfo.mSerial.meFormat);
		bufferDesc.Width						= mInstanceInfo.mSerial.maMipData[0].mvDim.x;
		bufferDesc.Height						= mInstanceInfo.mSerial.maMipData[0].mvDim.y;
		bufferDesc.MipLevels					= mInstanceInfo.mSerial.maMipData.Count();
		bufferDesc.ArraySize					= 1;
		bufferDesc.Format						= EMgr::GfxRender.AWFormatToNative(mInstanceInfo.mSerial.meFormat);
		bufferDesc.SampleDesc.Count				= 1;
		bufferDesc.SampleDesc.Quality			= 0;
		bufferDesc.CPUAccessFlags				= 0;
		bufferDesc.MiscFlags					= 0;

		if( mInstanceInfo.mSerial.mCreationFlags.All(zenConst::keTexCreate_RenderTarget) )
		{			
			bufferDesc.Usage					= D3D11_USAGE_DEFAULT;
			bufferDesc.BindFlags				= bIsDepth ? D3D11_BIND_DEPTH_STENCIL : D3D11_BIND_SHADER_RESOURCE|D3D11_BIND_RENDER_TARGET;
		}
		else
		{
			ZENAssertMsg(!bIsDepth, "Only RenderTarget can be of type 'Depth'.");
			bufferDesc.Usage					= D3D11_USAGE_IMMUTABLE;
			bufferDesc.BindFlags				= D3D11_BIND_SHADER_RESOURCE;
		}

		bool bValidInitData(false);
		for(zenUInt mipIdx(0); mipIdx<bufferDesc.MipLevels; ++mipIdx)
		{
			ZENAssert(mipIdx < ZENArrayCount(aInitData));			
			if( mInstanceInfo.mSerial.maMipData[mipIdx].maData.Count() > 0 )
			{
				bValidInitData					= true;
				aInitData[mipIdx].pSysMem		= mInstanceInfo.mSerial.maMipData[mipIdx].maData.First();
				aInitData[mipIdx].SysMemPitch	= EExp::GetTextureBlockInfo(mInstanceInfo.mSerial.meFormat).muSize*mInstanceInfo.mSerial.maMipData[mipIdx].mvDim.x;
			}			
		}

		HRESULT ret = EMgr::GfxRender.DX11GetDevice()->CreateTexture2D( &bufferDesc, bValidInitData ? aInitData : NULL, &mInstanceInfo.mpTextureBuffer );
		if( SUCCEEDED(ret) && !bIsDepth)
		{
			viewDesc.Format						= bufferDesc.Format;
			viewDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
			viewDesc.Texture2D.MostDetailedMip	= 0;
			viewDesc.Texture2D.MipLevels		= bufferDesc.MipLevels;
			ret = EMgr::GfxRender.DX11GetDevice()->CreateShaderResourceView( mInstanceInfo.mpTextureBuffer, &viewDesc, &mInstanceInfo.mpTextureView );
		}

		return SUCCEEDED(ret);
	}
}
