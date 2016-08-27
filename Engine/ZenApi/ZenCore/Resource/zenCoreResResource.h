#pragma once
#ifndef __zenApi_Core_ResRef_h__
#define __zenApi_Core_ResRef_h__

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
	class zResourceRef;

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

namespace zcRes
{
typedef zenRes::zResourceRef<class zenRes::zExportData,	class zenRes::zExportData,		0xFFFFFFFFFFFFFFFF>								ResourceAnyRef;
typedef zenRes::zResourceRef<class GfxSampler,			class GfxSamplerHAL,			zEnumMask<zU64>(keResType_GfxSampler)>			GfxSamplerRef;
typedef zenRes::zResourceRef<class GfxRenderPass,		class GfxRenderPassHAL, 		zEnumMask<zU64>(keResType_GfxRenderPass)>		GfxRenderPassRef;
typedef zenRes::zResourceRef<class GfxMeshStrip,		class GfxMeshStripHAL,			zEnumMask<zU64>(keResType_GfxMeshStrip)>		GfxMeshStripRef;
typedef zenRes::zResourceRef<class GfxMesh,				class GfxMeshHAL,				zEnumMask<zU64>(keResType_GfxMesh)>				GfxMeshRef;
typedef zenRes::zResourceRef<class GfxShaderBinding,	class GfxShaderBindingHAL,		zEnumMask<zU64>(keResType_GfxShaderBinding)>	GfxShaderBindingRef;
typedef zenRes::zResourceRef<class GfxShaderParamDef,	class GfxShaderParamDefHAL,		zEnumMask<zU64>(keResType_GfxShaderParamDef)>	GfxShaderParamDefRef;
typedef zenRes::zResourceRef<class GfxShaderParam,		class GfxShaderParamHAL,		zEnumMask<zU64>(keResType_GfxShaderParam)>		GfxShaderParamRef;
typedef zenRes::zResourceRef<class GfxInputSignature,	class GfxInputSignatureHAL,		zEnumMask<zU64>(keResType_GfxInputSignature)>	GfxInputSignatureRef;
typedef zenRes::zResourceRef<class GfxInputStream,		class GfxInputStreamHAL,		zEnumMask<zU64>(keResType_GfxInputStream)>		GfxInputStreamRef;
typedef zenRes::zResourceRef<class zExportData,			class zExportData,				zEnumMask<zU64>(keResType_GfxShaderPixel, keResType_GfxShaderVertex) > GfxShaderAnyRef;
typedef zenRes::zResourceRef<class GfxShaderPixel,		class GfxShaderPixelHAL,		zEnumMask<zU64>(keResType_GfxShaderPixel)>		GfxShaderPixelRef;
typedef zenRes::zResourceRef<class GfxShaderVertex,		class GfxShaderVertexHAL,		zEnumMask<zU64>(keResType_GfxShaderVertex)>		GfxShaderVertexRef;
typedef zenRes::zResourceRef<class GfxStateBlend,		class GfxStateBlendHAL,			zEnumMask<zU64>(keResType_GfxBlend)>			GfxStateBlendRef;
typedef zenRes::zResourceRef<class GfxStateDepthStencil,class GfxStateDepthStencilHAL,	zEnumMask<zU64>(keResType_GfxDepthStencil)>		GfxStateDepthStencilRef;
typedef zenRes::zResourceRef<class GfxStateRaster,		class GfxStateRasterHAL,		zEnumMask<zU64>(keResType_GfxRaster)>			GfxStateRasterRef;
typedef zenRes::zResourceRef<class GfxVertex,			class GfxVertexHAL,				zEnumMask<zU64>(keResType_GfxVertex)>			GfxVertexRef;
typedef zenRes::zResourceRef<class GfxWindow,			class GfxWindowHAL,				zEnumMask<zU64>(keResType_GfxWindow)>			GfxWindowRef;
typedef zenRes::zResourceRef<class GfxView,				class GfxViewHAL,				zEnumMask<zU64>(keResType_GfxView)>				GfxViewRef;
typedef zenRes::zResourceRef<class GfxTarget2D,			class GfxTarget2DHAL,			zEnumMask<zU64>(keResType_GfxTarget2D)>			GfxTarget2DRef;
typedef zenRes::zResourceRef<class GfxTexture2d,		class GfxTexture2dHAL,			zEnumMask<zU64>(keResType_GfxTexture2D)>		GfxTexture2dRef;
typedef zenRes::zResourceRef<class GfxIndex,			class GfxIndexHAL,				zEnumMask<zU64>(keResType_GfxIndex)>			GfxIndexRef;
typedef zenRes::zResourceRef<class GfxBuffer,			class GfxBufferHAL,				zEnumMask<zU64>(keResType_GfxBuffer)>			GfxBufferRef;

};

#endif
