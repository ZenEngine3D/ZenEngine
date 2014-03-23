#include "libZenEngine.h"

#if ZEN_ENGINETOOL

#define ZEN_ASSETPROPERTIES_EXPAND_CODE(_TypeName_)	zStringHash32( ZENStringDefine(_TypeName_) ),
static const zStringHash32 saPropertyName[]={ ZEN_ASSETPROPERTIES_EXPAND };
#undef	ZEN_ASSETPROPERTIES_EXPAND_CODE

namespace zen { namespace zenAss
{
const char* GetPropertyTypeName(zenConst::eAssetPropertyType _eType)
{
	ZENAssert( _eType < zenConst::keAssProp__Count);
	return saPropertyName[_eType].mzName;
}

PropertyBase::PropertyBase( const char* _zName, zenConst::eAssetPropertyType _eType, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc )
: mName(_zName)	
, meType(_eType)
, mzDisplayName( _zDisplayName && _zDisplayName[0] != NULL ? _zDisplayName : _zName )
, mzDescription(_zDescription)
, mbShowInAssetDesc(_bShowInAssetDesc)
{
}

PropertyValue::~PropertyValue()
{
	zenDelNull(mpValue);
}

//=================================================================================================
// PROPERTY BOOL
//=================================================================================================
PropertyBool::PropertyBool(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, bool _bDefault)
: PropertyBase(_zName, zenConst::keAssProp_Bool, _zDisplayName, _zDescription, _bShowInAssetDesc )
, mDefault(_bDefault)
{
}

zUInt PropertyBool::ToString(const void* _pValue, zUInt _zLen, char* _zOutString)const
{
	const Data* pValue = static_cast<const Data*>(_pValue);
	return ::sprintf_s(_zOutString, _zLen, "%s", zenConst::kzFalseTrue[*pValue]);
}

zUInt PropertyBool::ToXml(const void* _pValue, zUInt _zLen, char* _zOutString)const
{
	const Data* pValue = static_cast<const Data*>(_pValue);
	return ::sprintf_s(_zOutString, _zLen, "%i", *pValue);
}

void PropertyBool::FromXml(const char* _zValue, void* _pOutValue)const
{
	Data* pValue = static_cast<Data*>(_pOutValue);
	*pValue = atoi(_zValue);
}

//=================================================================================================
// PROPERTY FILE
//=================================================================================================
PropertyFile::PropertyFile(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, const char* _zDefault, const char* _zFileExt )
: PropertyBase(_zName, zenConst::keAssProp_File, _zDisplayName, _zDescription, _bShowInAssetDesc )
, mDefault(_zDefault)
, mzFileExt(_zFileExt)
{
}

zUInt PropertyFile::ToString(const void* _pValue, zUInt _zLen, char* _zOutString)const
{
	const Data* pValue = static_cast<const Data*>(_pValue);
	return ::sprintf_s(_zOutString, _zLen, "%s", (const char*)pValue);
}

zUInt PropertyFile::ToXml(const void* _pValue, zUInt _zLen, char* _zOutString)const
{
	const Data* pValue = static_cast<const Data*>(_pValue);
	return ::sprintf_s(_zOutString, _zLen, "%s", (const char*)pValue);
}

void PropertyFile::FromXml(const char* _zValue, void* _pOutValue)const
{
	Data* pValue = static_cast<Data*>(_pOutValue);
	*pValue = _zValue;
}

}} //namespace zen { namespace zenAss

#endif //ZEN_ENGINETOOL
