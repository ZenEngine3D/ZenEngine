#pragma once
#ifndef __LibCore_Api_Type_ResourceID_h__
#define __LibCore_Api_Type_ResourceID_h__

namespace Api { namespace Type {

//=================================================================================================
//! @class		Resource
//-------------------------------------------------------------------------------------------------
//! @brief				
//=================================================================================================
class ResourceID
{
AWClassDeclareNoParent(ResourceID)
public:
	typedef Api::Type::awHash32	NameHash;
	typedef awU64					TypeMask ;
	enum eSource
	{
		keSource_Offline,		//!< Being exported in offline mode (converted to 'Loaded' on save)
		keSource_Loaded,		//!< Loaded from a package
		keSource_Runtime,		//!< Created at runtime, and using generated name
		keSource__Count,
		keSource__Invalid,
	};
	enum ePlatform
	{
		//--- Renderer ---
		kePlatform_DX11,

		//--- OS ---
		kePlatform_Window,

		//--- ... ---
		kePlatform__Count,
		kePlatform__Invalid,
	};	
	enum ePlatformType
	{
		kePlatformType_OS,
		kePlatformType_GFX,
		kePlatformType__Count,
	};
	enum eType{
		//--- GFX ---
		keType_GfxShaderPixel,
		keType_GfxShaderVertex,
		keType_GfxShaderInputSignature,	
		keType_GfxShaderParamDef,
		keType_GfxShaderParam,
		keType_GfxShaderBinding,		
		keType_GfxRenderTarget,
		keType_GfxView,
		keType_GfxWindow,
		keType_GfxIndex,
		keType_GfxVertex,	
		keType_GfxTexture2D,
		keType_GfxSampler,
		keType_GfxMeshStrip,
		keType_GfxMesh,
		keType_GfxInputStreamBinding,

		//--- .... ---	
		keType__Count,
		keType__Invalid,															// Invalid resource
		keType__MaskAll		= (TypeMask(1)<<keType__Count)-1,						// Used to create mask of all valid resources
		keType__MaskGfx		= (TypeMask(1)<<(keType_GfxInputStreamBinding+1))-1,	// Used to create mask of all GFX resources
		keType__MaskShaders	= (TypeMask(1)<<keType_GfxShaderPixel) | (TypeMask(1)<<keType_GfxShaderVertex),		
	};
								ResourceID():mhName(""), meType(keType__Invalid), mePlatform(kePlatform__Invalid), meSource(keSource_Runtime){}
								ResourceID(NameHash _hName, ePlatform _ePlatform, eType _eType, eSource _eSource):mhName(_hName), meType(_eType), mePlatform(_ePlatform), meSource(_eSource){}								
								ResourceID& operator=(const ResourceID& _Copy){muHashID = _Copy.muHashID; return *this;}
								
		
	inline const NameHash&		Name()const							{ return mhName; }
	inline eType				Type()const							{ return static_cast<eType>(meType); }
	inline ePlatform			Platform()const						{ return static_cast<ePlatform>(mePlatform); } 
	inline eSource				Source()const						{ return static_cast<eSource>(meSource); }
	inline awU64					HashID()const						{ return muHashID; }
	inline void					SetName(NameHash _hName)			{ mhName = _hName; }
	inline void					SetType(eType _eResType)			{ meType = _eResType; }
	inline void					SetPlatform(ePlatform _ePlatform)	{ mePlatform = _ePlatform; }
	inline void					SetSource(eSource _eSource)			{ meSource = _eSource; }
	inline bool					IsRuntime()const					{ return meSource==keSource_Runtime; }
	inline bool					IsExport()const						{ return meSource==keSource_Offline; }
	inline bool					IsLoaded()const						{ return meSource==keSource_Loaded; }
	inline bool					IsValid()const						{ return meType < ResourceID::keType__Count; }
	inline bool					IsValidMask(eType _eTypeMask)const	{ AWAssert(_eTypeMask>=keType__MaskAll); return ((1<<meType) & _eTypeMask) != 0;}
	inline bool					operator==(const ResourceID& _Compare)const{return muHashID == _Compare.muHashID;};
	inline bool					operator!=(const ResourceID& _Compare)const{return muHashID != _Compare.muHashID;};
	
protected:
	union
	{
		struct 
		{			
			NameHash	mhName;		//!< Hashed name of the resource
			awU16			meType;		//!< Resource type (eType)
			awU8			mePlatform;	//!< Platform this should be used on
			awU8			meSource;	//!< Source of the resource (eSource)			
		};
		awU64	muHashID;
	};
};

#if AW_PLATFORM_PC
const ResourceID::ePlatform kAWCurrentPlatformOS	= ResourceID::kePlatform_Window;
#endif

#if AW_RENDERER_DX11
const ResourceID::ePlatform kAWCurrentPlatformGfx	= ResourceID::kePlatform_DX11;
#endif
}}  //namespace Api, Type 

#endif