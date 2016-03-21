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
		RenderContext();
		zVec4U16						mvScreenScissor		= zVec4U16(0, 0, 0, 0);
		bool							mbScreenScissorOn	= false;
		zcRes::GfxRenderPassRef			mrRenderpass		= nullptr;
		zcRes::GfxStateBlendRef			mrStateBlend		= nullptr;
		zcRes::GfxStateDepthStencilRef	mrStateDepthStencil	= nullptr;
		zcRes::GfxStateRasterizerRef	mrStateRaster		= nullptr;
		zcRes::GfxViewRef				mrStateView			= nullptr;
		
		zcRes::GfxInputStreamRef		mrInputStream		= nullptr;
		D3D11_PRIMITIVE_TOPOLOGY		mePrimitiveType		= D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
		zcRes::GfxShaderVertexRef		mrShaderVertex		= nullptr;
		zcRes::GfxShaderPixelRef		mrShaderPixel		= nullptr;
		
		zcRes::GfxSamplerRef			maCurrentSampler[zenConst::keShaderStage__Count][zcExp::kuDX11_TexturePerStageMax];
		zcRes::GfxTexture2dRef			maCurrentTexture[zenConst::keShaderStage__Count][zcExp::kuDX11_TexturePerStageMax];
		zU16							muPerStageTextureCount[zenConst::keShaderStage__Count];
	};

	virtual void					FrameBegin(zcRes::GfxWindowRef _FrameWindow);
	virtual void					FrameEnd();
	void							Render(zArrayDynamic<zEngineRef<zcGfx::Command>>& _aDrawcalls);
	void							NamedEventBegin(const zStringHash32& zName);
	void							NamedEventEnd();
	
//---------------------------------------------------------
// DirectX device infos
//---------------------------------------------------------
public:	
	ID3D11Device*					DX11GetDevice(){return mDX11pDevice;}
	ID3D11DeviceContext*			DX11GetDeviceContext(){return mDX11pContextImmediate;}
	DXGI_FORMAT						ZenFormatToNative( zenConst::eTextureFormat _eTexFormat )const { return meFormatConversion[_eTexFormat]; }
	void							UnbindTextures();
	void							UnbindResources();
protected:
	ZENInline void					UpdateGPUState(const zEngineRef<zcGfx::Command>& _rDrawcall, RenderContext& _Context);
	ZENInline void					UpdateShaderState(const zcGfx::CommandDraw& _Drawcall, RenderContext& _Context);

	zEngineRef<zcGfx::Command>		mrPreviousDrawcall;	
	DXGI_FORMAT						meFormatConversion[zenConst::keTexFormat__Count];
	D3D_DRIVER_TYPE					mDX11DriverType			= D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL				mDX11FeatureLevel		= D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*					mDX11pDevice			= nullptr;
	ID3D11DeviceContext*			mDX11pContextImmediate	= nullptr;	
	ID3DUserDefinedAnnotation*		mDX11pPerf				= nullptr;
	bool							mbTextureUnbound		= false;
	bool							mbResourceUnbound		= false;
//---------------------------------------------------------
// ManagerBase Section
//---------------------------------------------------------
public:
	virtual	bool			Load			();
	virtual	bool			Unload			();
};

}
#endif
