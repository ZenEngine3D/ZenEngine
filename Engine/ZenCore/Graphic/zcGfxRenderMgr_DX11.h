#pragma once

namespace zcGfx 
{
//=================================================================================================
//! @brief		Encapsulate DirectX object needed to query device context about clock rate
//! @details	
//! @todo		Move to own file, with other queries
//=================================================================================================	
class QueryDisjoint_DX11 : public zRefCounted
{
zenClassDeclare(QueryDisjoint_DX11, zRefCounted)
public:
	static zEngineRef<QueryDisjoint_DX11>	Create();								//!< @brief Get a new disjoint query	
	void									Start(ID3D11DeviceContext* _pContext);	//!< @brief Starts clock frequency query
	void									Stop(ID3D11DeviceContext* _pContext);	//!< @brief Stops clock frequency query	
	zU64									GetClockRate();							//!< @brief Gets clock frequency result (0 if invalid)

protected:
											QueryDisjoint_DX11();
	virtual void							ReferenceDeleteCB();		//!< @brief Return object to free list instead of deleting it
	ID3D11Query*							mpDX11Query		= nullptr;	//!< @brief DirectX disjoint query object used to get result
	D3D11_QUERY_DATA_TIMESTAMP_DISJOINT		mDisjointInfo;				//!< @brief Frequency infos returned from query, about the GPU
	zenDbgCode(ID3D11DeviceContext*			mpDeviceContext	= nullptr);	//!< @brief Used for validation that Start/Stop used same context
	zU64									muFrameStop		= 0;		//!< @brief When query was ended (to make sure 1 frame elapsed)
	bool									mbValidResult	= false;	//!< @brief True if we got the result back from GPU

	zListLink								mlstLink;
public:
	typedef zList<QueryDisjoint_DX11, &QueryDisjoint_DX11::mlstLink, false> List;
protected:
	static List								slstQueryCreated;
};

//=================================================================================================
//! @brief		Encapsulate DirectX object needed to query device context about current time
//! @details	
//=================================================================================================	
class QueryTimestamp_DX11 : public zRefCounted
{
zenClassDeclare(QueryTimestamp_DX11, zRefCounted)
public:
	static zEngineRef<QueryTimestamp_DX11>	Create();				//!< @brief Get a new disjoint query and start the timestamp request
	zU64									GetTimestampUSec();		//!< @brief Retrieve the timestamp result (0 if invalid)
		
protected:
											QueryTimestamp_DX11();
	virtual void							ReferenceDeleteCB();	//!< @brief Return object to free list instead of deleting it
	ID3D11Query*							mpDX11Query;			//!< @brief DirectX timestamp query object used to get result
	zEngineRef<QueryDisjoint_DX11>			mrQueryDisjoint;		//!< @brief Reference to Disjoint query to use for getting gpu frequency
	bool									mbValidResult;			//!< @brief True if we got the result back from GPU
	zU64									muTimestamp;			//!< @brief Time on the GPU when query was processed (in microseconds)
	zListLink								mlstLink;
public:
	typedef zList<QueryTimestamp_DX11, &QueryTimestamp_DX11::mlstLink, false> List;
protected:
	static List								slstQueryCreated;
};

//=================================================================================================
//! @brief		zbType::Manager used to control hardware DX11 renderer
//! @details	
//! @todo 1 rename to ManagerRender_DX11
//=================================================================================================	
class ManagerRender_DX11 : public ManagerRender_Base
{
zenClassDeclare(ManagerRender_DX11, ManagerRender_Base)
//---------------------------------------------------------
// Common to all ManagerRender
//---------------------------------------------------------
public:
	virtual void							FrameBegin(zcRes::GfxWindowRef _FrameWindow);
	virtual void							FrameEnd();
	void									Render(ScopedDrawlist& _Drawlist);
	void									NamedEventBegin(const zStringHash32& zName);
	void									NamedEventEnd();
	const zEngineRef<QueryDisjoint_DX11>&	GetQueryDisjoint()const;

//---------------------------------------------------------
// DirectX device infos
//---------------------------------------------------------
public:	
	ID3D11Device*							GetDevice(){return mDX11pDevice;}
	ID3D11DeviceContext*					GetDeviceContext(){return mDX11pContextImmediate;} //! @todo 1 need to remove all access directly to this, use GpuContext
	DXGI_FORMAT								ZenFormatToNative( zenConst::eTextureFormat _eTexFormat )const { return meFormatConversion[_eTexFormat]; }
	
//! @todo urgent clean this up
	void									UnbindTextures(){};
	void									UnbindResources(){};

protected:

	DXGI_FORMAT								meFormatConversion[zenConst::keTexFormat__Count];
	D3D_DRIVER_TYPE							mDX11DriverType			= D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL						mDX11FeatureLevel		= D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*							mDX11pDevice			= nullptr;
	ID3D11DeviceContext*					mDX11pContextImmediate	= nullptr;
	ID3D11InputLayout*						mDX11pEmptyInputLayout	= nullptr;
	ID3DUserDefinedAnnotation*				mDX11pPerf				= nullptr;

	bool									mbDX11ProfilerDetected	= true;
	zEngineRef<QueryDisjoint_DX11>			mrQueryDisjoint;
	zcGfx::GPUContext						mGpuContext[1];	//!< @note Only 1 context for the moment, increase when multihreading is supported
//---------------------------------------------------------
// ManagerBase Section
//---------------------------------------------------------
public:
	virtual	bool			Load			();
	virtual	bool			Unload			();
};

} 

