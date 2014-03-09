#include "libZenEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zeAss
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
		zenStringHash32("TestProperty"),	//keType_TestProperty,
		zenStringHash32("Texture2D"),		//keType_Texture2D,
		zenStringHash32("Mesh"),			//keType_Mesh,
	};

//=================================================================================================
//! @brief		Factory to create a new Asset of specified type
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param		_eAssetType	- Type of asset to create
//! @param		_Owner		- Owner package of the newly create Asset
//! @return					- Created Asset
//=================================================================================================
AssetItem* AssetItem::CreateItem( zenConst::eAssetType _eAssetType, Package& _Owner )
{
	AssetItem* pNewItem(NULL);
	switch( _eAssetType )
	{
	case zenConst::keAssType_TestProperty:	pNewItem = zenNewDefault TestProperty();	break;
	case zenConst::keAssType_Texture2D:		pNewItem = zenNewDefault GfxTexture2D();	break;
	case zenConst::keAssType_Mesh:			pNewItem = zenNewDefault GfxMesh();			break;
	default:								ZENAssertMsg(0, "Unsupported Asset Type");	break;
	}

	if( pNewItem )
	{
		pNewItem->InitDefault();
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
const char* AssetItem::GetTypeName(zenConst::eAssetType _eAssetType)
{	
	ZENAssert(_eAssetType<zenConst::keAssType__Count);
	return sTypeDescription[_eAssetType].mzName;
}

zenConst::eAssetType AssetItem::GetTypeFromName(zenHash32 _hAssetName)
{
	zenUInt idx = zenStringHash32::Find(_hAssetName, sTypeDescription, ZENArrayCount(sTypeDescription));
	return idx < zenConst::keAssType__Count ? static_cast<zenConst::eAssetType>(idx) : zenConst::keAssType__Invalid;
}

AssetItem::AssetItem()	
: mpPackage(NULL)
, mhID("")
{
    ZENStaticAssertMsg( ZENArrayCount(sTypeDescription)==zenConst::keAssType__Count, "Make sure to have a valid description for each resource type" );
	static zenU64 sCounter(1);
	mhID = sCounter++; //! @todo : fix this (HACK)	
}

AssetItem::~AssetItem()	
{
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
	InitDefault();

	// Load base asset infos
	const char* zAssetName	= nodeAsset.attribute(kzXmlName_AssetAtr_Name).as_string();
	const char* zAssetGroup	= nodeAsset.attribute(kzXmlName_AssetAtr_Group).as_string();
	LoadGroupAndName(zAssetName, zAssetGroup, maGroup );
	
	mhGroupID = zenHash32("Asset");
	for(int idx(0), count(maGroup.Count()-1); idx<count; ++idx )
		mhGroupID.Append( maGroup[idx] );
	/*
	//! @todo
	// Load properties values
 	for (pugi::xml_node nodeProp = nodeAsset.child(kzXmlName_Node_Property); nodeProp; nodeProp = nodeProp.next_sibling(kzXmlName_Node_Property))
	{
		const char* zPropName(nodeProp.attribute(kzXmlName_PropAtr_Name).as_string());
		zenHash32 hPropName( zPropName );
		zenUInt propIdx = GetPropertyDefIndex(hPropName);
		if( propIdx < maPropertyValueOld.Count() )
			bResult &= maPropertyValueOld[propIdx].GetBase()->ValueFromXml(nodeProp);
	}
	*/
	RebuiltDescription();
	return bResult;
}

bool AssetItem::Save( pugi::xml_document& _Doc )
{
	/*
	//! @todo
	pugi::xml_node nodeAsset = _Doc.append_child(kzXmlName_Node_Asset);
	nodeAsset.append_attribute(kzXmlName_AssetAtr_Name).set_value("testName"); //!< @todo set proper na,e
	nodeAsset.append_attribute(kzXmlName_AssetAtr_Type).set_value(GetTypeDesc());
	for(zenUInt idx(0), count(maPropertyValueOld.Count()); idx<count; ++idx)
		maPropertyValueOld[idx].GetBase()->ValueToXml(nodeAsset);
	*/
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
	//! @TODO
	/*
	for(zenUInt idx(0), count(maPropertyValueOld.Count()); idx<count; ++idx)
	{
		const PropertyDefBase& propDef = maPropertyValueOld[idx].GetBase()->mParentDef;
		if( propDef.mbShowInAssetDesc )
		{
			char zBuffer[128];			
			char zValue[64];
			maPropertyValueOld[idx].GetBase()->ValueToString( zValue, sizeof(zValue) );
			sprintf(zBuffer, "(%s : %s) ", propDef.mzDisplayName, zValue);
			mzDescription += zBuffer;
		}		
	}
	*/
}

void AssetItem::InitDefault()
{	
	// Only need to reset value
	if( maPropertyValue.Count() > 0 )
	{
		zenAss::PropertyValue* pValCur	= maPropertyValue.First();
		zenAss::PropertyValue* pValLast = maPropertyValue.Last();
		while( pValCur <= pValLast )
		{
			pValCur->Reset();
			++pValCur;
		}
	}
	// Need to allocate value (which sets it to default)
	else
	{
		const zenAss::PropertyArray& aProperties = GetProperties();	
		ZENAssert(aProperties.Count() > 0 );
		maPropertyValue.SetCount( aProperties.Count() );
		const zenAss::PropertyBase* const*	pPropCur	= aProperties.First();
		zenAss::PropertyValue*				pValCur		= maPropertyValue.First();
		zenAss::PropertyValue*				pValLast	= maPropertyValue.Last();
		while( pValCur <= pValLast )
		{
			pValCur->Allocate(**pPropCur);
			++pValCur;
			++pPropCur;
		}
	}
}

//=================================================================================================
//! @brief		Construct a dictionary mapping Properties name to index in the property/value array
//! @details	Used by child class to initialize their static dictionary
//-------------------------------------------------------------------------------------------------
//! @param		_dPropertyMap - Dictionary to initialize with Property's Name / Index infos
//! @return		
//=================================================================================================
bool AssetItem::InitPropertyMap(zenMap<zenInt>::Key32& _dPropertyMap)const
{	
	const zenAss::PropertyArray& aPropertyDef = GetProperties();
	_dPropertyMap.Init(aPropertyDef.Count()*2);
	_dPropertyMap.SetDefaultValue(-1);
	for(zenInt idx(0), count(aPropertyDef.Count()); idx<count; ++idx)
		_dPropertyMap.Set( aPropertyDef[idx]->mName.mhName, idx );
	return _dPropertyMap.Count() > 0;
}

//=================================================================================================
//! @brief		
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param hPropertyName	- Name of property to look for
//! @return					- Index of found item (-1 if not found)
//=================================================================================================
zenInt TestProperty::GetValueIndex(zenHash32 _hPropertyName)const	
{
	static zenMap<zenInt>::Key32 sdPropertyIndex;
	static bool sbInit = InitPropertyMap(sdPropertyIndex);	//!< suEntryCount only used to init hashtable once
	return sdPropertyIndex[_hPropertyName];
}

const zenAss::PropertyArray& TestProperty::GetProperties()const
{ 	
	//zenMap<zenU32>::Key32					mdPropertyIndex; todo
	static const zenAss::PropertyBool	Property00("TestBool",		"", "Test Bool Field",		true,	false);		
	static const zenAss::PropertyFile	Property02("Source",		"", "Texture file",			true,	"C:\\temp\\test.txt", "Images|*.bmp;*.png;*.jpeg;*.jpg|BMP(*.bmp)|*.bmp|PNG(*.png)|*.png|JPEG(*.jpg;*.jpeg)|*.jpg;*.jpeg");
	static const zenAss::PropertyBase*	aPropertiesAll[] = {&Property00, &Property02 };
	static zenAss::PropertyArray		saProperties( aPropertiesAll, ZENArrayCount(aPropertiesAll) );
	return saProperties;
}





}} //namespace zen { namespace zeAss

#endif //ZEN_ENGINETOOL