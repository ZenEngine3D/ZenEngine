#pragma once
#ifndef __LibResource_Resource_Resource_h__
#define __LibResource_Resource_Resource_h__

namespace ERes
{	
	//=============================================================================================
	//! @class	Base class for all resources that can be created and used
	//=============================================================================================
	class Resource: public zenRes::zResource, public zList2xNode
	{
	ZENClassDeclare(Resource, zenRes::zResource)
	public:									
		virtual								~Resource();
		virtual bool						ResourceInit();
		virtual bool						ResourceCreate( EExp::ExportItem& _SerialItem, EExp::ExportInfoBase& _ExportInfo );
		bool								ResourceCreateRuntime(); //!< @todo Get rid of this
	protected:			
											Resource();
		static zList2x						spResourceActive[zenConst::keResType__Count];	//!< List of all actives resources created, per type
		static zList2x						spActiveToDelete[3];							//!< List of resources to delete (with 1 frame delay to make sure GPU are done with them)
		static zUInt						suDeleteIndex;									//!< Current active ToDelete list
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
		ZENInline								ResourceRef(zenRes::zResource* _pResource);
		ZENInline								ResourceRef(zResID _ResourceID);
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
	typedef ResourceRef<zGfxIndex,			zenRes::zGfxIndex>			GfxIndexRef;
	typedef ResourceRef<zGfxVertex,			zenRes::zGfxVertex>			GfxVertexRef;
	typedef ResourceRef<zGfxInputStream,		zenRes::zGfxInputStream>	GfxInputStreamRef;
	typedef ResourceRef<zGfxInputSignature,	zenRes::zGfxInputSignature>	GfxInputSignatureRef;
	typedef ResourceRef<zGfxShaderBinding,	zenRes::zGfxShaderBinding>	GfxShaderBindingRef;
	typedef ResourceRef<zGfxMesh,			zenRes::zGfxMesh>			GfxMeshRef;
	typedef ResourceRef<zGfxMeshStrip,		zenRes::zGfxMeshStrip>		GfxMeshStripRef;
	typedef ResourceRef<zGfxTexture2D,		zenRes::zGfxTexture2D>		GfxTexture2DRef;
	typedef ResourceRef<zGfxRenderTarget,	zenRes::zGfxRenderTarget>	GfxRenderTargetRef;
	typedef ResourceRef<zGfxView,			zenRes::zGfxView>			GfxViewRef;
	typedef ResourceRef<zGfxSampler,			zenRes::zGfxSampler>		GfxSamplerRef;
	typedef ResourceRef<zGfxWindow,			zenRes::zGfxWindow>			GfxWindowRef;
	typedef ResourceRef<zGfxShaderParamDef,	zenRes::zGfxShaderParamDef>	GfxShaderParamDefRef;
	typedef ResourceRef<zGfxShader,			zenRes::zGfxShader>			GfxShaderRef;
	typedef ResourceRef<zGfxShaderVertex,	zenRes::zGfxShaderVertex>	GfxShaderVertexRef;
	typedef ResourceRef<zGfxShaderPixel,		zenRes::zGfxShaderPixel>	GfxShaderPixelRef;
	typedef ResourceRef<zGfxShaderParam,		zenRes::zGfxShaderParam>	GfxShaderParamRef;
}

#endif
