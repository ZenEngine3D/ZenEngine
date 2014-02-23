#include "libZenEngine.h"
#if AW_ENGINETOOL

namespace FAss
{	

Package::Package()
: madAssetPerType(AssetItem::keType__Count)
{
	for(awUInt typIdx(0), typCount(madAssetPerType.Count()); typIdx<typCount; ++typIdx)
	{
		madAssetPerType[typIdx].Init(50);
		madAssetPerType[typIdx].SetDefaultValue(NULL);
	}
}

Package::~Package()
{
	Unload();
}

void Package::Unload()
{	
	for(awUInt typIdx(0), typCount(madAssetPerType.Count()); typIdx<typCount; ++typIdx)
	{
		Hashmap<AssetItem*>::Key64::Iterator it;	
		madAssetPerType[typIdx].GetFirst(it);
		while( it.IsValid() )
		{
			AWDel(it.GetValue());
			++it;
		}			
		madAssetPerType[typIdx].Clear();
	}
	mPackagePath = L"";
	maGroup.SetCount(0);
}

bool Package::Load(const CFil::Filename& _Filename, Hashmap<AssetItem*>::Key64& _dAllAsset)
{	
	Unload();
	pugi::xml_document Doc;	

	char zcName[128];
	wcstombs(zcName,_Filename.GetNameNoExt(),sizeof(zcName));

	mID	= awHash32(_Filename.GetNameFull());		
	Doc.load_file(_Filename.GetNameFull());

	// Load package infos
	pugi::xml_node nodePackage	= Doc.child(kzXmlName_Node_Package);
	LoadGroupAndName(zcName, nodePackage ? nodePackage.attribute(kzXmlName_PkgAtr_Group).as_string() : "", maGroup );
	
	mhGroupID = awHash32("Package");
	for(int idx(0), count(maGroup.Count()-1); idx<count; ++idx )
		mhGroupID.Append( maGroup[idx] );
	
	// Load all assets
	for(pugi::xml_node nodeAsset = Doc.child(kzXmlName_Node_Asset); nodeAsset; nodeAsset = nodeAsset.next_sibling(kzXmlName_Node_Asset))
	{		
		const char* zAssetType			= nodeAsset.attribute(kzXmlName_AssetAtr_Type).as_string();		
		AssetItem::enumType eAssetType	= AssetItem::GetType(awHash32(zAssetType));
		if( eAssetType < AssetItem::keType__Count )
		{
			AssetItem* pNewAsset = AssetItem::CreateItem( eAssetType, *this );
			pNewAsset->Load( nodeAsset );
			madAssetPerType[eAssetType].Set(pNewAsset->GetID(), pNewAsset);
			_dAllAsset.Set(pNewAsset->GetID(), pNewAsset);
		}
	}

	//! @todo notify manager
	return true;
}

bool Package::Save()
{	
	// Save package infos
	pugi::xml_document Doc;
	pugi::xml_node nodePackage = Doc.append_child(kzXmlName_Node_Package);
	awString zGroup="";
	for(awUInt idx(0), count(maGroup.Count()-1); idx<count; ++idx)
	{
		zGroup += maGroup[idx];
		zGroup += "\\";
	}
	nodePackage.append_attribute(kzXmlName_PkgAtr_Group).set_value(static_cast<const char*>(zGroup));

	// Save assets infos
	for(awUInt typIdx(0), typCount(madAssetPerType.Count()); typIdx<typCount; ++typIdx)
	{
		Hashmap<AssetItem*>::Key64::Iterator it;	
		madAssetPerType[typIdx].GetFirst(it);
		while( it.IsValid() )
		{
			it.GetValue()->Save(Doc);
			++it;
		}			
	}

	// Save package
	CMgr::File.CreateDir( mPackagePath.GetPathFull() );
	Doc.save_file((const char*)mPackagePath.GetNameFull() ); //! @todo Secure version of this, with temp file
	
	return true;
}

AssetItem* Package::AssetGet(AssetItem::enumType _eType, awHash64 _hAssetName)
{
	return madAssetPerType[_eType][_hAssetName];
}
		
void LoadGroupAndName(const char* _zName, const char* _zGroup, awArrayStatic<awString>& _aOutGroup )
{
	if( _zGroup )
	{
		awUInt uGroupEnd[64];
		awUInt uGroupCount(0);
		awUInt pos(0);
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
		for(awUInt idx(0); idx<uGroupCount; ++idx)
		{
			char temp[128];
			awUInt len = zenMath::Min<awUInt>(AWArrayCount(temp), uGroupEnd[idx]-pos+1 );
			memcpy(temp, &_zGroup[pos], len);
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

}

#endif //AW_ENGINETOOL