#pragma once
#ifndef __zCore_Res_Gfx_State_DX11_h__
#define __zCore_Res_Gfx_State_DX11_h__

namespace zcRes
{
	class GfxSamplerProxy_DX11 : public zRefCountedAutoDel
	{
	ZENClassDeclare(GfxSamplerProxy_DX11, zRefCountedAutoDel)
	public:
												GfxSamplerProxy_DX11();
		virtual									~GfxSamplerProxy_DX11();												
		bool									Initialize(class GfxSampler& _Owner);
		ID3D11SamplerState*						mpSamplerState;
		ZENDbgCode(class GfxSampler*			mpOwner);
	};

	class GfxStateBlendProxy_DX11 : public zRefCountedAutoDel
	{
	ZENClassDeclare(GfxStateBlendProxy_DX11, zRefCountedAutoDel)
	public:
												GfxStateBlendProxy_DX11();
		virtual									~GfxStateBlendProxy_DX11();												
		bool									Initialize(class GfxStateBlend& _Owner);
		ID3D11BlendState*						mpBlendState;
		UINT									muSampleMask;
		float									mafBlendFactor[4]; 
		ZENDbgCode(class GfxStateBlend*			mpOwner);
	};

	class GfxStateDepthStencilProxy_DX11 : public zRefCountedAutoDel
	{
	ZENClassDeclare(GfxStateDepthStencilProxy_DX11, zRefCountedAutoDel)
	public:
												GfxStateDepthStencilProxy_DX11();
		virtual									~GfxStateDepthStencilProxy_DX11();												
		bool									Initialize(class GfxStateDepthStencil& _Owner);
		ID3D11DepthStencilState*				mpDepthStencilState;
		UINT									muStencilValue;
		ZENDbgCode(class GfxStateDepthStencil*	mpOwner);
	};

	class GfxStateRasterizerProxy_DX11 : public zRefCountedAutoDel
	{
	ZENClassDeclare(GfxStateRasterizerProxy_DX11, zRefCountedAutoDel)
	public:
												GfxStateRasterizerProxy_DX11();
		virtual									~GfxStateRasterizerProxy_DX11();												
		bool									Initialize(class GfxStateRasterizer& _Owner);
		ID3D11RasterizerState*					mpRasterizerState;
		ZENDbgCode(class GfxStateRasterizer*	mpOwner);
	};

}

#endif
