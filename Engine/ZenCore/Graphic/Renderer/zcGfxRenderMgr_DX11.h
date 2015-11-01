#pragma once
#ifndef __zCore_Gfx_Renderer_Manager_DX11_h__
#define __zCore_Gfx_Renderer_Manager_DX11_h__

namespace zcGfx
{

//=================================================================================================
//! @brief		zbType::Manager used to control hardware DX11 renderer
//! @details	
//=================================================================================================	
class ManagerRender : public ManagerRender_Base
{
ZENClassDeclare(ManagerRender, ManagerRender_Base)
//---------------------------------------------------------
// Common to all ManagerRender
//---------------------------------------------------------
public:
	struct RenderContext
	{
		RenderContext()
		{
			zenMem::Zero(maCurrentSampler, sizeof(maCurrentSampler));
			zenMem::Zero(maCurrentTexture, sizeof(maCurrentTexture));
			zenMem::Zero(muPerStageTextureCount, sizeof(muPerStageTextureCount));
		}
		zcRes::GfxRenderPassProxyRef		mrRenderpass		= nullptr;
		zcRes::GfxStateBlendProxyRef		mrStateBlend		= nullptr;
		zcRes::GfxStateDepthStencilProxyRef	mrStateDepthStencil	= nullptr;
		zcRes::GfxStateRasterizerProxyRef	mrStateRaster		= nullptr;
		zcRes::GfxViewProxyRef				mrStateView			= nullptr;
		
		zcRes::GfxInputStreamProxyRef		mrInputStream		= nullptr;
		D3D11_PRIMITIVE_TOPOLOGY			mePrimitiveType		= D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
		zcRes::GfxShaderVertexProxyRef		mrShaderVertex		= nullptr;
		zcRes::GfxShaderPixelProxyRef		mrShaderPixel		= nullptr;

		zcRes::GfxSamplerProxyRef			maCurrentSampler[zenConst::keShaderStage__Count][zcExp::kuDX11_TexturePerStageMax];
		zcRes::GfxTexture2dProxyRef			maCurrentTexture[zenConst::keShaderStage__Count][zcExp::kuDX11_TexturePerStageMax];
		zU16								muPerStageTextureCount[zenConst::keShaderStage__Count];
	};

									ManagerRender();
	virtual void					FrameBegin(zcRes::GfxWindowRef _FrameWindow);
	virtual void					FrameEnd();
	
	void							Render(zArrayDynamic<zenRes::zGfxDrawcall>& _aDrawcalls);
	void							Render(const zenRes::zGfxDrawcall& _rDrawcall);
//---------------------------------------------------------
// DirectX device infos
//---------------------------------------------------------
public:	
	ID3D11Device*					DX11GetDevice(){return mDX11pDevice;}
	ID3D11DeviceContext*			DX11GetDeviceContext(){return mDX11pContextImmediate;}
	DXGI_FORMAT						ZenFormatToNative( zenConst::eTextureFormat _eTexFormat )const { return meFormatConversion[_eTexFormat]; }
	void							UnbindTextures();

protected:
	ZENInline void					UpdateGPUState(const zenRes::zGfxDrawcall& _rDrawcall, RenderContext& _Context);
	ZENInline void					UpdateShaderState(const zenRes::zGfxDrawcall& _rDrawcall, RenderContext& _Context);

	zenRes::zGfxDrawcall			mrPreviousDrawcall;	
	DXGI_FORMAT						meFormatConversion[zenConst::keTexFormat__Count];
	D3D_DRIVER_TYPE					mDX11DriverType;
	D3D_FEATURE_LEVEL				mDX11FeatureLevel;
	ID3D11Device*					mDX11pDevice;
	ID3D11DeviceContext*			mDX11pContextImmediate;	
	bool							mbTextureUnbound = false;
	
//---------------------------------------------------------
// ManagerBase Section
//---------------------------------------------------------
public:
	virtual	bool			Load			();
	virtual	bool			Unload			();
};

}
#endif
