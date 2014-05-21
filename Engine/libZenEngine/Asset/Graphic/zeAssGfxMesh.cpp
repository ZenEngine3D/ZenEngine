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

const zenAss::zPropertyArray& GfxMesh::GetProperties()const
{ 	
	//! @todo Asset : TODO TODO
#if 1
	static const zenAss::PropertyBool	Property00("TestBool",		"", "Test Bool Field",		true,	false);		
	//zMap<zU32>::Key32					mdPropertyIndex; todo
	//static const zenAss::PropertyFile	Property00("Source",		"", "3D Model", true,	"", "");
	static const zenAss::PropertyDefinition*	aPropertiesAll[] = {&Property00};
#else

// 	static const zenAss::PropertyInt	PropertyInt		("PropertyInt",		"", "Property test: Int",	true,	0, -10, 10, 1);		
// 	static const zenAss::PropertyDefinition*	aPropertiesAll[] = {&PropertyInt};
#endif
 	static zenAss::zPropertyArray		saProperties( aPropertiesAll, ZENArrayCount(aPropertiesAll) );
	return saProperties;

}

}} //namespace zen { namespace zeAss

#endif //ZEN_ENGINETOOL
