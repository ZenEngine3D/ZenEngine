#pragma once
#ifndef __LibResource_Graphics_Texture_DX11_h__
#define __LibResource_Graphics_Texture_DX11_h__

namespace ERes
{
	class GfxTexture2D_DX11 : public Resource
	{
		struct InstanceInfo : public EExp::SerialGfxTexture2D_DX11
		{
			ID3D11Texture2D*			mpTextureBuffer;
			ID3D11ShaderResourceView*	mpTextureView;
		};
	AWResourceDeclare(GfxTexture2D_DX11, InstanceInfo, awconst::keResType_GfxTexture2D)
	public:
		virtual						~GfxTexture2D_DX11();
		virtual bool				ResourceInit();

	//---------------------------------------------------------
	// Common to all Texture2D
	//---------------------------------------------------------
		inline const awVec2U16&		GetDim(){ return (Get().mSerial.maMipData.Count() > 0 ) ? Get().mSerial.maMipData[0].mvDim : awVec2U16::Zero; }
	};
}

#endif
