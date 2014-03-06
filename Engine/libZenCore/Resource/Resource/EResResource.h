#pragma once
#ifndef __LibResource_Resource_Resource_h__
#define __LibResource_Resource_Resource_h__

namespace ERes
{	
	//=============================================================================================
	//! @class	Base class for all resources that can be created and used
	//=============================================================================================
	class Resource: public zenRes::awResource, public zenList2xNode
	{
	ZENClassDeclare(Resource, zenRes::awResource)
	public:									
		virtual								~Resource();
		virtual bool						ResourceInit();
		virtual bool						ResourceCreate( EExp::ExportItem& _SerialItem, EExp::ExportInfoBase& _ExportInfo );
		bool								ResourceCreateRuntime(); //!< @todo Get rid of this
	protected:			
											Resource();
		static zenList2x						spResourceActive[zenConst::keResType__Count];	//!< List of all actives resources created, per type
		static zenList2x						spActiveToDelete[3];							//!< List of resources to delete (with 1 frame delay to make sure GPU are done with them)
		static zenUInt						suDeleteIndex;									//!< Current active ToDelete list
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
	//! @class	Templated specialized version of ResourceAnyRef, that does ResourceType check
	//!			before use, making sure we're not mis-casting to wrong ResourceType
	//=============================================================================================
	template<class TResourceClass, class TReferenceClass>
	class ResourceRef : public TReferenceClass
	{
	ZENClassDeclare(ResourceRef, TReferenceClass)
	public:	
		ZENInline								ResourceRef();
		ZENInline								ResourceRef(zenRes::awResource* _pResource);
		ZENInline								ResourceRef(zenResID _ResourceID);
		ZENInline								ResourceRef(const TReferenceClass& _Copy);
		ZENInline TResourceClass*				operator->();		//!< Return a pointer to resource
		ZENInline const TResourceClass*		operator->()const;	//!< Return a const pointiner to resource
	};
}

#include "EResResource.inl"
#include ZENHeaderRenderer( EResResource )	// Each renderer resources typedef to generic name used in-engine
#include ZENHeaderPlatform( EResResource )	// Each platform resources typedef to generic name used in-engine

namespace ERes
{	
	typedef ResourceRef<GfxIndex,			zenRes::GfxIndex>			GfxIndexRef;
	typedef ResourceRef<GfxVertex,			zenRes::GfxVertex>			GfxVertexRef;
	typedef ResourceRef<GfxInputStream,		zenRes::GfxInputStream>	GfxInputStreamRef;
	typedef ResourceRef<GfxInputSignature,	zenRes::GfxInputSignature>	GfxInputSignatureRef;
	typedef ResourceRef<GfxShaderBinding,	zenRes::GfxShaderBinding>	GfxShaderBindingRef;
	typedef ResourceRef<GfxMesh,			zenRes::GfxMesh>			GfxMeshRef;
	typedef ResourceRef<GfxMeshStrip,		zenRes::GfxMeshStrip>		GfxMeshStripRef;
	typedef ResourceRef<GfxTexture2D,		zenRes::GfxTexture2D>		GfxTexture2DRef;
	typedef ResourceRef<GfxRenderTarget,	zenRes::GfxRenderTarget>	GfxRenderTargetRef;
	typedef ResourceRef<GfxView,			zenRes::GfxView>			GfxViewRef;
	typedef ResourceRef<GfxSampler,			zenRes::GfxSampler>		GfxSamplerRef;
	typedef ResourceRef<GfxWindow,			zenRes::GfxWindow>			GfxWindowRef;
	typedef ResourceRef<GfxShaderParamDef,	zenRes::GfxShaderParamDef>	GfxShaderParamDefRef;
	typedef ResourceRef<GfxShader,			zenRes::GfxShader>			GfxShaderRef;
	typedef ResourceRef<GfxShaderVertex,	zenRes::GfxShaderVertex>	GfxShaderVertexRef;
	typedef ResourceRef<GfxShaderPixel,		zenRes::GfxShaderPixel>	GfxShaderPixelRef;
	typedef ResourceRef<GfxShaderParam,		zenRes::GfxShaderParam>	GfxShaderParamRef;
}

#endif
