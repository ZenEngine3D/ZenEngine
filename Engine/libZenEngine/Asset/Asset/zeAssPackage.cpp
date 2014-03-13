#include "libZenEngine.h"
#if ZEN_ENGINETOOL

namespace zen { namespace zeAss
{	

Package::Package()
: madAssetPerType(zenConst::keAssType__Count)
{
	for(zUInt typIdx(0), typCount(madAssetPerType.Count()); typIdx<typCount; ++typIdx)
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
	for(zUInt typIdx(0), typCount(madAssetPerType.Count()); typIdx<typCount; ++typIdx)
	{
		zMap<zenAss::zAssetItem>::Key64::Iterator it;	
		madAssetPerType[typIdx].GetFirst(it);
		while( it.IsValid() )
		{
			//! @todo remove reference zenDel(it.GetValue());
			++it;
		}			
		madAssetPerType[typIdx].Clear();
	}
	mPackagePath = L"";
	maGroup.SetCount(0);
}

bool Package::Load(const CFil::Filename& _Filename, zMap<AssetItem*>::Key64& _dAllAsset)
{	
	Unload();
	pugi::xml_document Doc;	

	char zcName[128];
	wcstombs(zcName,_Filename.GetNameNoExt(),sizeof(zcName));

	mID	= zHash32(_Filename.GetNameFull());		
	Doc.load_file(_Filename.GetNameFull());

	// Load package infos
	pugi::xml_node nodePackage	= Doc.child(kzXmlName_Node_Package);
	LoadGroupAndName(zcName, nodePackage ? nodePackage.attribute(kzXmlName_PkgAtr_Group).as_string() : "", maGroup );
	
	mhGroupID = zHash32("Package");
	for(int idx(0), count(maGroup.Count()-1); idx<count; ++idx )
		mhGroupID.Append( maGroup[idx] );
	
	// Load all assets
	for(pugi::xml_node nodeAsset = Doc.child(kzXmlName_Node_Asset); nodeAsset; nodeAsset = nodeAsset.next_sibling(kzXmlName_Node_Asset))
	{		
		const char* zAssetType			= nodeAsset.attribute(kzXmlName_AssetAtr_Type).as_string();		
		zenConst::eAssetType eAssetType	= AssetItem::GetTypeFromName(zHash32(zAssetType));
		if( eAssetType < zenConst::keAssType__Count )
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
			//SF @todo it.GetValue().Save(Doc);
			++it;
		}			
	}

	// Save package
	CMgr::File.CreateDir( mPackagePath.GetPathFull() );
	Doc.save_file((const char*)mPackagePath.GetNameFull() ); //! @todo Secure version of this, with temp file
	
	return true;
}

	
void LoadGroupAndName(const char* _zName, const char* _zGroup, zArrayStatic<zString>& _aOutGroup )
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

}} //namespace zen { namespace zeAss

#endif //ZEN_ENGINETOOL