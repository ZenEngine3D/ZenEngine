#include "libZenEngine.h"
#if ZEN_ENGINETOOL

namespace zen { namespace zeAss
{
zArrayStatic<zU32>	GfxMesh::saIcon;

//=================================================================================================
//! @brief		
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param hPropertyName	- Name of property to look for
//! @return					- Index of found item (-1 if not found)
//=================================================================================================
zInt GfxMesh::GetValueIndex(zHash32 _hPropertyName)const	
{
	static zMap<zInt>::Key32 sdPropertyIndex;
	static bool sbInit = InitPropertyMap(sdPropertyIndex);	//!< suEntryCount only used to init hashtable once
	return sdPropertyIndex[_hPropertyName];
}

const zenAss::zArrayProperty& GfxMesh::GetProperties()const
{ 	
	//zMap<zU32>::Key32					mdPropertyIndex; todo
	static const zenAss::PropertyFile	Property00("Source",		"", "3D Model", true,	"", "");
	static const zenAss::PropertyBase*	aPropertiesAll[] = {&Property00};
	static zenAss::zArrayProperty		saProperties( aPropertiesAll, ZENArrayCount(aPropertiesAll) );
	return saProperties;
}

}} //namespace zen { namespace zeAss

#endif //ZEN_ENGINETOOL
