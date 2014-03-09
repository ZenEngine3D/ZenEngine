#include "libZenEngine.h"

#if ZEN_ENGINETOOL

#define ZEN_ASSETPROPERTIES_EXPAND_CODE(_TypeName_)	zenStringHash32( ZENStringDefine(_TypeName_) ),
static const zenStringHash32 saPropertyName[]={ ZEN_ASSETPROPERTIES_EXPAND };
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

}} //namespace zen { namespace zenAss

#endif //ZEN_ENGINETOOL
