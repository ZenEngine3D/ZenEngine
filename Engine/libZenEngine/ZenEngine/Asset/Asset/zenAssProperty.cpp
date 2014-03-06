#include "libZenEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss
{

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
