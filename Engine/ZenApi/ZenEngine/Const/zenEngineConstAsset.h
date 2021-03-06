#pragma once

#if ZEN_ENGINETOOL

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

	//! @note: When adding support for new parameter type in assets :
	//!			1-Add enum value here
	//!			2-Add matching ParameterDef class to support it (zenAssProperty.h)
	//!			3-Add description (saPropertyName in zenAssProperty.h)
	//!			4-Add the property in test Asset (zeAssItem.cpp:GetProperties() )
	//!			5-Add serialization support (zxAddLoaderXml, ...)

	enum eAssetPropertyType
	{
		keAssProp_Bool,
		keAssProp_File,
		keAssProp_Float,
		keAssProp_Float2,
		keAssProp_Float3,
		keAssProp_Float4,
		keAssProp_Int,
		keAssProp_Int2,
		keAssProp_Int3,
		keAssProp_Int4,
		keAssProp_Enum,
		keAssProp_Asset,
		keAssProp_Struct,
		keAssProp_Array,
		keAssProp__Count,
		keAssProp__Invalid
	};
}

namespace zenType
{
	typedef zFlag<zenConst::eAssetType,zenConst::keAssType__Count, zU64>	zFlagAssetType;
}

} //namespace zen

#endif
