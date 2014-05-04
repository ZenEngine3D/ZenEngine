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

bool AssetLoaderXml::LoadPackages()
{
	bool bResult(true);
	const zbFile::FileInfo* pFileInfo;
	CMgr::File.Search( zbFile::keFileFlag_File, L"Packages", L"*.xml",  true);
	while( CMgr::File.SearchNext(pFileInfo) )
	{
		bResult &= LoadPackage(pFileInfo->GetFilename());
	}
	return bResult;
}

bool AssetLoaderXml::LoadPackage(const zbFile::Filename& _Filename)
{
	char zName[128];
	char zStorageName[256];
	wcstombs(zName, _Filename.GetNameNoExt(), sizeof(zName));
	wcstombs(zStorageName, _Filename.GetNameFull(), sizeof(zStorageName));	//! @todo Clean : Baaadddddd, need to fully use wchar (or not at all)

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
			if( pNewPackage->Init(uPkgID, zName, zGroup, zStorageName, uEngineVer ) )
			{
				AddPackage(pNewPackage);
				for(pugi::xml_node nodeAsset = Doc.child(kzXmlName_Node_Asset); nodeAsset; nodeAsset = nodeAsset.next_sibling(kzXmlName_Node_Asset))
					LoadAsset(*pNewPackage, nodeAsset);
				return true;
			}
			else
			{
				zenDel( pNewPackage );
			}
		}
	}
	return false;
}

bool AssetLoaderXml::Save(zenAss::zPackage& _rPackage)
{
	//! @todo Optim: Quick discardable alloc (instead of costly default allocator)
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
	//! @todo Asset: remove this test
	bool bResult = Doc.save_file(L"E:\\TestPackage.xml");
	return bResult;
}

bool AssetLoaderXml::LoadAsset(zeAss::Package& _Package, const pugi::xml_node& _XmlNodeAsset)
{
	const char* zAssetType			= _XmlNodeAsset.attribute(kzXmlName_AssetAtr_Type).as_string();		
	zenConst::eAssetType eAssetType	= zenAss::AssetNameToType(zHash32(zAssetType));
	if( eAssetType < zenConst::keAssType__Count )
	{
		zeAss::Asset* pNewAsset = zeAss::Asset::CreateItem( eAssetType );
		if( pNewAsset )
		{
			// Load base asset infos
			zU32 uID				= _XmlNodeAsset.attribute(kzXmlName_AssetAtr_ID).as_uint();
			const char* zAssetName	= _XmlNodeAsset.attribute(kzXmlName_AssetAtr_Name).as_string();
			const char* zAssetGroup	= _XmlNodeAsset.attribute(kzXmlName_AssetAtr_Group).as_string();
			pNewAsset->Init(uID, zAssetName, zAssetGroup, _Package);

			// Load properties values
			for (pugi::xml_node nodeProp = _XmlNodeAsset.child(kzXmlName_Node_Property); nodeProp; nodeProp = nodeProp.next_sibling(kzXmlName_Node_Property))
			{
				//! @todo Asset: Load straight, instead of string conversion
				const char* zPropName	= nodeProp.attribute(kzXmlName_PropAtr_Name).as_string();				
				zInt valudIdx			= pNewAsset->GetValueIndex( zHash32(zPropName) );
				if( valudIdx >= 0 )
				{
					zenAss::PropertyValue&	assetValue	= pNewAsset->GetValue(valudIdx);
					switch( assetValue.GetType() )
					{
					case zenConst::keAssProp_Bool:	assetValue.GetValueBool()	= nodeProp.attribute("Value").as_bool(); break;
					case zenConst::keAssProp_Float:	assetValue.GetValueFloat()	= nodeProp.attribute("Value").as_float(); break;
					case zenConst::keAssProp_File:	assetValue.GetValueFile()	= nodeProp.attribute("Value").as_string(); break;
					default:
						ZENAssertMsg(0, "Unsupported Asset Property type while saving.");
					}
				}
			}
			return true;
		}
	}

	return false;
}

bool AssetLoaderXml::SaveAsset(zenAss::zAssetItem _rAsset, pugi::xml_node& _XmlNodeDoc)
{
	zString zGroupName;
	zGroupName.Merge(_rAsset->GetGroupAndName(), '\\', zGroupName, -1 );

	pugi::xml_node nodeAsset = _XmlNodeDoc.append_child(kzXmlName_Node_Asset);
	nodeAsset.append_attribute(kzXmlName_AssetAtr_ID).set_value( _rAsset->GetID() );
	nodeAsset.append_attribute(kzXmlName_AssetAtr_Name).set_value( _rAsset->GetName() );
	nodeAsset.append_attribute(kzXmlName_AssetAtr_Type).set_value( zenAss::AssetTypeToString(_rAsset->GetType()) );		
	nodeAsset.append_attribute(kzXmlName_AssetAtr_Group).set_value( zGroupName );
	for(zInt idx(0), count(_rAsset->GetValueCount()); idx<count; ++idx)
	{
		zenAss::PropertyValue& assetValue	= _rAsset->GetValue(idx);
		if( assetValue.IsDefault() == false )
		{
			pugi::xml_node nodeProp			= nodeAsset.append_child(kzXmlName_Node_Property);
			nodeProp.append_attribute(kzXmlName_PropAtr_Name).set_value(assetValue.GetProperty()->mName.mzName);			
			switch( assetValue.GetType() )
			{
			case zenConst::keAssProp_Bool:	nodeProp.append_attribute("Value").set_value(assetValue.GetValueBool()); break;
			case zenConst::keAssProp_Float:	nodeProp.append_attribute("Value").set_value(assetValue.GetValueFloat()); break;
			case zenConst::keAssProp_File:	nodeProp.append_attribute("Value").set_value(assetValue.GetValueFile()); break;
			default:
				ZENAssertMsg(0, "Unsupported Asset Property type while saving.");
			}
		}
	}

	return true;
}

}} //namespace zen { namespace zxAss

#endif //ZEN_ENGINETOOL
