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
const char* AssetItem::GetTypeDesc(zenConst::eAssetType _eAssetType)
{	
	ZENAssert(_eAssetType<zenConst::keAssType__Count);
	return sTypeDescription[_eAssetType].mzName;
}

//=================================================================================================
//! @brief		Find which enum has the same name hash
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param		_hAssetName	- Asset type name we want enum value from
//=================================================================================================
zenConst::eAssetType AssetItem::GetType(zenHash32 _hAssetName)
{
	zenUInt uIndex = zenStringHash32::Find(_hAssetName, sTypeDescription, ZENArrayCount(sTypeDescription));
	return uIndex < zenConst::keAssType__Count ? static_cast<zenConst::eAssetType>(uIndex) : zenConst::keAssType__Invalid;
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
	PropertyArray& propDefinition = GetPropertyDef();
	maPropertyValueOld.SetCount(propDefinition.Count());
	for(zenUInt valIdx(0), valCount(maPropertyValueOld.Count()); valIdx<valCount; ++valIdx)
	{
		maPropertyValueOld[valIdx] = propDefinition[valIdx]->Allocate();
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
	for(zenUInt valIdx(0), valCount(maPropertyValueOld.Count()); valIdx<valCount; ++valIdx)
	{
		zenDel(maPropertyValueOld[valIdx].GetBase());
	}
	maPropertyValueOld.SetCount(0);
}

//=================================================================================================
//! @brief		Find property with a particular name
//! @details	If none has been loaded yet, or the values do not perfectly match the 
//-------------------------------------------------------------------------------------------------
//! @return		PropertyValue pointer if found (null if not found)
//=================================================================================================	
ValuePointer AssetItem::GetPropertyValue(zenHash32 _hPropertyName)const
{
	zenUInt propIdx = GetPropertyDefIndex(_hPropertyName);
	if( propIdx < maPropertyValueOld.Count() )
		return maPropertyValueOld[propIdx];

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

	// Load properties values
 	for (pugi::xml_node nodeProp = nodeAsset.child(kzXmlName_Node_Property); nodeProp; nodeProp = nodeProp.next_sibling(kzXmlName_Node_Property))
	{
		const char* zPropName(nodeProp.attribute(kzXmlName_PropAtr_Name).as_string());
		zenHash32 hPropName( zPropName );
		zenUInt propIdx = GetPropertyDefIndex(hPropName);
		if( propIdx < maPropertyValueOld.Count() )
			bResult &= maPropertyValueOld[propIdx].GetBase()->ValueFromXml(nodeProp);
	}

	RebuiltDescription();
	return bResult;
}

bool AssetItem::Save( pugi::xml_document& _Doc )
{
	pugi::xml_node nodeAsset = _Doc.append_child(kzXmlName_Node_Asset);
	nodeAsset.append_attribute(kzXmlName_AssetAtr_Name).set_value("testName"); //!< @todo set proper na,e
	nodeAsset.append_attribute(kzXmlName_AssetAtr_Type).set_value(GetTypeDesc());
	for(zenUInt idx(0), count(maPropertyValueOld.Count()); idx<count; ++idx)
		maPropertyValueOld[idx].GetBase()->ValueToXml(nodeAsset);
	
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
	const zenArrayStatic<const PropertyDefBase*>& aPropertyDef = GetPropertyDef();
	_dPropertyMap.Init(aPropertyDef.Count()*2);
	_dPropertyMap.SetDefaultValue(0xFFFFFFFF);
	for(zenUInt idx(0), count(aPropertyDef.Count()); idx<count; ++idx)
		_dPropertyMap.Set( aPropertyDef[idx]->mhName, idx );
}


//=================================================================================================
//! @brief		Get the list of properties definition for this asset
//! @details	Used to know which data to read/write to xml
//-------------------------------------------------------------------------------------------------
//! @return		List of properties this asset needs
//=================================================================================================
const zenArrayStatic<const PropertyDefBase*>& TestProperty::GetPropertyDef() const
{
#if 0
	const PropertyDefEnum::Entry	enumEntries[]={	PropertyDefEnum::Entry(0, "Value0", "Enum Value 0"),	
													PropertyDefEnum::Entry(1, "Value1", "Enum Value 1"),	
													PropertyDefEnum::Entry(2, "Value2", "Enum Value 2"),	
													PropertyDefEnum::Entry(3, "Value3", "Enum Value 3"),	
													PropertyDefEnum::Entry(4, "Value4", "Enum Value 4"),	
													PropertyDefEnum::Entry(5, "Value5", "Enum Value 5")};
		
	static const PropertyDefBool	Property00("TestBool",		"", "Test Bool Field",		true,	false);		
	static const PropertyDefEnum	Property01("TestEnum",		"", "Test Enum Field",		true,	zenHash32("Value0"), enumEntries, ZENArrayCount(enumEntries));
	static const PropertyDefFile	Property02("Source",		"", "Texture file",			true,	"C:\\temp\\test.txt", "Images|*.bmp;*.png;*.jpeg;*.jpg|BMP(*.bmp)|*.bmp|PNG(*.png)|*.png|JPEG(*.jpg;*.jpeg)|*.jpg;*.jpeg");
	static const PropertyDefInt		Property03("TestIntA",		"", "Test Int Field",		true,	0, -10, 10,1);		
// 		static const PropertyDefInt2	Property03("TestInt2",		"", "Test Int2 Field",		true,	zenVec2S32(0,1), -10, 10);
// 		static const PropertyDefInt3	Property04("TestInt3",		"", "Test Int3 Field",		true,	zenVec3S32(0,1,2), -10, 10);
// 		static const PropertyDefInt4	Property05("TestInt4",		"", "Test Int4 Field",		true,	zenVec4S32(0,1,2,3), -10, 10);
	static const PropertyDefFloat	Property06("TestFloat",		"", "Test Float Field",		true,	0.5, -10, 10);
 	static const PropertyDefFloat2	Property07("TestFloat2",	"", "Test Float2 Field",	true,	zenVec2F(0,1.1f), zenVec2F(-10,-5), zenVec2F(10,5), zenVec2F(1,0.5));
// 		static const PropertyDefFloat3	Property08("TestFloat3",	"", "Test Float3 Field",	true,	zenVec3F(0,1.1f,2.2f), -10, 10);
// 		static const PropertyDefFloat4	Property09("TestFloat4",	"", "Test Float4 Field",	true,	zenVec4F(0,1.1f,2.2f,3.3f), -10, 10);
		
		
	static const PropertyDefBase*		aPropertyAll[] = {	&Property00, &Property01, &Property02, &Property03, /*&Property04, 
																&Property05,*/ &Property06, &Property07/*, &Property08, &Property09,
																&Property10*/};
#else
	static const PropertyDefBool	Property00("TestBool",		"", "Test Bool Field",		true,	false);		
	static const PropertyDefFile	Property02("Source",		"", "Texture file",			true,	"C:\\temp\\test.txt", "Images|*.bmp;*.png;*.jpeg;*.jpg|BMP(*.bmp)|*.bmp|PNG(*.png)|*.png|JPEG(*.jpg;*.jpeg)|*.jpg;*.jpeg");
	static const PropertyDefBase*	aPropertyAll[] = {&Property00, &Property02 };
#endif
	static zenArrayStatic<const PropertyDefBase*> saPropertyDef( aPropertyAll, ZENArrayCount(aPropertyAll) );
	return saPropertyDef;		
}


//=================================================================================================
//! @brief		Get index of PropertyDef with same name
//! @details	Use fast lookup through a dictionary of all supported properties
//-------------------------------------------------------------------------------------------------
//! @param hPropertyName	- Name of property to look for
//! @return					- Index of found item (0xFFFFFFFF if not found)
//=================================================================================================
zenUInt TestProperty::GetPropertyDefIndex(zenHash32 _hPropertyName)const	
{
	static zenMap<zenUInt>::Key32 sdPropertyIndex;
	if( !sdPropertyIndex.IsInit() )
		InitPropertyDefIndex(sdPropertyIndex);
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

void AssetItem::ResetDefault()
{	
	ZENAssert(maPropertyPointer.Count() > 0 );
	zenAss::PropertyValue* pValCur	= maPropertyPointer.First();
	zenAss::PropertyValue* pValLast = maPropertyPointer.Last();
	while( pValCur <= pValLast )
	{
		pValCur->Reset();
		++pValCur;
	}
}

void AssetItem::Allocate()
{
	const zenAss::PropertyArray& aProperties = GetProperties();	
	ZENAssert(aProperties.Count() > 0 );
	maPropertyPointer.SetCount( aProperties.Count() );
	const zenAss::PropertyBase* const*	pPropCur	= aProperties.First();
	zenAss::PropertyValue*				pValCur		= maPropertyPointer.First();
	zenAss::PropertyValue*				pValLast	= maPropertyPointer.Last();
	
	while( pValCur <= pValLast )
	{
		pValCur->Allocate(**pPropCur);
		++pValCur;
		++pPropCur;
	}
}

}} //namespace zen { namespace zeAss

#endif //ZEN_ENGINETOOL