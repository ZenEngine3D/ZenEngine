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
									ManagerRender();
	virtual void					FrameBegin(zcRes::GfxWindowRef _FrameWindow);
	virtual void					FrameEnd();

	void							Render(zcRes::GfxMeshProxyRef _rMesh);
	void							Render(zcRes::GfxMeshStripProxyRef _rMeshStrip);
	
//---------------------------------------------------------
// DirectX device infos
//---------------------------------------------------------
public:	
	ID3D11Device*					DX11GetDevice(){return mDX11pDevice;}
	ID3D11DeviceContext*			DX11GetDeviceContext(){return mDX11pContextImmediate;}
	DXGI_FORMAT						ZenFormatToNative( zenConst::eTextureFormat _eTexFormat )const { return meFormatConversion[_eTexFormat]; }
	void							UnbindTextures();

protected:
	zU16							muPerStageTextureCount[zenConst::keShaderStage__Count];
	DXGI_FORMAT						meFormatConversion[zenConst::keTexFormat__Count];
	D3D_DRIVER_TYPE					mDX11DriverType;
	D3D_FEATURE_LEVEL				mDX11FeatureLevel;
	ID3D11Device*					mDX11pDevice;
	ID3D11DeviceContext*			mDX11pContextImmediate;
	zcRes::GfxSamplerProxyRef		maCurrentSampler[zenConst::keShaderStage__Count][zcExp::kuDX11_TexturePerStageMax];
	zcRes::GfxTexture2dProxyRef		maCurrentTexture[zenConst::keShaderStage__Count][zcExp::kuDX11_TexturePerStageMax];
	
//---------------------------------------------------------
// ManagerBase Section
//---------------------------------------------------------
public:
	virtual	bool			Load			();
	virtual	bool			Unload			();
};

}
#endif
