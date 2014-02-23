#include "libZenEngine.h"
#if AW_ENGINETOOL

namespace FAss
{

awArrayStatic<awU32>	GfxMesh::saIcon;

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
const awArrayStatic<const FAss::PropertyDefBase*>& GfxMesh::GetPropertyDef() const
{
	static const FAss::PropertyDefFile		Property01("Source", "", "3D Model", true,	"", "");
	static const FAss::PropertyDefBase*		aPropertyAll[] = { &Property01 };
	static awArrayStatic<const FAss::PropertyDefBase*> saPropertyDef(aPropertyAll, AWArrayCount(aPropertyAll));
	return saPropertyDef;		
}

//=================================================================================================
//! @brief		Get index of PropertyDef with same name
//! @details	Use fast lookup through a dictionary of all supported properties
//-------------------------------------------------------------------------------------------------
//! @param hPropertyName	- Name of property to look for
//! @return					- Index of found item (0xFFFFFFFF if not found)
//=================================================================================================
awUInt GfxMesh::GetPropertyDefIndex(awHash32 _hPropertyName)const	
{
	static Hashmap<awUInt>::Key32 sdPropertyIndex;
	if( !sdPropertyIndex.IsInit() )
		InitPropertyDefIndex(sdPropertyIndex);

	return sdPropertyIndex[_hPropertyName];
}

}

#endif //AW_ENGINETOOL
