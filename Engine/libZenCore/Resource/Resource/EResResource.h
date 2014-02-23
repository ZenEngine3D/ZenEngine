#pragma once
#ifndef __LibResource_Resource_Resource_h__
#define __LibResource_Resource_Resource_h__

namespace ERes
{	
	//=============================================================================================
	//! @class	Base class for all resources that can be created and used
	//=============================================================================================
	class Resource: public awres::awResource, public zenList2xNode
	{
	AWClassDeclare(Resource, awres::awResource)
	public:									
		virtual								~Resource();
		virtual bool						ResourceInit();
		virtual bool						ResourceCreate( EExp::ExportItem& _SerialItem, EExp::ExportInfoBase& _ExportInfo );
		bool								ResourceCreateRuntime(); //!< @todo Get rid of this
	protected:			
											Resource();
		static zenList2x						spResourceActive[awconst::keResType__Count];	//!< List of all actives resources created, per type
		static zenList2x						spActiveToDelete[3];							//!< List of resources to delete (with 1 frame delay to make sure GPU are done with them)
		static zenUInt						suDeleteIndex;									//!< Current active ToDelete list
	};
	

	//=============================================================================================
	//! @class	When a platform doesn't support a resource type, we use this in the typedef.
	//!			The code will compile, but gives an error if someone try to use it
	//=============================================================================================
	class UnsupportedResource : public Resource
	{
	AWClassDeclare(UnsupportedResource, Resource);
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
	AWClassDeclare(ResourceRef, TReferenceClass)
	public:	
		inline								ResourceRef();
		inline								ResourceRef(awres::awResource* _pResource);
		inline								ResourceRef(zenResID _ResourceID);
		inline								ResourceRef(const TReferenceClass& _Copy);
		inline TResourceClass*				operator->();		//!< Return a pointer to resource
		inline const TResourceClass*		operator->()const;	//!< Return a const pointiner to resource
	};
}

#include "EResResource.inl"
#include AWHeaderRenderer( EResResource )	// Each renderer resources typedef to generic name used in-engine
#include AWHeaderPlatform( EResResource )	// Each platform resources typedef to generic name used in-engine

namespace ERes
{	
	typedef ResourceRef<GfxIndex,			awres::awGfxIndex>			GfxIndexRef;
	typedef ResourceRef<GfxVertex,			awres::awGfxVertex>			GfxVertexRef;
	typedef ResourceRef<GfxInputStream,		awres::awGfxInputStream>	GfxInputStreamRef;
	typedef ResourceRef<GfxInputSignature,	awres::awGfxInputSignature>	GfxInputSignatureRef;
	typedef ResourceRef<GfxShaderBinding,	awres::awGfxShaderBinding>	GfxShaderBindingRef;
	typedef ResourceRef<GfxMesh,			awres::awGfxMesh>			GfxMeshRef;
	typedef ResourceRef<GfxMeshStrip,		awres::awGfxMeshStrip>		GfxMeshStripRef;
	typedef ResourceRef<GfxTexture2D,		awres::awGfxTexture2D>		GfxTexture2DRef;
	typedef ResourceRef<GfxRenderTarget,	awres::awGfxRenderTarget>	GfxRenderTargetRef;
	typedef ResourceRef<GfxView,			awres::awGfxView>			GfxViewRef;
	typedef ResourceRef<GfxSampler,			awres::awGfxSampler>		GfxSamplerRef;
	typedef ResourceRef<GfxWindow,			awres::awGfxWindow>			GfxWindowRef;
	typedef ResourceRef<GfxShaderParamDef,	awres::awGfxShaderParamDef>	GfxShaderParamDefRef;
	typedef ResourceRef<GfxShader,			awres::awGfxShader>			GfxShaderRef;
	typedef ResourceRef<GfxShaderVertex,	awres::awGfxShaderVertex>	GfxShaderVertexRef;
	typedef ResourceRef<GfxShaderPixel,		awres::awGfxShaderPixel>	GfxShaderPixelRef;
	typedef ResourceRef<GfxShaderParam,		awres::awGfxShaderParam>	GfxShaderParamRef;
}

#endif
