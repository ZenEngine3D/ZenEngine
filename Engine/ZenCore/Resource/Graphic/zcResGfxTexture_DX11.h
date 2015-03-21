#pragma once
#ifndef __zCore_Res_Gfx_Texture_DX11_h__
#define __zCore_Res_Gfx_Texture_DX11_h__

namespace zcRes
{
	class GfxTexture2D_DX11 : public Resource
	{
		struct InstanceInfo : public zcExp::SerialGfxTexture2D_DX11
		{
			ID3D11Texture2D*			mpTextureBuffer;
			ID3D11ShaderResourceView*	mpTextureView;
		};
	ZENResourceDeclare(GfxTexture2D_DX11, InstanceInfo, zenConst::keResType_GfxTexture2D)
	public:
		virtual						~GfxTexture2D_DX11();
		virtual bool				ResourceInit();

	//---------------------------------------------------------
	// Common to all Texture2D
	//---------------------------------------------------------
		ZENInline const zVec2U16&		GetDim(){ return (Get().mSerial.maMipData.Count() > 0 ) ? Get().mSerial.maMipData[0].mvDim : zVec2U16::Zero; }
	};
}

#endif
