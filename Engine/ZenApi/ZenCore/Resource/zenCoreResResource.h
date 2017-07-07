#pragma once

namespace zen { namespace zenRes {	

	class zExportData : public zRefCounted
	{
	zenClassDeclare(zExportData, zRefCounted)
	public:
							~zExportData();							
		zU32				muSize;			//!< Serialized data size
		zU32				muVersion;		//!< Resource version ID (Based on engine used to export it)
		zTimeStamp			mExportTime;	//!< When resource was exported
		zResID				mResID;			//!< Resource unique ID
	};
			
	//----------------------------------------------
	template< class TResourceClass, class TResourceHAL, zU64 TTypeMask >
	class zResourceConstRef
	{	
	zenClassDeclareNoParent(zResourceConstRef)
	public:	
		enum { keTypeMask = TTypeMask };
		typedef TResourceClass												Class;
		typedef zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>	Const;
	
	public:
									template<class TCopyClass, class TCopyHAL, zU64 TCopyMask >
									zResourceConstRef(const zResourceConstRef<TCopyClass, TCopyHAL, TCopyMask>& _rResource);
									zResourceConstRef(const zResourceConstRef& _rResource);																		
									zResourceConstRef(TResourceClass* _pResource);
									zResourceConstRef(zResID _ResourceID);
									zResourceConstRef();									
		virtual						~zResourceConstRef();

									template<class TCopyClass, class TCopyHAL, zU64 TCopyMask >
		const zResourceConstRef&	operator=(const zResourceConstRef<TCopyClass, TCopyHAL, TCopyMask>& _rResource);
		const zResourceConstRef&	operator=(const zResourceConstRef& _rResource);
		const zResourceConstRef&	operator=(const zResID& _ResourceID);
		const zResourceConstRef&	operator=(TResourceClass* _pResource);

		bool						operator==( const zResourceConstRef& _rCmpResource )const;
		bool						operator!=( const zResourceConstRef& _rCmpResource )const;
		const TResourceClass*		operator->()const;		//!< Return pointer to resource
		const TResourceClass*		Get()const;				//!< Return pointer to resource
		const TResourceHAL*			HAL()const;				//!< Return pointer to resource HAL
		zResID						GetResID()const;
		bool						IsValid()const{ return mpResource != nullptr; }
	protected:
		void						SetResource(zExportData* _pData);
		TResourceClass*				mpResource = nullptr;
		friend class				zResourceConstRef;
	};

	template< class TResourceClass, class TResourceHAL, zU64 TTypeMask >
	class zResourceRef : public zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>
	{	
	zenClassDeclare(zResourceRef, zResourceConstRef)
	public:
		TResourceClass*				operator->()const;		//!< Return a pointer to resource
		TResourceClass*				Get()const;				//!< Return a pointer to resource
		TResourceHAL*				HAL()const;				//!< Return pointer to resource HAL
		using Super::operator=;
		using Super::Super;
	};
}} // namespace zen, zenRes

#include "zenCoreResResource.inl"

// Forward Declarations
namespace zcExp
{
	class ExportGfxShader;
};

namespace zcRes
{	
	//==============================================================================================
	//! @detail	Add support for a resource, creating an alias for the HAL class  
	//!			and adding ResourceRef object associated.
	//!			Exemple :	AddResourceSupport(GfxBuffer) 
	//!						-Adds alias 'GfxBuffer_HAL' for class GfxBuffer_(RENDERER)
	//!						-Adds alias 'GfxBufferRef' for RefClass supporting GfxBuffer object 
	//!							with type 'keResType_Buffer'
	//==============================================================================================
	#define AddResourceSupport(_ResClassName_)													\
		using _ResClassName_##_HAL	= class zenDefineStich3(_ResClassName_, _, ZEN_RENDERER);	\
		using _ResClassName_##Ref	= zenRes::zResourceRef<class _ResClassName_, _ResClassName_##_HAL, zEnumMask<zU64>(keResType_##_ResClassName_)>;
	
	AddResourceSupport(GfxRenderPass);
	AddResourceSupport(GfxMeshStrip);
	AddResourceSupport(GfxMesh);
	AddResourceSupport(GfxShaderBinding);
	AddResourceSupport(GfxCBufferDefinition);
	AddResourceSupport(GfxCBuffer);
	AddResourceSupport(GfxBuffer);
	AddResourceSupport(GfxShaderPixel);
	AddResourceSupport(GfxShaderVertex);
	AddResourceSupport(GfxStateSampler);
	AddResourceSupport(GfxStateBlend);
	AddResourceSupport(GfxStateDepthStencil);
	AddResourceSupport(GfxStateRaster);
	AddResourceSupport(GfxView);
	AddResourceSupport(GfxTarget2D);
	AddResourceSupport(GfxTexture2D);
	AddResourceSupport(GfxIndex);
	AddResourceSupport(GfxWindow);

	// Grouped Resources Declaration (more than one type of resource supported by smartpointer)
	using ResourceAnyRef		= zenRes::zResourceRef<	class zenRes::zExportData,		class zenRes::zExportData,		
									0xFFFFFFFFFFFFFFFF>;
	
	using GfxShaderAnyRef		= zenRes::zResourceRef<	class zcExp::ExportGfxShader,	class zcExp::ExportGfxShader,
									zEnumMask<zU64>(keResType_GfxShaderPixel, keResType_GfxShaderVertex) >;
	
	using GfxShaderResourceRef	= zenRes::zResourceRef<	class zenRes::zExportData,		class zenRes::zExportData,
									zEnumMask<zU64>(keResType_GfxStateSampler, keResType_GfxTexture2D, keResType_GfxBuffer, keResType_GfxCBuffer) >;
};
