#include "libZenEngine.h"
#if AW_ENGINETOOL

namespace FAss
{

const char* kzXmlName_Node_Package="Package";
const char* kzXmlName_PkgAtr_Group="Group";

const char* kzXmlName_Node_Asset="Asset";
const char* kzXmlName_AssetAtr_Name="Name";
const char* kzXmlName_AssetAtr_Group="Group";
const char* kzXmlName_AssetAtr_Type="Type";

const char* kzXmlName_Node_Property="Property";
const char* kzXmlName_PropAtr_Name="Name";
const char* kzXmlName_PropAtr_Type="Type";

static zenStringHash32 sTypeDescription[]={
		zenStringHash32("Texture2D"),	//keType_Texture2D,
		zenStringHash32("Mesh"),		//keType_Mesh,
	};

AssetItem::AssetItem()	
: mpPackage(NULL)
, mhID("")
{
	AWStaticAssertMsg( AWArrayCount(sTypeDescription)==keType__Count, "Make sure to have a valid description for each resource type" );

	static zenU64 sCounter(1);
	mhID = sCounter++; //HACK
}

AssetItem::~AssetItem()	
{
	FreePropertyValues();
}

//=================================================================================================
//! @brief		Factory to create a new Asset of specified type
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param		_eAssetType	- Type of asset to create
//! @param		_Owner		- Owner package of the newly create Asset
//! @return					- Created Asset
//=================================================================================================
AssetItem* AssetItem::CreateItem( enumType _eAssetType, Package& _Owner )
{
	AssetItem* pNewItem(NULL);
	switch( _eAssetType )
	{
	case keType_Texture2D:	pNewItem = AWNewDefault FAss::GfxTexture2D();	break;
	case keType_Mesh:		pNewItem = AWNewDefault FAss::GfxMesh();		break;
	default:				AWAssertMsg(0, "Unsupported Asset Type");
	}

	if( pNewItem )
	{
		pNewItem->ResetPropertyValues();
		pNewItem->mpPackage = &_Owner;
	}
	return pNewItem;
}

//=================================================================================================
//! @brief		zenString representation of all Asset type enum
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param		_eAssetType	- Property we want name from
//! @return		- zenString representation of the enum value
//=================================================================================================
const char* AssetItem::GetTypeDesc(AssetItem::enumType _eAssetType)
{	
	AWAssert(_eAssetType<keType__Count);
	return sTypeDescription[_eAssetType].mzName;
}

//=================================================================================================
//! @brief		Find which enum has the same name hash
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param		_hAssetName	- Asset type name we want enum value from
//=================================================================================================
AssetItem::enumType AssetItem::GetType(zenHash32 _hAssetName)
{
	zenUInt uIndex = zenStringHash32::Find(_hAssetName, sTypeDescription, AWArrayCount(sTypeDescription));
	return uIndex < keType__Count ? static_cast<AssetItem::enumType>(uIndex) : keType__Invalid;
}

//=================================================================================================
//! @brief		Configure all supported properties for this Asset, to default values
//! @details	
//-------------------------------------------------------------------------------------------------
//! @return		- None
//=================================================================================================	
void AssetItem::ResetPropertyValues()
{
	FreePropertyValues();
	FAss::PropertyArray& propDefinition = GetPropertyDef();
	maPropertyValue.SetCount(propDefinition.Count());
	for(zenUInt valIdx(0), valCount(maPropertyValue.Count()); valIdx<valCount; ++valIdx)
	{
		maPropertyValue[valIdx] = propDefinition[valIdx]->Allocate();
	}
}

//=================================================================================================
//! @brief		De-allocate every values allocated
//! @details	
//-------------------------------------------------------------------------------------------------
//! @return		- None
//=================================================================================================	
void AssetItem::FreePropertyValues()
{
	for(zenUInt valIdx(0), valCount(maPropertyValue.Count()); valIdx<valCount; ++valIdx)
	{
		AWDel(maPropertyValue[valIdx].GetBase());
	}
	maPropertyValue.SetCount(0);
}

//=================================================================================================
//! @brief		Find property with a particular name
//! @details	If none has been loaded yet, or the values do not perfectly match the 
//-------------------------------------------------------------------------------------------------
//! @return		PropertyValue pointer if found (null if not found)
//=================================================================================================	
FAss::ValuePointer AssetItem::GetPropertyValue(zenHash32 _hPropertyName)const
{
	zenUInt propIdx = GetPropertyDefIndex(_hPropertyName);
	if( propIdx < maPropertyValue.Count() )
		return maPropertyValue[propIdx];

	return NULL;
}

//=================================================================================================
//! @brief		Load asset infos from xml stream
//! @details	Take open xml package and load all supported asset informations. Use child override
//!				'GetPropertyDef' to know what data to look for
//-------------------------------------------------------------------------------------------------
//! @return		True if no warning
//=================================================================================================
bool AssetItem::Load( const pugi::xml_node& nodeAsset )
{	
	bool bResult(true);
	
	// Reset properties values to 0
	ResetPropertyValues();

	// Load base asset infos
	const char* zAssetName	= nodeAsset.attribute(kzXmlName_AssetAtr_Name).as_string();
	const char* zAssetGroup	= nodeAsset.attribute(kzXmlName_AssetAtr_Group).as_string();
	LoadGroupAndName(zAssetName, zAssetGroup, maGroup );
	
	mhGroupID = zenHash32("Asset");
	for(int idx(0), count(maGroup.Count()-1); idx<count; ++idx )
		mhGroupID.Append( maGroup[idx] );

	// Load properies values
 	for (pugi::xml_node nodeProp = nodeAsset.child(kzXmlName_Node_Property); nodeProp; nodeProp = nodeProp.next_sibling(kzXmlName_Node_Property))
	{
		const char* zPropName(nodeProp.attribute(kzXmlName_PropAtr_Name).as_string());
	 zenHash32 hPropName( zPropName );
		zenUInt propIdx = GetPropertyDefIndex(hPropName);
		if( propIdx < maPropertyValue.Count() )
			bResult &= maPropertyValue[propIdx].GetBase()->ValueFromXml(nodeProp);
	}

	RebuiltDescription();
	return bResult;
}

bool AssetItem::Save( pugi::xml_document& _Doc )
{
	pugi::xml_node nodeAsset = _Doc.append_child(kzXmlName_Node_Asset);
	nodeAsset.append_attribute(kzXmlName_AssetAtr_Name).set_value("testName"); //!< @todo set proper na,e
	nodeAsset.append_attribute(kzXmlName_AssetAtr_Type).set_value(GetTypeDesc());
	for(zenUInt idx(0), count(maPropertyValue.Count()); idx<count; ++idx)
		maPropertyValue[idx].GetBase()->ValueToXml(nodeAsset);
	
	RebuiltDescription();
	return true;
}

//=================================================================================================
//! @brief		Build a string description based on asset property definitions/values
//! @details	Take all flagged properties and make a string description from it. Used for
//!				Asset short description in editor
//-------------------------------------------------------------------------------------------------
//! @return		
//=================================================================================================
void AssetItem::RebuiltDescription()
{	
	mzDescription = "";
	for(zenUInt idx(0), count(maPropertyValue.Count()); idx<count; ++idx)
	{
		const PropertyDefBase& propDef = maPropertyValue[idx].GetBase()->mParentDef;
		if( propDef.mbShowInAssetDesc )
		{
			char zBuffer[128];			
			char zValue[64];
			maPropertyValue[idx].GetBase()->ValueToString( zValue, sizeof(zValue) );
			sprintf(zBuffer, "(%s : %s) ", propDef.mzDisplayName, zValue);
			mzDescription += zBuffer;
		}		
	}
}

//=================================================================================================
//! @brief		Construct a dictionary mapping Property,s name to index in the property/value array
//! @details	Used by child class to initialize their static dictionary
//-------------------------------------------------------------------------------------------------
//! @param		_dPropertyMap - Dictionary to initialize with Property's Name / Index infos
//! @return		
//=================================================================================================
void AssetItem::InitPropertyDefIndex(zenMap<zenUInt>::Key32& _dPropertyMap)const
{	
	const zenArrayStatic<const FAss::PropertyDefBase*>& aPropertyDef = GetPropertyDef();
	_dPropertyMap.Init(aPropertyDef.Count()*2);
	_dPropertyMap.SetDefaultValue(0xFFFFFFFF);
	for(zenUInt idx(0), count(aPropertyDef.Count()); idx<count; ++idx)
		_dPropertyMap.Set( aPropertyDef[idx]->mhName, idx );
}

}

#endif //AW_ENGINETOOL