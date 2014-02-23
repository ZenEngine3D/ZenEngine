#pragma once
#ifndef __LibGraphic_State_Manager_DX11_h__
#define __LibGraphic_State_Manager_DX11_h__ 

namespace EGfx
{

class ManagerState_DX11 : public ManagerState_Base
{
AWClassDeclare(ManagerState_DX11, ManagerState_Base)
	//---------------------------------------------------------
	// Common to all ManagerState
	//---------------------------------------------------------
public:
							ManagerState_DX11	();
	void					SetDepthStencil		(const ConfigDepthStencil& aDepthStencil);
	void					SetBlend			(const ConfigBlend& aBlend, awUInt aRenderTarget);
	void					SetView				(ERes::GfxViewRef _rView);
	void					PipelineReset		();
	void					PipelineUpdate		();
	
	//---------------------------------------------------------
	// DirectX device infos
	//---------------------------------------------------------
protected:
	struct BlendingToDX11{
		D3D11_BLEND Src; 
		D3D11_BLEND Dst; 
		D3D11_BLEND_OP Op;
		void Set(D3D11_BLEND aSrc, D3D11_BLEND aDst, D3D11_BLEND_OP aOp){Src=aSrc; Dst=aDst; Op=aOp;}
	};	
	enum eConstants
	{
		kuMaxSampler=16,
		kuMaxRenderTarget=4,
		kuStackCount=16,
	};

	ERes::GfxViewRef			mrView;

	bool						mbUsingMRTs;
	ConfigDepthStencil			mConfigDepthStencil;
	ConfigBlend					mConfigBlend[kuMaxRenderTarget];
	awU8							mBlendWriteMaskToDX11[16];	
	BlendingToDX11				mBlendToDX11[ConfigBlend::keBlend__Count];
	D3D11_COMPARISON_FUNC		mDepthTestToDX11[ConfigDepthStencil::keDepth__Count];
	
	ID3D11BlendState*			mpStateBlend;
	ID3D11DepthStencilState*	mpStateDepthStencil;

	awUInt						muStackCurrent;			//!< Implement a stock Push/Pop
	awUInt						mbUpdatedView			: 1; //!< Track render target change
	awUInt						mbUpdatedBlend			: 1; //!< Track Blending state update of RenderTargets
	awUInt						mbUpdatedDepthStencil	: 1; //!< Track Depth/Stencil state update
	//	awU32							muUpdateSampler;		//!< Track Texture Sampler state update
	//---------------------------------------------------------
	// ManagerBase Section
	//---------------------------------------------------------
public:
	virtual	bool			Load			();
	virtual	bool			Unload			();	
};

typedef class ManagerState_DX11 ManagerState;

}

#endif
