#include "libZenEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zeAss
{

//=================================================================================================
//! @brief		Factory to create a new Asset of specified type
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param		_eAssetType	- Type of asset to create
//! @param		_Owner		- Owner package of the newly create Asset
//! @return					- Created Asset
//=================================================================================================
Asset* Asset::CreateItem( zenConst::eAssetType _eAssetType )
{
	Asset* pNewItem(NULL);
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
	}
	return pNewItem;
}

Asset::Asset()	
: mrPackage(NULL)
, muID(0)
{
	static zU32 sCounter(1);
	muID = sCounter++; //! @todo Asset: fix this (HACK)	
}

Asset::~Asset()	
{
}

void Asset::Init(zU32 _uID, const char* _zName, const char* _zGroup, Package& _ParentPkg)
{	
	zString::Split(_zGroup, '\\', maGroup, 1);
	*maGroup.Last() = _zName;

	muID		= _uID != 0 ? _uID :  zeMgr::Asset.GetAssetNextID( GetType() ); 
	mrPackage	= &_ParentPkg;

	SetPackage( &_ParentPkg );
	zeMgr::Asset.AssetAdd(this);
	//! @todo Asset : description init
	//RebuiltDescription();
}

void Asset::SetPackage(Package* _pParentPkg)
{
	if( mrPackage.IsValid() )
		mrPackage->AssetRem(*this);
	
	mrPackage = _pParentPkg;
	if( _pParentPkg )
		_pParentPkg->AssetAdd(*this);
}

//=================================================================================================
//! @brief		Build a string description based on asset property definitions/values
//! @details	Take all flagged properties and make a string description from it. Used for
//!				Asset short description in editor
//-------------------------------------------------------------------------------------------------
//! @return		
//=================================================================================================
void Asset::RebuiltDescription()
{	
	mzDescription = "";
	//! @TODO
	/*
	for(zUInt idx(0), count(maPropertyValueOld.Count()); idx<count; ++idx)
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

void Asset::InitDefault()
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
		const zenAss::zArrayProperty& aProperties = GetProperties();	
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
bool Asset::InitPropertyMap(zMap<zInt>::Key32& _dPropertyMap)const
{	
	const zenAss::zArrayProperty& aPropertyDef = GetProperties();
	_dPropertyMap.Init(aPropertyDef.Count()*2);
	_dPropertyMap.SetDefaultValue(-1);
	for(zInt idx(0), count(aPropertyDef.Count()); idx<count; ++idx)
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
zInt TestProperty::GetValueIndex(zHash32 _hPropertyName)const	
{
	static zMap<zInt>::Key32 sdPropertyIndex;
	static bool sbInit = InitPropertyMap(sdPropertyIndex);	//!< suEntryCount only used to init hashtable once
	return sdPropertyIndex[_hPropertyName];
}

const zenAss::zArrayProperty& TestProperty::GetProperties()const
{ 	
	//zMap<zU32>::Key32					mdPropertyIndex; todo
	static const zenAss::PropertyBool	Property00("PropertyBool",		"", "Property test: Bool",	true,	false);		
	static const zenAss::PropertyFloat	Property01("PropertyFloat",		"", "Property test: Float",	true,	0, 0.1, -10, 10);		
	static const zenAss::PropertyFile	Property02("PropertyFile",		"", "Property test: File",	true,	"C:\\temp\\test.txt", "Images|*.bmp;*.png;*.jpeg;*.jpg|BMP(*.bmp)|*.bmp|PNG(*.png)|*.png|JPEG(*.jpg;*.jpeg)|*.jpg;*.jpeg");
	static const zenAss::PropertyBase*	aPropertiesAll[] = {&Property00, &Property01, &Property02 };
	static zenAss::zArrayProperty		saProperties( aPropertiesAll, ZENArrayCount(aPropertiesAll) );
	return saProperties;
}

}} //namespace zen { namespace zeAss

#endif //ZEN_ENGINETOOL