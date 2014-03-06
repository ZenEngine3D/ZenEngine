#pragma once
#ifndef __LibResource_Graphics_GfxSampler_DX11_h__
#define __LibResource_Graphics_GfxSampler_DX11_h__

namespace ERes
{
	class GfxSampler_DX11 : public Resource
	{
	struct InstanceInfo : public EExp::SerialGfxSampler_DX11
	{
		ID3D11SamplerState*		mpSamplerState;
	};
	AWResourceDeclare(GfxSampler_DX11, InstanceInfo, zenConst::keResType_GfxSampler)
	public:
		virtual					~GfxSampler_DX11();
		virtual bool			ResourceInit();		
	};

	class GfxBlend_DX11 : public Resource
	{
		struct InstanceInfo : public EExp::SerialGfxBlend_DX11
		{
			ID3D11BlendState*		mpBlendState;
		};
		AWResourceDeclare(GfxBlend_DX11, InstanceInfo, zenConst::keResType_GfxBlend)
	public:
		virtual					~GfxBlend_DX11();
		virtual bool			ResourceInit();		
	};

	class GfxDepthStencil_DX11 : public Resource
	{
		struct InstanceInfo : public EExp::SerialGfxDepthStencil_DX11
		{
			ID3D11DepthStencilState*		mpDepthStencilState;
		};
		AWResourceDeclare(GfxDepthStencil_DX11, InstanceInfo, zenConst::keResType_GfxDepthStencil)
	public:
		virtual					~GfxDepthStencil_DX11();
		virtual bool			ResourceInit();		
	};

	class GfxRasterizer_DX11 : public Resource
	{
		struct InstanceInfo : public EExp::SerialGfxRasterizer_DX11
		{
			ID3D11RasterizerState*		mpRasterizerState;
		};
		AWResourceDeclare(GfxRasterizer_DX11, InstanceInfo, zenConst::keResType_GfxRasterizer)
	public:
		virtual					~GfxRasterizer_DX11();
		virtual bool			ResourceInit();		
	};
}

#endif
