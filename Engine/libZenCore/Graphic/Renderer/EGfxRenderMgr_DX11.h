#pragma once
#ifndef __LibGraphic_Renderer_Manager_DX11_h__
#define __LibGraphic_Renderer_Manager_DX11_h__

namespace EGfx
{

//=================================================================================================
//! @brief		zbType::Manager used to control hardware DX11 renderer
//! @details	
//=================================================================================================	
class ManagerRender : public ManagerRender_Base
{
AWClassDeclare(ManagerRender, ManagerRender_Base)
//---------------------------------------------------------
// Common to all ManagerRender
//---------------------------------------------------------
public:
							ManagerRender();
	virtual void			FrameBegin(ERes::GfxWindowRef _FrameWindow);
	virtual void			FrameEnd();

	void					Render(ERes::GfxMeshRef _rMesh);
	void					Render(ERes::GfxMeshStripRef _rMeshStrip);
	
//---------------------------------------------------------
// DirectX device infos
//---------------------------------------------------------
public:
	
	ID3D11Device*			DX11GetDevice(){return mDX11pDevice;}
	ID3D11DeviceContext*	DX11GetDeviceContext(){return mDX11pContextImmediate;}
	DXGI_FORMAT				AWFormatToNative( awconst::eTextureFormat _eTexFormat )const { return meFormatConversion[_eTexFormat]; }
	void					UnbindTextures();
protected:
	awU16					muPerStageTextureCount[EExp::keShaderStage__Count];
	DXGI_FORMAT				meFormatConversion[awconst::keTexFormat__Count];
	D3D_DRIVER_TYPE         mDX11DriverType;
	D3D_FEATURE_LEVEL       mDX11FeatureLevel;
	ID3D11Device*           mDX11pDevice;
	ID3D11DeviceContext*    mDX11pContextImmediate;
	ERes::GfxSamplerRef		maCurrentSampler[EExp::keShaderStage__Count][EExp::kuDX11_TexturePerStageMax];
	ERes::GfxTexture2DRef	maCurrentTexture[EExp::keShaderStage__Count][EExp::kuDX11_TexturePerStageMax];
	
//---------------------------------------------------------
// ManagerBase Section
//---------------------------------------------------------
public:
	virtual	bool			Load			();
	virtual	bool			Unload			();
};

}
#endif
