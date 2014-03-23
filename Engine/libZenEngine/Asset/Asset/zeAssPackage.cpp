#include "libZenEngine.h"

#if ZEN_ENGINETOOL

#include <Engine/ThirdParty/pugixml/src/pugixml.hpp>	//! @todo Asset: Include properly

namespace zen { namespace zeAss
{	

//=================================================================================================
//! @brief		Load one Asset content
//! @details	This handle xml file loading, using pugi external library
//-------------------------------------------------------------------------------------------------
//! @param[in]		_InNodeAsset - Xml entry for asset
//! @param[in]		_pInPackage	- Package we're loading
//! @param[out]		_zOutGroup	- String for group this package should be put under
//! @return			true if successful
//=================================================================================================
bool AssetLoadXml(const pugi::xml_node& _InNodeAsset, Package& _ParentPkg, AssetItem& _InAsset)
{
	bool bResult(true);
	
	// Load base asset infos
	const char* zAssetName	= _InNodeAsset.attribute(kzXmlName_AssetAtr_Name).as_string();
	const char* zAssetGroup	= _InNodeAsset.attribute(kzXmlName_AssetAtr_Group).as_string();
	_InAsset.Init(_ParentPkg, zAssetName, zAssetGroup);
	
	// Load properties values
	for (pugi::xml_node nodeProp = _InNodeAsset.child(kzXmlName_Node_Property); nodeProp; nodeProp = nodeProp.next_sibling(kzXmlName_Node_Property))
	{
		const char* zPropName	= nodeProp.attribute(kzXmlName_PropAtr_Name).as_string();
		const char* zPropVal	= nodeProp.attribute(kzXmlName_PropAtr_Value).as_string();
		zInt valudIdx			= _InAsset.GetValueIndex( zHash32(zPropName) );
		if( valudIdx >= 0 && zPropVal && zPropVal[0] != 0)
			_InAsset.GetValue(valudIdx).FromXml(zPropVal);
	}
	return true;
}

//=================================================================================================
//! @brief		Load one package content
//! @details	This handle xml file loading, using pugi external library
//! @todo Optim: Async loading, priority given to AssetIDs
//-------------------------------------------------------------------------------------------------
//! @param[in]		_InFilename	- File to load
//! @param[in]		_pInPackage	- Package we're loading
//! @return			true if successful
//=================================================================================================
bool PackageLoadXml(const CFil::Filename& _InFilename, Package& _InPackage )
{
	pugi::xml_document Doc;	
	pugi::xml_parse_result result = Doc.load_file(_InFilename.GetNameFull());
	if( result )
	{
		// Load package infos
		pugi::xml_node nodePackage	= Doc.child(kzXmlName_Node_Package);
		if( nodePackage )
		{
			const char* zGroup = nodePackage.attribute(kzXmlName_PkgAtr_Group).as_string();
			_InPackage.Init(zGroup, _InFilename);
		
			// Load all assets inside that package
			for(pugi::xml_node nodeAsset = Doc.child(kzXmlName_Node_Asset); nodeAsset; nodeAsset = nodeAsset.next_sibling(kzXmlName_Node_Asset))
			{		
				const char* zAssetType			= nodeAsset.attribute(kzXmlName_AssetAtr_Type).as_string();		
				zenConst::eAssetType eAssetType	= zenAss::AssetNameToType(zHash32(zAssetType));
				if( eAssetType < zenConst::keAssType__Count )
				{
					AssetItem* pNewAsset = AssetItem::CreateItem( eAssetType );							
					if( !AssetLoadXml( nodeAsset, _InPackage, *pNewAsset) )
						zenDel(pNewAsset);
				}
			}
			return true;
		}
	}
	return false;
}

//=================================================================================================
//! @brief		Save package content to xml file
//! @details	This handle xml file saving, using pugi external library
//-------------------------------------------------------------------------------------------------
//! @param[in]		_InFilename	- File to save
//! @param[in]		_pInPackage	- Package we're loading
//! @param[out]		_zOutGroup	- String for group this package should be put under
//! @return			true if successful
//=================================================================================================
bool PackageSaveXml(const CFil::Filename& _InFilename, const Package* _pInPackage )
{	
	/*
	// Save package infos
	pugi::xml_document Doc;
	pugi::xml_node nodePackage = Doc.append_child(kzXmlName_Node_Package);
	zString zGroup="";
	for(zUInt idx(0), count(maGroup.Count()-1); idx<count; ++idx)
	{
		zGroup += maGroup[idx];
		zGroup += "\\";
	}
	nodePackage.append_attribute(kzXmlName_PkgAtr_Group).set_value(static_cast<const char*>(zGroup));

	// Save assets infos
	for(zUInt typIdx(0), typCount(madAssetPerType.Count()); typIdx<typCount; ++typIdx)
	{
		zMap<zenAss::zAssetItem>::Key64::Iterator it;	
		madAssetPerType[typIdx].GetFirst(it);
		while( it.IsValid() )
		{
			//SF @todo Asset: it.GetValue().Save(Doc);
			++it;
		}			
	}

	// Save package
	CMgr::File.CreateDir( mPackagePath.GetPathFull() );
	Doc.save_file((const char*)mPackagePath.GetNameFull() ); //! @todo Error: Secure version of this, with temp file
	*/
	return true;
}

//=================================================================================================
//! @brief		Generates an array with Group names and item name inserted into array
//! @details	Used by Packages and Assets, for their naming hierarchy
//-------------------------------------------------------------------------------------------------
//! @param[in]		_zName		- Item name
//! @param[in]		_zGroup		- Group name as a string (with each element separated by '\' 
//! @param[out]		_aOutGroup	- Array to store result (last element is item name)
//=================================================================================================
void ParseGroupAndName(const char* _zName, const char* _zGroup, zArrayStatic<zString>& _aOutGroup )
{
	if( _zGroup )
	{
		zUInt uGroupEnd[64];
		zUInt uGroupCount(0);
		zUInt pos(0);
		while( _zGroup[pos] != 0 )
		{
			if( _zGroup[pos] == '\\' )
				uGroupEnd[uGroupCount++] = pos;
			++pos;
		}
		if( pos > 0 && _zGroup[pos-1]!= '\\' )
			uGroupEnd[uGroupCount++] = pos;

		pos	= 0;
		_aOutGroup.SetCount(uGroupCount+1);
		for(zUInt idx(0); idx<uGroupCount; ++idx)
		{
			char temp[128];
			zUInt len = zenMath::Min<zUInt>(ZENArrayCount(temp), uGroupEnd[idx]-pos+1 );
			zenMem::Copy(temp, &_zGroup[pos], len);
			temp[len-1]		= 0;
			_aOutGroup[idx]	= temp;
			pos				= uGroupEnd[idx]+1;
		}
	}
	else
	{
		_aOutGroup.SetCount(1);
	}
	*_aOutGroup.Last() = _zName;
}

Package::Package()
: madAssetPerType(zenConst::keAssType__Count)
{
	for(zUInt typIdx(0), typCount(madAssetPerType.Count()); typIdx<typCount; ++typIdx)
	{
		madAssetPerType[typIdx].Init(50);
		madAssetPerType[typIdx].SetDefaultValue(NULL);
		madAssetPerType[typIdx].SetDeleteItemCB(ResetAssetReference);
	}
}

Package::~Package()
{
	//Unload();
	for(zUInt typIdx(0), typCount(madAssetPerType.Count()); typIdx<typCount; ++typIdx)
	{
		zMap<zenAss::zAssetItem>::Key64::Iterator it;	
		madAssetPerType[typIdx].GetFirst(it);
		while( it.IsValid() )
		{
			//! @todo Asset: Set package reference to null
			//zenDel(it.GetValue());
			++it;
		}			
		madAssetPerType[typIdx].Clear();
	}
	mFilename = L"";
	maGroup.SetCount(0);
}

void Package::Unload()
{	
	for(zUInt typIdx(0), typCount(madAssetPerType.Count()); typIdx<typCount; ++typIdx)
	{
		zMap<zenAss::zAssetItem>::Key64::Iterator it;	
		madAssetPerType[typIdx].GetFirst(it);
		while( it.IsValid() )
		{
			//! @todo Asset: Set package reference to null
			//zenDel(it.GetValue());
			++it;
		}			
		madAssetPerType[typIdx].Clear();
	}
	mFilename = L"";
	maGroup.SetCount(0);
}

bool Package::Load(const CFil::Filename& _Filename)
{	
	Unload();	
	return PackageLoadXml(_Filename, *this);
}

bool Package::Save(const CFil::Filename& _Filename)
{
	mFilename = _Filename;
	return PackageSaveXml(mFilename, this);
}

bool Package::Save()
{
	return PackageSaveXml(mFilename, this);
}

}} //namespace zen { namespace zeAss

#endif //ZEN_ENGINETOOL