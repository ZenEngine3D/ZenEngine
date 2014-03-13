#pragma once
#ifndef __zenEngine_Const_Asset_h__
#define __zenEngine_Const_Asset_h__

#if ZEN_ENGINETOOL

//! @note: When adding support for new parameter type in assets :
//!			1-Add name here
//!			2-Add matching ParameterDefinition class to support it, with same ending name
//!	To use this macro, define 'ZEN_ASSETPROPERTIES_EXPAND_CODE' to the content you want expanded with 
//! each supported parameter name, and then undef it.
#define ZEN_ASSETPROPERTIES_EXPAND	ZEN_ASSETPROPERTIES_EXPAND_CODE(Bool)		\
									ZEN_ASSETPROPERTIES_EXPAND_CODE(File)		\
//									ZEN_ASSETPROPERTIES_EXPAND_CODE(Int)		\
//									ZEN_ASSETPROPERTIES_EXPAND_CODE(Float)		\
//									ZEN_ASSETPROPERTIES_EXPAND_CODE(Float2)		\
//									ZEN_ASSETPROPERTIES_EXPAND_CODE(Enum)		\									
// 									ZEN_ASSETPROPERTIES_EXPAND_CODE(Int2)		\
// 									ZEN_ASSETPROPERTIES_EXPAND_CODE(Int3)		\
// 									ZEN_ASSETPROPERTIES_EXPAND_CODE(Int4)		\
// 									ZEN_ASSETPROPERTIES_EXPAND_CODE(Float3)		\
// 									ZEN_ASSETPROPERTIES_EXPAND_CODE(Float4)		\
// 									ZEN_ASSETPROPERTIES_EXPAND_CODE(AssetRef)	

namespace zen 
{ 

namespace zenConst 
{
	enum eAssetType
	{
		keAssType_TestProperty,
		keAssType_Texture2D,
		keAssType_Mesh,
		keAssType__Count,
		keAssType__Invalid
	};
	
	#define ZEN_ASSETPROPERTIES_EXPAND_CODE(_TypeName_)	keAssProp_##_TypeName_,
	enum eAssetPropertyType
	{
		ZEN_ASSETPROPERTIES_EXPAND
		keAssProp__Count,
		keAssProp__Invalid
	};
	#undef ZEN_ASSETPROPERTIES_EXPAND_CODE
}

namespace zenType
{
	typedef zFlag<zenConst::eAssetType,zenConst::keAssType__Count, zU64>	zenFlagAssetType;
}

} //namespace zen

#endif
#endif