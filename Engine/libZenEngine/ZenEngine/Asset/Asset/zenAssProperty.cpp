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
PropertyBool::PropertyBool(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, Data _Default)
: PropertyBase(_zName, zenConst::keAssProp_Bool, _zDisplayName, _zDescription, _bShowInAssetDesc )
, mDefault(_Default)
{
}

zUInt PropertyBool::ToString(const void* _pValue, zUInt _zLen, char* _zOutString)const
{
	const Data* pValue = static_cast<const Data*>(_pValue);
	return ::sprintf_s(_zOutString, _zLen, "%s", zenConst::kzFalseTrue[*pValue]);
}

bool PropertyBool::IsDefault( const void* _pValue )const
{
	const Data* pValue = static_cast<const Data*>(_pValue);
	return *pValue == mDefault;
}

//=================================================================================================
// PROPERTY FLOAT
//=================================================================================================
PropertyFloat::PropertyFloat(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, float _Default, float _Inc, float _Min, float _Max )
: PropertyBase(_zName, zenConst::keAssProp_Float, _zDisplayName, _zDescription, _bShowInAssetDesc )
, mDefault(_Default)
, mValMin(_Min)
, mValMax(_Max)
, mValInc(_Inc)
{
}

zUInt PropertyFloat::ToString(const void* _pValue, zUInt _zLen, char* _zOutString)const
{
	const Data* pValue = static_cast<const Data*>(_pValue);
	return ::sprintf_s(_zOutString, _zLen, "%f", *pValue);
}

bool PropertyFloat::IsDefault( const void* _pValue )const
{
	const Data* pValue = static_cast<const Data*>(_pValue);
	return *pValue == mDefault;
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

bool PropertyFile::IsDefault( const void* _pValue )const
{
	const Data* pValue = static_cast<const Data*>(_pValue);
	return *pValue == mDefault;
}

}} //namespace zen { namespace zenAss

#endif //ZEN_ENGINETOOL
