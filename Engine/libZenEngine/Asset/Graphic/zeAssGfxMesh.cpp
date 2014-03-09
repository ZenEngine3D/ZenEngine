#include "libZenEngine.h"
#if ZEN_ENGINETOOL

namespace zen { namespace zeAss
{


zenArrayStatic<zenU32>	GfxMesh::saIcon;

#if 0

//=================================================================================================
//! @brief		
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param		
//! @return		
//=================================================================================================

//=================================================================================================
//! @brief		Get the list of properties definition for this asset
//! @details	Used to know which data to read/write to xml
//-------------------------------------------------------------------------------------------------
//! @return		List of properties this asset needs
//=================================================================================================
const zenArrayStatic<const PropertyDefBase*>& GfxMesh::GetPropertyDef() const
{
	static const PropertyDefFile		Property01("Source", "", "3D Model", true,	"", "");
	static const PropertyDefBase*		aPropertyAll[] = { &Property01 };
	static zenArrayStatic<const PropertyDefBase*> saPropertyDef(aPropertyAll, ZENArrayCount(aPropertyAll));
	return saPropertyDef;		
}

//=================================================================================================
//! @brief		Get index of PropertyDef with same name
//! @details	Use fast lookup through a dictionary of all supported properties
//-------------------------------------------------------------------------------------------------
//! @param hPropertyName	- Name of property to look for
//! @return					- Index of found item (0xFFFFFFFF if not found)
//=================================================================================================
zenUInt GfxMesh::GetPropertyDefIndex(zenHash32 _hPropertyName)const	
{
	static zenMap<zenUInt>::Key32 sdPropertyIndex;
	if( !sdPropertyIndex.IsInit() )
		InitPropertyDefIndex(sdPropertyIndex);

	return sdPropertyIndex[_hPropertyName];
}
#endif
//=================================================================================================
//! @brief		
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param hPropertyName	- Name of property to look for
//! @return					- Index of found item (-1 if not found)
//=================================================================================================
zenInt GfxMesh::GetValueIndex(zenHash32 _hPropertyName)const	
{
	static zenMap<zenInt>::Key32 sdPropertyIndex;
	static bool sbInit = InitPropertyMap(sdPropertyIndex);	//!< suEntryCount only used to init hashtable once
	return sdPropertyIndex[_hPropertyName];
}

const zenAss::PropertyArray& GfxMesh::GetProperties()const
{ 	
	//zenMap<zenU32>::Key32					mdPropertyIndex; todo
	static const zenAss::PropertyFile	Property00("Source",		"", "3D Model", true,	"", "");
	static const zenAss::PropertyBase*	aPropertiesAll[] = {&Property00};
	static zenAss::PropertyArray		saProperties( aPropertiesAll, ZENArrayCount(aPropertiesAll) );
	return saProperties;
}

}} //namespace zen { namespace zeAss

#endif //ZEN_ENGINETOOL
