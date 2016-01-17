#pragma once
#ifndef __zCore_Res_Resource_h__
#define __zCore_Res_Resource_h__

//! @todo Clean : move this to proper location and implement it
ZENInline bool zenIsResourceCreationThread()
{
	return true;
}

namespace zcRes
{	
	//=============================================================================================
	//! @class	Base class for all resources that can be created and used
	//=============================================================================================
	class Resource: public zenRes::zResource
	{
	ZENClassDeclare(Resource, zenRes::zResource)
	public:									
		virtual								~Resource();	
		bool								Initialize();		

	protected:					
		virtual bool						ResourceInit();		//!< @todo cleanup rename this.  Initialize and Resourcinit confusing
		virtual void						StripResData(){}	//!< @Brief Child class should override this if some ResData can be stripped out for runtime, after proxy init
											Resource();
		zListLink							mlnkList;
	public:
		typedef zList<Resource, &Resource::mlnkList> TypeList;
	};
	
	//=============================================================================================
	//! @class	When a platform doesn't support a resource type, we use this in the typedef.
	//!			The code will compile, but gives an error if someone try to use it
	//=============================================================================================
	class UnsupportedResource : public Resource
	{
	ZENClassDeclare(UnsupportedResource, Resource);
	public:
											UnsupportedResource();
	};

	//=============================================================================================
	//! @class	Templated specialized version of Resource
	//!			Implement basic common functionalities like creation, Proxy declaration and access...
	//=============================================================================================
	template<class TClassResource, class TClassResData, class TClassProxy, class TClassExporter>
	class TResource : public Resource
	{
	ZENClassDeclare(TResource, Resource)
	public:
		typedef TClassProxy					ClassProxy;
		typedef TClassExporter				ClassExporter;
		typedef TClassResData				ClassResData;
		typedef TClassResource				ClassResource;
		typedef zEngineRef<TClassProxy>		ProxyRef;
		typedef zEngineRef<TClassResData>	ResDataRef;
		typedef zEngineRef<TClassResource>	ResourceRef;
				
	protected:
		//! @todo cleanup
		virtual bool ResourceInit()
		{
			ZENAssert( !mrProxy.IsValid() );
			ZENAssert( zenIsResourceCreationThread() );
			if( Super::ResourceInit() )
			{				
				muCreatedIndex = suCreatedCount++;
				static zenMem::zAllocatorPool sMemPool("Pool TResource Proxy", sizeof(ClassProxy), 32, 32 );
				mrProxy = zenNew(&sMemPool) ClassProxy();
				if( !mrProxy->Initialize( *static_cast<ClassResource*>(this)) )
					mrProxy = nullptr; //! @todo assign a default value

				StripResData();				
			}
			return mrProxy.IsValid();
		}
		
	public:			

		//! @Brief Child class should override this if some ResData can be stripped out for runtime, after proxy init
		virtual void StripResData()
		{
		}

		ZENInline const ResDataRef& GetResData()const									
		{ 
			return mrResData;
		}

		ZENInline const ProxyRef& GetProxy() const
		{
			return mrProxy;
		}

		static ResourceRef RuntimeExport(zcExp::ExportInfoBase& _ExportInfo)
		{ 	
			static zenMem::zAllocatorPool sMemPool("Pool TResource SeriaData", sizeof(ClassResData), 32, 32 );			
			ClassResData* pData = zenNew(&sMemPool) ClassResData();

			#if 0 //SF Temp code to help find memory leak of export data..
			if( _ExportInfo.mExportResID.GetType() == zenConst::keResType_GfxShaderParam )
			{				
				printf(""); 
			}
			#endif
			ResDataRef rResData = pData;
			ClassExporter Exporter(rResData);
			Exporter.Export( _ExportInfo );
			if( _ExportInfo.IsSuccess() )
				return RuntimeCreate(rResData);
			
			return nullptr;				
		}
		
		//! @todo clean Move gfx state creation to use this ?
		static ResourceRef RuntimeCreate(const ResDataRef& _rResData)
		{ 						
			ResourceRef rNewResource	= zenNewDefault ClassResource;
			rNewResource->mrResData		= _rResData;
			rNewResource->mResID		= _rResData->mResID;
			if(rNewResource->Initialize() )
			{
				if(rNewResource->mrResData.IsValid() && !zcDepot::ResourceData.IsValid(rNewResource->mResID) )
				{
					zenConst::eResType eType				= rNewResource->mResID.GetType();
					rNewResource->mrResData->muVersion		= zcDepot::ResourceData.GetEngineVersion(eType);
					rNewResource->mrResData->mExportTime	= zenSys::GetTimeStamp();
					zcDepot::ResourceData.SetItem(rNewResource->mrResData.Get());
				}
				return rNewResource;
			}
			
			return nullptr;			
		}
		
	protected:
		TResource(){}
		ProxyRef	mrProxy			= nullptr;
		ResDataRef	mrResData		= nullptr;	//!< @todo clean make this constref, and use pointer from resource and proxy
		zU32		muCreatedIndex	= 0;		//!< Unique index assign per resource type, based on how many resource were created before
		static zU32	suCreatedCount;				//!< How many resource of this type was created before
	};
	

	//=============================================================================================
	//! @class	Templated specialized version of ResourceAnyRef, that does ResourceType check
	//!			before use, making sure we're not mis-casting to wrong ResourceType
	//=============================================================================================
	template<class TResourceClass, class TReferenceClass>
	class ResourceRef : public TReferenceClass
	{
	ZENClassDeclare(ResourceRef, TReferenceClass)
	public:	
		typedef TResourceClass Resource;
		ZENInline								ResourceRef();
		ZENInline								ResourceRef(zenRes::zResource* _pResource);
		ZENInline								ResourceRef(zResID _ResourceID);
		ZENInline								ResourceRef(const TReferenceClass& _Copy);
		ZENInline TResourceClass*				operator->();		//!< Return a pointer to resource
		ZENInline const TResourceClass*			operator->()const;	//!< Return a const pointer to resource
		ZENInline TResourceClass*				Get();				//!< Return a pointer to resource
		ZENInline const TResourceClass*			Get()const;			//!< Return a const pointer to resource
	};
}

#include "zcResResource.inl"
#include ZENHeaderRenderer( zcResResource )	// Each renderer resources typedef to generic name used in-engine
#include ZENHeaderPlatform( zcResResource )	// Each platform resources typedef to generic name used in-engine

namespace zcRes
{		
	typedef ResourceRef<class GfxShaderVertex,		zenRes::zGfxShaderVertex>		GfxShaderVertexRef;
	typedef ResourceRef<class GfxShaderPixel,		zenRes::zGfxShaderPixel>		GfxShaderPixelRef;
	typedef ResourceRef<class GfxIndex,				zenRes::zGfxIndex>				GfxIndexRef;
	typedef ResourceRef<class GfxVertex,			zenRes::zGfxVertex>				GfxVertexRef;
	typedef ResourceRef<class GfxTexture2d,			zenRes::zGfxTexture2d>			GfxTexture2dRef;
	typedef ResourceRef<class GfxRenderTarget,		zenRes::zGfxRenderTarget>		GfxRenderTargetRef;
	typedef ResourceRef<class GfxView,				zenRes::zGfxView>				GfxViewRef;
	typedef ResourceRef<class GfxWindow,			zenRes::zGfxWindow>				GfxWindowRef;
	typedef ResourceRef<class GfxRenderPass,		zenRes::zGfxRenderPass>			GfxRenderPassRef;
	typedef ResourceRef<class GfxSampler,			zenRes::zGfxSampler>			GfxSamplerRef;
	typedef ResourceRef<class GfxStateBlend,		zenRes::zGfxStateBlend>			GfxStateBlendRef;
	typedef ResourceRef<class GfxStateDepthStencil,	zenRes::zGfxStateDepthStencil>	GfxStateDepthStencilRef;
	typedef ResourceRef<class GfxStateRasterizer,	zenRes::zGfxStateRasterizer>	GfxStateRasterizerRef;
	typedef ResourceRef<class GfxInputStream,		zenRes::zGfxInputStream>		GfxInputStreamRef;
	typedef ResourceRef<class GfxInputSignature,	zenRes::zGfxInputSignature>		GfxInputSignatureRef;
	typedef ResourceRef<class GfxMeshStrip,			zenRes::zGfxMeshStrip>			GfxMeshStripRef;
	typedef ResourceRef<class GfxMesh,				zenRes::zGfxMesh>				GfxMeshRef;
	typedef ResourceRef<class GfxShaderParamDef,	zenRes::zGfxShaderParamDef>		GfxShaderParamDefRef;
	typedef ResourceRef<class GfxShaderParam,		zenRes::zGfxShaderParam>		GfxShaderParamRef;
	typedef ResourceRef<class GfxShaderBinding,		zenRes::zGfxShaderBinding>		GfxShaderBindingRef;
		
	typedef zEngineConstRef<GfxSamplerResData>										GfxSamplerResDataRef;
	typedef zEngineConstRef<GfxStateBlendResData>									GfxStateBlendResDataRef;
	typedef zEngineConstRef<GfxStateDepthStencilResData>							GfxStateDepthStencilResDataRef;
	typedef zEngineConstRef<GfxStateRasterizerResData>								GfxStateRasterizerResDataRef;
	typedef zEngineConstRef<GfxIndexResData>										GfxIndexResDataRef;
	typedef zEngineConstRef<GfxVertexResData>										GfxVertexResDataRef;
	typedef zEngineConstRef<GfxTexture2dResData>									GfxTexture2dResDataRef;
	typedef zEngineConstRef<GfxRenderTargetResData>									GfxRenderTargetResDataRef;
	typedef zEngineConstRef<GfxViewResData>											GfxViewResDataRef;
	typedef zEngineConstRef<GfxWindowResData>										GfxWindowResDataRef;
	typedef zEngineConstRef<GfxRenderPassResData>									GfxRenderPassResDataRef;
	typedef zEngineConstRef<GfxShaderResData>										GfxShaderResDataRef;
	typedef zEngineConstRef<GfxInputStreamResData>									GfxInputStreamResDataRef;
	typedef zEngineConstRef<GfxInputSignatureResData>								GfxInputSignatureResDataRef;
	typedef zEngineConstRef<GfxMeshStripResData>									GfxMeshStripResDataRef;
	typedef zEngineConstRef<GfxMeshResData>											GfxMeshResDataRef;
	typedef zEngineConstRef<GfxShaderParamDefResData>								GfxShaderParamDefResDataRef;
	typedef zEngineConstRef<GfxShaderParamResData>									GfxShaderParamResDataRef;
	typedef zEngineConstRef<GfxShaderBindingResData>								GfxShaderBindingResDataRef;	
}

#endif
