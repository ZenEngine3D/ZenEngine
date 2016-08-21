#pragma once
#ifndef __zCore_Gfx_Renderer_Manager_DX11_h__
#define __zCore_Gfx_Renderer_Manager_DX11_h__

namespace zcGfx
{
//=================================================================================================
//! @brief		Encapsulate DirectX object needed to query device context about clock rate
//! @details	
//! @todo		Move to own file, with other queries
//=================================================================================================	
class DX11QueryDisjoint : public zRefCounted
{
public:
	static zEngineRef<DX11QueryDisjoint>	Create();					//!< @brief Get a new disjoint query	
	void									Start();					//!< @brief Starts clock frequency query
	void									Stop();						//!< @brief Stops clock frequency query	
	zU64									GetClockRate();				//!< @brief Gets clock frequency result (0 if invalid)

protected:
											DX11QueryDisjoint();
	virtual void							ReferenceDeleteCB();		//!< @brief Return object to free list instead of deleting it
	ID3D11Query*							mpDX11Query;				//!< @brief DirectX disjoint query object used to get result
	D3D11_QUERY_DATA_TIMESTAMP_DISJOINT		mDisjointInfo;				//!< @brief Frequency infos returned from query, about the GPU
	zU64									muFrameStop;				//!< @brief When query was ended (to make sure 1 frame elapsed)
	bool									mbValidResult;				//!< @brief True if we got the result back from GPU
	zListLink								mlstLink;
public:
	typedef zList<DX11QueryDisjoint, &DX11QueryDisjoint::mlstLink, false> List;
protected:
	static List								slstQueryCreated;
};

//=================================================================================================
//! @brief		Encapsulate DirectX object needed to query device context about current time
//! @details	
//=================================================================================================	
class DX11QueryTimestamp : public zRefCounted
{
public:
	static zEngineRef<DX11QueryTimestamp>	Create();				//!< @brief Get a new disjoint query and start the timestamp request
	zU64									GetTimestampUSec();		//!< @brief Retrieve the timestamp result (0 if invalid)
		
protected:
											DX11QueryTimestamp();
	virtual void							ReferenceDeleteCB();	//!< @brief Return object to free list instead of deleting it
	ID3D11Query*							mpDX11Query;			//!< @brief DirectX timestamp query object used to get result
	zEngineRef<DX11QueryDisjoint>			mrQueryDisjoint;		//!< @brief Reference to Disjoint query to use for getting gpu frequency
	bool									mbValidResult;			//!< @brief True if we got the result back from GPU
	zU64									muTimestamp;			//!< @brief Time on the GPU when query was processed (in microseconds)
	zListLink								mlstLink;
public:
	typedef zList<DX11QueryTimestamp, &DX11QueryTimestamp::mlstLink, false> List;
protected:
	static List								slstQueryCreated;
};

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
		zcRes::GfxRenderPassRef			mrRenderpass		= nullptr;
		zcRes::GfxViewRef				mrStateView			= nullptr;
		zcRes::GfxStateBlendRef			mrStateBlend		= nullptr;
		zcRes::GfxStateDepthStencilRef	mrStateDepthStencil	= nullptr;
		zcRes::GfxStateRasterRef		mrStateRaster		= nullptr;
		zcRes::GfxInputStreamRef		mrInputStream		= nullptr;		
		zcRes::GfxShaderAnyRef			marShader[zenConst::keShaderStage__Count];				
		zcRes::GfxSamplerRef			maCurrentSampler[zenConst::keShaderStage__Count][zcExp::kuDX11_TexturePerStageMax];
		zcRes::GfxTexture2dRef			maCurrentTexture[zenConst::keShaderStage__Count][zcExp::kuDX11_TexturePerStageMax];
		zU16							muPerStageTextureCount[zenConst::keShaderStage__Count];
		D3D11_PRIMITIVE_TOPOLOGY		mePrimitiveType		= D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
		zVec4U16						mvScreenScissor		= zVec4U16(0, 0, 0, 0);
		bool							mbScreenScissorOn	= false;
	};

	virtual void								FrameBegin(zcRes::GfxWindowRef _FrameWindow);
	virtual void								FrameEnd();
	void										Render(zArrayDynamic<zEngineRef<zcGfx::Command>>& _aDrawcalls);
	void										NamedEventBegin(const zStringHash32& zName);
	void										NamedEventEnd();
	const zEngineRef<DX11QueryDisjoint>&		GetQueryDisjoint()const;

//---------------------------------------------------------
// DirectX device infos
//---------------------------------------------------------
public:	
	ID3D11Device*								DX11GetDevice(){return mDX11pDevice;}
	ID3D11DeviceContext*						DX11GetDeviceContext(){return mDX11pContextImmediate;}
	DXGI_FORMAT									ZenFormatToNative( zenConst::eTextureFormat _eTexFormat )const { return meFormatConversion[_eTexFormat]; }
	void										UnbindTextures();
	void										UnbindResources();
protected:
	ZENInline void								UpdateGPUState(const zEngineRef<zcGfx::Command>& _rDrawcall, RenderContext& _Context);
	ZENInline void								UpdateShaderState(const zcGfx::CommandDraw& _Drawcall, RenderContext& _Context);

	zEngineRef<zcGfx::Command>					mrPreviousDrawcall;	
	DXGI_FORMAT									meFormatConversion[zenConst::keTexFormat__Count];
	D3D_DRIVER_TYPE								mDX11DriverType			= D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL							mDX11FeatureLevel		= D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*								mDX11pDevice			= nullptr;
	ID3D11DeviceContext*						mDX11pContextImmediate	= nullptr;	
	ID3DUserDefinedAnnotation*					mDX11pPerf				= nullptr;
	bool										mbTextureUnbound		= false;
	bool										mbResourceUnbound		= false;
	bool										mbDX11ProfilerDetected	= true;
	zEngineRef<DX11QueryDisjoint>				mrQueryDisjoint;
//---------------------------------------------------------
// ManagerBase Section
//---------------------------------------------------------
public:
	virtual	bool			Load			();
	virtual	bool			Unload			();
};

}
#endif
