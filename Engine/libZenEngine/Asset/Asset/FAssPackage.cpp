#include "libZenEngine.h"
#if AW_ENGINETOOL

namespace FAss
{	

Package::Package()
: madAssetPerType(AssetItem::keType__Count)
{
	for(zenUInt typIdx(0), typCount(madAssetPerType.Count()); typIdx<typCount; ++typIdx)
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
	for(zenUInt typIdx(0), typCount(madAssetPerType.Count()); typIdx<typCount; ++typIdx)
	{
		zenMap<AssetItem*>::Key64::Iterator it;	
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

bool Package::Load(const CFil::Filename& _Filename, zenMap<AssetItem*>::Key64& _dAllAsset)
{	
	Unload();
	pugi::xml_document Doc;	

	char zcName[128];
	wcstombs(zcName,_Filename.GetNameNoExt(),sizeof(zcName));

	mID	= zenHash32(_Filename.GetNameFull());		
	Doc.load_file(_Filename.GetNameFull());

	// Load package infos
	pugi::xml_node nodePackage	= Doc.child(kzXmlName_Node_Package);
	LoadGroupAndName(zcName, nodePackage ? nodePackage.attribute(kzXmlName_PkgAtr_Group).as_string() : "", maGroup );
	
	mhGroupID = zenHash32("Package");
	for(int idx(0), count(maGroup.Count()-1); idx<count; ++idx )
		mhGroupID.Append( maGroup[idx] );
	
	// Load all assets
	for(pugi::xml_node nodeAsset = Doc.child(kzXmlName_Node_Asset); nodeAsset; nodeAsset = nodeAsset.next_sibling(kzXmlName_Node_Asset))
	{		
		const char* zAssetType			= nodeAsset.attribute(kzXmlName_AssetAtr_Type).as_string();		
		AssetItem::enumType eAssetType	= AssetItem::GetType(zenHash32(zAssetType));
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
	zenString zGroup="";
	for(zenUInt idx(0), count(maGroup.Count()-1); idx<count; ++idx)
	{
		zGroup += maGroup[idx];
		zGroup += "\\";
	}
	nodePackage.append_attribute(kzXmlName_PkgAtr_Group).set_value(static_cast<const char*>(zGroup));

	// Save assets infos
	for(zenUInt typIdx(0), typCount(madAssetPerType.Count()); typIdx<typCount; ++typIdx)
	{
		zenMap<AssetItem*>::Key64::Iterator it;	
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

AssetItem* Package::AssetGet(AssetItem::enumType _eType, zenHash64 _hAssetName)
{
	return madAssetPerType[_eType][_hAssetName];
}
		
void LoadGroupAndName(const char* _zName, const char* _zGroup, zenArrayStatic<zenString>& _aOutGroup )
{
	if( _zGroup )
	{
		zenUInt uGroupEnd[64];
		zenUInt uGroupCount(0);
		zenUInt pos(0);
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
		for(zenUInt idx(0); idx<uGroupCount; ++idx)
		{
			char temp[128];
			zenUInt len = zenMath::Min<zenUInt>(AWArrayCount(temp), uGroupEnd[idx]-pos+1 );
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