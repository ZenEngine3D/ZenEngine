#include "libZenExternal.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zxAss
{
const char* kzXmlName_Node_Package		= "Package";
const char* kzXmlName_PkgAtr_ID			= "ID";
const char* kzXmlName_PkgAtr_Group		= "Group";
const char* kzXmlName_PkgAtr_Version	= "EngineVersion";

const char* kzXmlName_Node_Asset		= "Asset";
const char* kzXmlName_AssetAtr_ID		= "ID";
const char* kzXmlName_AssetAtr_Name		= "Name";
const char* kzXmlName_AssetAtr_Group	= "Group";
const char* kzXmlName_AssetAtr_Type		= "Type";
const char* kzXmlName_AssetAtr_Version	= "Version";


const char* kzXmlName_Node_Property		= "Property";
const char* kzXmlName_PropAtr_Name		= "Name";
const char* kzXmlName_PropAtr_Type		= "Type";
const char* kzXmlName_PropAtr_Value		= "Value";


void LoadPropertyValue( const zenAss::PropertyValueRef& _rValue, const pugi::xml_node& _Node )
{
	switch( _rValue.GetType() )
	{
	case zenConst::keAssProp_Bool:	{	
		zenAss::PropertyBool::ValueRef rValueTyped(_rValue);
		rValueTyped = _Node.attribute("Value").as_bool();	
	}break;
	case zenConst::keAssProp_File:	{	
		zenAss::PropertyFile::ValueRef rValueTyped(_rValue);
		//! @todo Asset: Finalize string handling
		zChar zFilename[1024];
		mbstowcs_s(NULL, zFilename, ZENArrayCount(zFilename), _Node.attribute("Value").as_string(), ZENArrayCount(zFilename));
		rValueTyped = zFilename;	
	}break;
	case zenConst::keAssProp_Float:	{	
		zenAss::PropertyFloat::ValueRef rValueTyped(_rValue);
		rValueTyped = _Node.attribute("Value").as_float();	
	}break;
	case zenConst::keAssProp_Float2:{	
		zenAss::PropertyFloat2::ValueRef rValueTyped(_rValue);
		rValueTyped = zVec2F(	_Node.attribute("X").as_float(), 
								_Node.attribute("Y").as_float()); 
	}break;
	case zenConst::keAssProp_Float3:{ 	
		zenAss::PropertyFloat3::ValueRef rValueTyped(_rValue);
		rValueTyped = zVec3F(	_Node.attribute("X").as_float(), 
								_Node.attribute("Y").as_float(), 
								_Node.attribute("Z").as_float()); 
	}break;
	case zenConst::keAssProp_Float4:{	
		zenAss::PropertyFloat4::ValueRef rValueTyped(_rValue);
		rValueTyped = zVec4F(	_Node.attribute("X").as_float(), 
								_Node.attribute("Y").as_float(), 
								_Node.attribute("Z").as_float(), 
								_Node.attribute("W").as_float()); 
	}break;
	case zenConst::keAssProp_Int:	{	
		zenAss::PropertyInt::ValueRef rValueTyped(_rValue);
		rValueTyped = _Node.attribute("Value").as_int(); 
	}break;
	case zenConst::keAssProp_Int2:	{ 	
		zenAss::PropertyInt2::ValueRef rValueTyped(_rValue);
		rValueTyped = zVec2S32(	_Node.attribute("X").as_int(), 
								_Node.attribute("Y").as_int()); 
	}break;
	case zenConst::keAssProp_Int3:	{	
		zenAss::PropertyInt3::ValueRef rValueTyped(_rValue);
		rValueTyped = zVec3S32(	_Node.attribute("X").as_int(), 
								_Node.attribute("Y").as_int(), 
								_Node.attribute("Z").as_int()); 
	}break;
	case zenConst::keAssProp_Int4:	{	
		zenAss::PropertyInt4::ValueRef rValueTyped(_rValue);
		rValueTyped = zVec4S32(	_Node.attribute("X").as_int(), 
								_Node.attribute("Y").as_int(), 
								_Node.attribute("Z").as_int(), 
								_Node.attribute("W").as_int()); 
	}break;
 	case zenConst::keAssProp_Enum:	{	
		zenAss::PropertyEnum::ValueRef rValueTyped(_rValue);
		rValueTyped = _Node.attribute("Value").as_string();
	}break;
	case zenConst::keAssProp_Struct:{	
		zenAss::PropertyStruct::ValueRef rValueTyped(_rValue);
 		for (pugi::xml_node _NodeChild = _Node.child(kzXmlName_Node_Property); _NodeChild; _NodeChild = _NodeChild.next_sibling(kzXmlName_Node_Property))
 		{
 			const char* zPropName				= _NodeChild.attribute(kzXmlName_PropAtr_Name).as_string();				
 			zenAss::PropertyValueRef structVal 	= rValueTyped.GetStructValue( zHash32(zPropName) );
 			if( structVal.IsValid() )
 				LoadPropertyValue( structVal, _NodeChild);
 		}
	}break;
	case zenConst::keAssProp_Array:	{	
		zenAss::PropertyArray::ValueRef rValueTyped(_rValue);
		zUInt uCount	= _Node.attribute("Num").as_uint();
		uCount			= zenMath::Clamp( uCount, rValueTyped.GetDefinition().muEntryCountMin, rValueTyped.GetDefinition().muEntryCountMax );
		rValueTyped.GetValue().SetCount( uCount );
		for (pugi::xml_node _NodeChild = _Node.child(kzXmlName_Node_Property); _NodeChild; _NodeChild = _NodeChild.next_sibling(kzXmlName_Node_Property))
		{
			zInt idx = _NodeChild.attribute("Index").as_int()-1;
			if( idx >= 0 && idx < zInt(uCount) )
				LoadPropertyValue( rValueTyped.GetValue()[idx], _NodeChild);
		}
	}break;
	default:
		ZENAssertMsg(0, "Unsupported Asset Property type while loading.");
	}
}

void SavePropertyValue(const zenAss::PropertyValueRef& _rValue, pugi::xml_node& _Node)
{
	switch( _rValue.GetType() )
	{
	case zenConst::keAssProp_Bool:	{	
		zenAss::PropertyBool::ValueRef rValueTyped(_rValue);
		_Node.append_attribute("Value").set_value( rValueTyped.GetValue() );
	}break;
	case zenConst::keAssProp_File:	{	
		zenAss::PropertyFile::ValueRef rValueTyped(_rValue);		
		//! @todo Asset: Finalize string handling
		char zFilename[1024];
		wcstombs_s(NULL, zFilename, ZENArrayCount(zFilename), rValueTyped.GetValue(), ZENArrayCount(zFilename));
		_Node.append_attribute("Value").set_value( zFilename );
	}break;
	case zenConst::keAssProp_Float:	{	
		zenAss::PropertyFloat::ValueRef rValueTyped(_rValue);
		_Node.append_attribute("Value").set_value( rValueTyped.GetValue() );
	}break;
	case zenConst::keAssProp_Float2:{	
		zenAss::PropertyFloat2::ValueRef rValueTyped(_rValue);
		_Node.append_attribute("X").set_value( rValueTyped.GetValue().x );
		_Node.append_attribute("Y").set_value( rValueTyped.GetValue().y );
	}break;
	case zenConst::keAssProp_Float3:{ 	
		zenAss::PropertyFloat3::ValueRef rValueTyped(_rValue);
		_Node.append_attribute("X").set_value( rValueTyped.GetValue().x );
		_Node.append_attribute("Y").set_value( rValueTyped.GetValue().y );
		_Node.append_attribute("Z").set_value( rValueTyped.GetValue().z );
	}break;
	case zenConst::keAssProp_Float4:{	
		zenAss::PropertyFloat4::ValueRef rValueTyped(_rValue);
		_Node.append_attribute("X").set_value( rValueTyped.GetValue().x );
		_Node.append_attribute("Y").set_value( rValueTyped.GetValue().y );
		_Node.append_attribute("Z").set_value( rValueTyped.GetValue().z );
		_Node.append_attribute("W").set_value( rValueTyped.GetValue().w );
	}break;
	case zenConst::keAssProp_Int:	{
		zenAss::PropertyInt::ValueRef rValueTyped(_rValue);
		_Node.append_attribute("Value").set_value( rValueTyped.GetValue() );
	}break;
	case zenConst::keAssProp_Int2:	{ 	
		zenAss::PropertyInt2::ValueRef rValueTyped(_rValue);
		_Node.append_attribute("X").set_value( rValueTyped.GetValue().x );
		_Node.append_attribute("Y").set_value( rValueTyped.GetValue().y );
	}break;
	case zenConst::keAssProp_Int3:	{	
		zenAss::PropertyInt3::ValueRef rValueTyped(_rValue);
		_Node.append_attribute("X").set_value( rValueTyped.GetValue().x );
		_Node.append_attribute("Y").set_value( rValueTyped.GetValue().y );
		_Node.append_attribute("Z").set_value( rValueTyped.GetValue().z );
	}break;
	case zenConst::keAssProp_Int4:	{	
		zenAss::PropertyInt4::ValueRef rValueTyped(_rValue);
		_Node.append_attribute("X").set_value( rValueTyped.GetValue().x );
		_Node.append_attribute("Y").set_value( rValueTyped.GetValue().y );
		_Node.append_attribute("Z").set_value( rValueTyped.GetValue().z );
		_Node.append_attribute("W").set_value( rValueTyped.GetValue().w );
	}break;
	case zenConst::keAssProp_Enum:	{	
		zenAss::PropertyEnum::ValueRef rValueTyped(_rValue);
		const zenAss::PropertyEnum::Entry& enumEntry = rValueTyped.GetEnumEntry();
	}break;
	case zenConst::keAssProp_Struct:{	
		zenAss::PropertyStruct::ValueRef rValueTyped(_rValue);
		zenAss::PropertyStruct::ValueStorage& aValues = rValueTyped.GetValue();
		for(zUInt idx(0), count(aValues.Count()); idx<count; ++idx)
		{
			if( !aValues[idx].IsDefault() )
			{
				pugi::xml_node nodeChild = _Node.append_child(kzXmlName_Node_Property);
				nodeChild.append_attribute(kzXmlName_PropAtr_Name).set_value(aValues[idx].GetDefinition().mName.mzName);
				nodeChild.append_attribute(kzXmlName_PropAtr_Type).set_value(aValues[idx].GetDefinition().GetTypeName() );
				SavePropertyValue( aValues[idx], nodeChild );
			}
		}
	}break;
	case zenConst::keAssProp_Array:	{	
		zenAss::PropertyArray::ValueRef rValueTyped(_rValue);
		zenAss::PropertyArray::ValueStorage& aValues = rValueTyped.GetValue();
		_Node.append_attribute("Num").set_value( aValues.Count() );
		_Node.append_attribute("ElementType").set_value( rValueTyped.GetDefinition().mrArrayPropertyDef->GetTypeName() );
		for(zUInt idx(0), count(aValues.Count()); idx<count; ++idx)
		{
			if( !aValues[idx].IsDefault() )
			{
				pugi::xml_node nodeChild = _Node.append_child(kzXmlName_Node_Property);
				nodeChild.append_attribute("Index").set_value(idx+1);
				SavePropertyValue( aValues[idx], nodeChild );
			}			
		}
	}break;
	default:
		ZENAssertMsg(0, "Unsupported Asset Property type while loading.");
	}
}

bool AssetLoaderXml::LoadPackages()
{
	bool bResult(true);
	const zbFile::FileInfo* pFileInfo;
	zbMgr::File.Search( zbFile::keFileFlag_File, L"Packages", L"*.xml",  true);
	while( zbMgr::File.SearchNext(pFileInfo) )
	{
		bResult &= LoadPackage(pFileInfo->GetFilename());
	}
	return bResult;
}

bool AssetLoaderXml::LoadPackage(const zbFile::Filename& _Filename)
{
	char zName[128];
	//char zStorageName[256];
	wcstombs_s(NULL, zName, ZENArrayCount(zName), _Filename.GetNameNoExt(), ZENArrayCount(zName));
	//wcstombs_s(NULL, zStorageName, ZENArrayCount(zStorageName), _Filename.GetNameFull(), ZENArrayCount(zStorageName));	//! @todo Clean : Baaadddddd, need to fully use wchar (or not at all)
	 
	pugi::xml_document Doc;	
	pugi::xml_parse_result result = Doc.load_file(_Filename.GetNameFull());
	if( result )
	{
		// Load package infos
		pugi::xml_node nodePackage	= Doc.child(kzXmlName_Node_Package);
		if( nodePackage )
		{
			zeAss::Package* pNewPackage = zenNewDefault zeAss::Package;
			zU32 uPkgID					= nodePackage.attribute(kzXmlName_PkgAtr_ID).as_uint();
			const char* zGroup			= nodePackage.attribute(kzXmlName_PkgAtr_Group).as_string();
			zU32 uEngineVer				= nodePackage.attribute(kzXmlName_PkgAtr_Version).as_uint();
			if( uEngineVer<=zenConst::keEngineVersion__Current		&& 
				zeMgr::Asset.PackageGet(uPkgID).IsValid() == false	&& 
				pNewPackage->Init(uPkgID, zName, zGroup, _Filename.GetNameFull(), uEngineVer ) )
			{
				AddPackage(pNewPackage);
				for(pugi::xml_node nodeAsset = Doc.child(kzXmlName_Node_Asset); nodeAsset; nodeAsset = nodeAsset.next_sibling(kzXmlName_Node_Asset))
					LoadAsset(*pNewPackage, nodeAsset);
				return true;
			}
			else
			{
				zenDel( pNewPackage );
				//! @todo Log: Warning about package load failure
			}
		}
	}
	return false;
}

bool AssetLoaderXml::Save(zenAss::zPackage& _rPackage)
{
	zString zGroupName;
	zGroupName.Merge(_rPackage->GetGroupAndName(), '\\', zGroupName, -1 );
	
	pugi::xml_document Doc;	
	pugi::xml_node nodePackage = Doc.append_child(kzXmlName_Node_Package);
	nodePackage.append_attribute(kzXmlName_PkgAtr_ID).set_value( _rPackage->GetID() );
	nodePackage.append_attribute(kzXmlName_PkgAtr_Group).set_value( zGroupName );
	nodePackage.append_attribute(kzXmlName_PkgAtr_Version).set_value( zenConst::keEngineVersion__Current );
	for(zInt idxType(0); idxType<zenConst::keAssType__Count; ++idxType)
	{
		const zenAss::zArrayAsset& dAssets = _rPackage->GetAsset( static_cast<zenConst::eAssetType>(idxType) );
		zenAss::zArrayAsset::Iterator itAsset(dAssets);
		while( itAsset.IsValid() )
		{
			SaveAsset( itAsset.GetValue(), Doc );
			++itAsset;
		}		
	}
	
	//! @todo Clean : Baaadddddd, need to fully use wchar (or not at all)
	//! @todo Asset : Use temp file writing destination before overwriting	
//	wchar_t zStorageName[256];
//	mbstowcs_s(NULL, zStorageName, ZENArrayCount(zStorageName), _rPackage->GetStorageName(), _rPackage->GetStorageName().Len() );
	bool bResult = Doc.save_file( _rPackage->GetStorageName() );
	return bResult;
}

bool AssetLoaderXml::LoadAsset(zeAss::Package& _Package, const pugi::xml_node& _XmlNodeAsset)
{
	const char* zAssetType			= _XmlNodeAsset.attribute(kzXmlName_AssetAtr_Type).as_string();
	zU32 uID						= _XmlNodeAsset.attribute(kzXmlName_AssetAtr_ID).as_uint();
	zenConst::eAssetType eAssetType	= zenAss::AssetNameToType(zHash32(zAssetType));
	if( eAssetType < zenConst::keAssType__Count && !zeMgr::Asset.AssetGet(eAssetType, uID).IsValid() )
	{
		zeAss::Asset* pNewAsset = zeAss::Asset::CreateItem( eAssetType );
		if( pNewAsset )
		{
			// Load base asset infos
			
			const char* zAssetName	= _XmlNodeAsset.attribute(kzXmlName_AssetAtr_Name).as_string();
			const char* zAssetGroup	= _XmlNodeAsset.attribute(kzXmlName_AssetAtr_Group).as_string();
			pNewAsset->Init(uID, zAssetName, zAssetGroup, _Package);

			// Load properties values
			for (pugi::xml_node nodeProp = _XmlNodeAsset.child(kzXmlName_Node_Property); nodeProp; nodeProp = nodeProp.next_sibling(kzXmlName_Node_Property))
			{
				const char* zPropName	= nodeProp.attribute(kzXmlName_PropAtr_Name).as_string();				
				zInt valudIdx			= pNewAsset->GetValueIndex( zHash32(zPropName) );
				if( valudIdx >= 0 )
					LoadPropertyValue( pNewAsset->GetValue(valudIdx), nodeProp);
			}
			return true;
		}
	}

	//! @todo Log: Add info on why asset rejected (Id collision, invalid type?)
	return false;
}

bool AssetLoaderXml::SaveAsset(zenAss::zAssetItem _rAsset, pugi::xml_node& _XmlNodeDoc)
{
	zString zGroupName;
	zGroupName.Merge(_rAsset->GetGroupAndName(), '\\', zGroupName, -1 );

	pugi::xml_node nodeAsset = _XmlNodeDoc.append_child(kzXmlName_Node_Asset);
	nodeAsset.append_attribute(kzXmlName_AssetAtr_ID).set_value( _rAsset->GetID().muIndex );
	nodeAsset.append_attribute(kzXmlName_AssetAtr_Name).set_value( _rAsset->GetName() );
	nodeAsset.append_attribute(kzXmlName_AssetAtr_Type).set_value( zenAss::AssetTypeToString(_rAsset->GetType()) );		
	nodeAsset.append_attribute(kzXmlName_AssetAtr_Group).set_value( zGroupName );
	for(zInt idx(0), count(_rAsset->GetValueCount()); idx<count; ++idx)
	{
		zenAss::PropertyValueRef rPropertyValue = _rAsset->GetValue(idx);
		if( rPropertyValue.IsDefault() == false )
		{
			pugi::xml_node nodeProp	= nodeAsset.append_child(kzXmlName_Node_Property);
			nodeProp.append_attribute(kzXmlName_PropAtr_Name).set_value(rPropertyValue.GetDefinition().mName.mzName);
			nodeProp.append_attribute(kzXmlName_PropAtr_Type).set_value(rPropertyValue.GetDefinition().GetTypeName() );
			SavePropertyValue(rPropertyValue, nodeProp);
		}
	}

	return true;
}

}} //namespace zen { namespace zxAss

#endif //ZEN_ENGINETOOL
