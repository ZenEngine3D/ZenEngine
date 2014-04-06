#include "libZenExternal.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zxAss
{
const char* kzXmlName_Node_Package		= "Package";
const char* kzXmlName_PkgAtr_Group		= "Group";

const char* kzXmlName_Node_Asset		= "Asset";
const char* kzXmlName_AssetAtr_Name		= "Name";
const char* kzXmlName_AssetAtr_Group	= "Group";
const char* kzXmlName_AssetAtr_Type		= "Type";

const char* kzXmlName_Node_Property		= "Property";
const char* kzXmlName_PropAtr_Name		= "Name";
const char* kzXmlName_PropAtr_Type		= "Type";
const char* kzXmlName_PropAtr_Value		= "Value";

bool AssetLoaderXml::Load()
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
	pugi::xml_document Doc;	
	pugi::xml_parse_result result = Doc.load_file(_Filename.GetNameFull());
	if( result )
	{
		// Load package infos
		pugi::xml_node nodePackage	= Doc.child(kzXmlName_Node_Package);
		if( nodePackage )
		{
			zeAss::Package* pNewPackage = zenNewDefault zeAss::Package;
			const char* zGroup			= nodePackage.attribute(kzXmlName_PkgAtr_Group).as_string();
			pNewPackage->Init(zGroup, _Filename);

			// Load all assets inside that package
			for(pugi::xml_node nodeAsset = Doc.child(kzXmlName_Node_Asset); nodeAsset; nodeAsset = nodeAsset.next_sibling(kzXmlName_Node_Asset))
			{		
				LoadAsset(*pNewPackage, nodeAsset);
			}
			AddPackage(pNewPackage);
			return true;
		}
	}
	return false;
}

bool AssetLoaderXml::LoadAsset(zeAss::Package& _Package, const pugi::xml_node& _AssetXmlNode)
{
	const char* zAssetType			= _AssetXmlNode.attribute(kzXmlName_AssetAtr_Type).as_string();		
	zenConst::eAssetType eAssetType	= zenAss::AssetNameToType(zHash32(zAssetType));
	if( eAssetType < zenConst::keAssType__Count )
	{
		zeAss::Asset* pNewAsset = zeAss::Asset::CreateItem( eAssetType );
		if( pNewAsset )
		{
			// Load base asset infos
			const char* zAssetName	= _AssetXmlNode.attribute(kzXmlName_AssetAtr_Name).as_string();
			const char* zAssetGroup	= _AssetXmlNode.attribute(kzXmlName_AssetAtr_Group).as_string();
			pNewAsset->Init(_Package, zAssetName, zAssetGroup);

			// Load properties values
			for (pugi::xml_node nodeProp = _AssetXmlNode.child(kzXmlName_Node_Property); nodeProp; nodeProp = nodeProp.next_sibling(kzXmlName_Node_Property))
			{
				const char* zPropName	= nodeProp.attribute(kzXmlName_PropAtr_Name).as_string();
				const char* zPropVal	= nodeProp.attribute(kzXmlName_PropAtr_Value).as_string();
				zInt valudIdx			= pNewAsset->GetValueIndex( zHash32(zPropName) );
				if( valudIdx >= 0 && zPropVal && zPropVal[0] != 0)
					pNewAsset->GetValue(valudIdx).FromXml(zPropVal);
			}

			return true;
		}
	}

	return false;
}

bool AssetLoaderXml::Save()
{
	return true;
}

bool AssetLoaderXml::Save(zenAss::zPackage& _rPackage)
{
	return true;
}

bool AssetLoaderXml::SavePackage(const zeAss::Package& _Package)
{
	return true;
}

bool AssetLoaderXml::SaveAsset()
{
	return true;
}

}} //namespace zen { namespace zxAss

#endif //ZEN_ENGINETOOL
