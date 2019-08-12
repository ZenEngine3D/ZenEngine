#include "zxExternal.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zxAss
{
const wchar_t kzPackagePath[]			= zenT("Package");

const char* kzXmlName_Node_Package		= "Package";
const char* kzXmlName_PkgAtr_ID			= "ID";
const char* kzXmlName_PkgAtr_Version	= "EngineVersion";

const char* kzXmlName_Node_Asset		= "Asset";
const char* kzXmlName_AssetAtr_ID		= "ID";
const char* kzXmlName_AssetAtr_Name		= "Name";
const char* kzXmlName_AssetAtr_Type		= "Type";
const char* kzXmlName_AssetAtr_Version	= "Version";

const char* kzXmlName_Node_Property		= "Property";
const char* kzXmlName_PropAtr_Name		= "Name";
const char* kzXmlName_PropAtr_Type		= "Type";
const char* kzXmlName_PropAtr_Value		= "Value";


void LoadPropertyValue( const zenAss::PropertyValueRef& _rValue, const pugi::xml_node& _Node )
{
	//Makes sure saved property is of expected type
	if( strcmp(_rValue.GetDefinition().GetTypeName(), _Node.attribute(kzXmlName_PropAtr_Type).as_string()) != 0 )
		return;

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
		mbstowcs_s(nullptr, zFilename, zenArrayCount(zFilename), _Node.attribute("Value").as_string(), zenArrayCount(zFilename));
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
	case zenConst::keAssProp_Asset:	{
		zenAss::PropertyAsset::ValueRef rValueTyped(_rValue);
		zenAss::PropertyAsset::ValueStorage	AssignValue;
		const char* zAssetType			= _Node.attribute("AssetType").as_string();
		AssignValue.meType				= zenAss::AssetNameToType(zHash32(zAssetType));
		AssignValue.muIndex				= _Node.attribute("AssetID").as_uint();
		rValueTyped						= AssignValue;
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
		zenAss::PropertyArray::ValueStorage aArrayVal( (zU32)uCount );
		for (pugi::xml_node _NodeChild = _Node.child(kzXmlName_Node_Property); _NodeChild; _NodeChild = _NodeChild.next_sibling(kzXmlName_Node_Property))
		{
			zInt idx = _NodeChild.attribute("Index").as_int()-1;
			if( idx >= 0 && idx < zInt(uCount) )
				LoadPropertyValue( aArrayVal[idx], _NodeChild);
		}
		rValueTyped = aArrayVal;
	}break;
	default:
		zenAssertMsg(0, "Unsupported Asset Property type while loading.");
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
		wcstombs_s(nullptr, zFilename, zenArrayCount(zFilename), rValueTyped.GetValue(), zenArrayCount(zFilename));
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
	case zenConst::keAssProp_Asset:	{
		zenAss::PropertyAsset::ValueRef rValueTyped(_rValue);
		_Node.append_attribute("AssetType").set_value( zenAss::AssetTypeToString(rValueTyped.GetValue().meType) );
		_Node.append_attribute("AssetID").set_value( (unsigned int)rValueTyped.GetValue().muIndex );
	}break;
	case zenConst::keAssProp_Struct:{	
		zenAss::PropertyStruct::ValueRef rValueTyped(_rValue);
		const zenAss::PropertyStruct::ValueStorage& aValues = rValueTyped.GetValue();
		for(zUInt idx(0), count(aValues.size()); idx<count; ++idx)
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
		const zenAss::PropertyArray::ValueStorage& aValues = rValueTyped.GetValue();
		_Node.append_attribute("Num").set_value((unsigned int)aValues.size());
		_Node.append_attribute("ElementType").set_value( rValueTyped.GetDefinition().mrArrayPropertyDef->GetTypeName() );
		for(zUInt idx(0), count(aValues.size()); idx<count; ++idx)
		{
			if( !aValues[idx].IsDefault() )
			{
				pugi::xml_node nodeChild = _Node.append_child(kzXmlName_Node_Property);
				nodeChild.append_attribute("Index").set_value((unsigned int)idx+1);
				SavePropertyValue( aValues[idx], nodeChild );
			}			
		}
	}break;
	default:
		zenAssertMsg(0, "Unsupported Asset Property type while loading.");
	}
}


bool AssetLoaderXml::LoadPackages()
{
	bool bResult(true);
	//zbFile::Filename file = kzPackagePath;
	return LoadGroup(zeMgr::Asset.GroupGetRoot(), kzPackagePath);
}

bool AssetLoaderXml::LoadGroup(const zenAss::zPackageGroupRef& _rParent, const zbFile::Filename& _Filename)
{
	bool bResult(true);
	zArrayDyn<zbFile::FileInfo> dirList;
	zArrayDyn<zbFile::FileInfo> fileList;
	
	zbMgr::File.Search(dirList, zbFile::keFileFlag_Dir, _Filename.GetNameFull(), zenT("*"), false);	
	for( zUInt idx(0), count(dirList.size()); idx<count; ++idx)
	{
		zenAss::zPackageGroupRef rNewParent	= zeMgr::Asset.GroupCreate(dirList[idx].GetFilename().GetName(), _rParent );		
		bResult								&= LoadGroup(rNewParent, dirList[idx].GetFilename());
		rNewParent->SetStorageInfo( dirList[idx].GetFilename().GetNameFull() );
	}

	zbMgr::File.Search(fileList, zbFile::keFileFlag_File, _Filename.GetNameFull(), zenT("*.xml"), false);
	for (zUInt idx(0), count(fileList.size()); idx < count; ++idx)
		bResult &= LoadPackage(_rParent, fileList[idx].GetFilename());

	return bResult;
}

bool AssetLoaderXml::LoadPackage(const zenAss::zPackageGroupRef& _rParent, const zbFile::Filename& _Filename)
{
	pugi::xml_document Doc;	
	pugi::xml_parse_result result = Doc.load_file(_Filename.GetNameFull());
	if( result )
	{
		// Load package infos
		pugi::xml_node nodePackage	= Doc.child(kzXmlName_Node_Package);
		if( nodePackage )
		{
			zenAss::zPackageRef rNewPackage	= zenMem::New<zeAss::Package>();
			zU32 uPkgID						= nodePackage.attribute(kzXmlName_PkgAtr_ID).as_uint();
			zU32 uEngineVer					= nodePackage.attribute(kzXmlName_PkgAtr_Version).as_uint();
			if( uEngineVer<=zenConst::keEngineVersion__Current		&& 
				zeMgr::Asset.PackageGet(uPkgID).IsValid() == false	&& 
				rNewPackage->Init(uPkgID, _Filename.GetNameNoExt(), _rParent, uEngineVer ) )
			{
				rNewPackage->SetStorageInfo(_Filename.GetNameFull());
				AddPackage(rNewPackage);
				for(pugi::xml_node nodeAsset = Doc.child(kzXmlName_Node_Asset); nodeAsset; nodeAsset = nodeAsset.next_sibling(kzXmlName_Node_Asset))
					LoadAsset( rNewPackage, nodeAsset);
				return true;
			}
			else
			{
				//! @todo Log: Warning about package load failure
			}
		}
	}
	return false;
}

bool AssetLoaderXml::Save(const zenAss::zPackageRef& _rPackage)
{
	pugi::xml_document Doc;	
	pugi::xml_node nodePackage = Doc.append_child(kzXmlName_Node_Package);
	nodePackage.append_attribute(kzXmlName_PkgAtr_ID).set_value( _rPackage->GetID() );
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
	
	// Find path to package file (based on group name hierarchy)
	zWString zNewFilename;
	GetGroupFilename(zNewFilename, _rPackage.GetParentGroup());
	zNewFilename					+= zenT("/");
	zNewFilename					+= _rPackage->GetName();
	zNewFilename					+= zenT(".xml");
	zWString zTempFilename			= zNewFilename;
	zTempFilename					+= L".tmp";
	zbFile::Filename tempFilename	= zTempFilename;
	zbFile::Filename oldFilename	= _rPackage->GetStorageInfo();
	zbFile::Filename newFilename	=  zNewFilename;
	
	// Save the package (and remove previous file)	
	zbMgr::File.Delete( tempFilename );
	zbMgr::File.CreateDir( tempFilename, true );
	if( Doc.save_file( tempFilename.GetNameFull() ) )
	{
		zbMgr::File.Delete( oldFilename );
		if( zbMgr::File.Rename(tempFilename, newFilename) )
		{
			_rPackage->SetStorageInfo(zNewFilename);
			return true;
		}			
	}
	return false;
}

bool AssetLoaderXml::Save(const zenAss::zPackageGroupRef& _rGroup)
{
	zWString zGroupFilename;
	GetGroupFilename(zGroupFilename, _rGroup);
	zbFile::Filename groupFilename = zGroupFilename;
	
	if( groupFilename.GetNameFull() != _rGroup->GetStorageInfo() )
	{
		if( _rGroup->GetStorageInfo() != zenT("") )
		{
			zbFile::Filename oldDirName = _rGroup->GetStorageInfo();
			zbMgr::File.Delete( oldDirName );
		}
		zbMgr::File.CreateDir( groupFilename, false );
		_rGroup->SetStorageInfo(zGroupFilename);
	}
	
	return true;
}

bool AssetLoaderXml::Delete(const zenAss::zPackageGroupRef& _rGroup)
{
	if( _rGroup.IsValid() && _rGroup->GetStorageInfo() != zenT(""))
		return zbMgr::File.Delete( zbFile::Filename(_rGroup->GetStorageInfo()) );
	
	return true;
}

bool AssetLoaderXml::Delete(const zenAss::zPackageRef& _rPackage)
{
	if( _rPackage.IsValid() )
		return zbMgr::File.Delete(zbFile::Filename(_rPackage->GetStorageInfo()));

	return true;
}

bool AssetLoaderXml::LoadAsset(const zenAss::zPackageRef& _rPackage, const pugi::xml_node& _XmlNodeAsset)
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
			pNewAsset->Init(zenAss::zAssetID(eAssetType, uID), zAssetName, _rPackage);

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

bool AssetLoaderXml::SaveAsset( const zenAss::zAssetItemRef& _rAsset, pugi::xml_node& _XmlNodeDoc)
{
	pugi::xml_node nodeAsset = _XmlNodeDoc.append_child(kzXmlName_Node_Asset);
	nodeAsset.append_attribute(kzXmlName_AssetAtr_ID).set_value( _rAsset->GetID().muIndex );
	nodeAsset.append_attribute(kzXmlName_AssetAtr_Name).set_value( _rAsset->GetName() );
	nodeAsset.append_attribute(kzXmlName_AssetAtr_Type).set_value( zenAss::AssetTypeToString(_rAsset->GetType()) );		
	for(zInt idx(0), count(_rAsset->GetValuesize()); idx<count; ++idx)
	{
		zenAss::PropertyValueRef rPropertyValue = _rAsset->GetValue(idx);
		if( rPropertyValue.IsDefault() == false )
		{
			pugi::xml_node nodeProp	= nodeAsset.append_child(kzXmlName_Node_Property);
			nodeProp.append_attribute(kzXmlName_PropAtr_Name).set_value(rPropertyValue.GetDefinition().mName.mzName);
			nodeProp.append_attribute(kzXmlName_PropAtr_Type).set_value(rPropertyValue.GetDefinition().GetTypeName());
			SavePropertyValue(rPropertyValue, nodeProp);
		}
	}

	return true;
}

void AssetLoaderXml::GetGroupFilename(zWString& _zFilenameOut, const zenAss::zPackageGroupRef& _rGroup)
{
	zWString zTemp;
	zenAss::zPackageGroupRef rGroupCur			= _rGroup;
	const zenAss::zPackageGroupRef& rGroupRoot	= zeMgr::Asset.GroupGetRoot();
	_zFilenameOut								= zenT("");
	while (rGroupCur.IsValid() && rGroupCur != rGroupRoot)
	{
		zTemp = zenT("/");
		zTemp += rGroupCur.GetName();
		_zFilenameOut.Prepend( zTemp  );
		rGroupCur = rGroupCur.GetParentGroup();
	}
	_zFilenameOut.Prepend(kzPackagePath);
}

}} //namespace zen { namespace zxAss

#else

zenPreventEmptyFileWarning

#endif //ZEN_ENGINETOOL
