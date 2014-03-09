
#if ZEN_ENGINETOOL

#include <stdio.h> ///temp

namespace zen { namespace zenAss 
{
template<class TProperty>
ZENForceInline void* AllocateValue(const TProperty& _PropertyDef)
{
	static zenMem::AllocatorPool sAllocPool( "PropertyPool", sizeof(TProperty::Data), 256, 256 );
	return zenNew(&sAllocPool) TProperty::Data( static_cast<const TProperty*>(&_PropertyDef)->mDefault );		
}

PropertyValue::PropertyValue()
: mpDefinition(NULL)
, mpValue(NULL)
{
}

zenConst::eAssetPropertyType PropertyValue::GetType() const					
{ 
	return mpDefinition->meType;		
}

const PropertyBase*	PropertyValue::GetProperty()const
{ 
	return mpDefinition;
}

void PropertyValue::Allocate(const PropertyBase& _PropertyDef)
{
	zenDelNull(mpValue);
	mpDefinition = &_PropertyDef;
	#define ZEN_ASSETPROPERTIES_EXPAND_CODE(_TypeName_)	case zenConst::keAssProp_##_TypeName_:	mpValue = AllocateValue( *static_cast<const Property##_TypeName_*>(mpDefinition) );	break;
	switch( _PropertyDef.meType )
	{
		ZEN_ASSETPROPERTIES_EXPAND	
	}
	#undef	ZEN_ASSETPROPERTIES_EXPAND_CODE
}

void PropertyValue::Reset()
{
	ZENAssert( mpDefinition && mpValue );
	#define ZEN_ASSETPROPERTIES_EXPAND_CODE(_TypeName_)	case zenConst::keAssProp_##_TypeName_:	*mpValue##_TypeName_ = static_cast<const Property##_TypeName_*>(mpDefinition)->mDefault; break;
	switch( mpDefinition->meType )
	{
		ZEN_ASSETPROPERTIES_EXPAND	
	}
	#undef	ZEN_ASSETPROPERTIES_EXPAND_CODE
}

zenUInt PropertyValue::ToString(zenUInt _zLen, char* _zOutString)const
{
	ZENAssert( mpDefinition && mpValue );
	#define ZEN_ASSETPROPERTIES_EXPAND_CODE(_TypeName_)	case zenConst::keAssProp_##_TypeName_: return static_cast<const Property##_TypeName_*>(mpDefinition)->ToString( *mpValue##_TypeName_, _zLen, _zOutString );
	switch( mpDefinition->meType )
	{
		ZEN_ASSETPROPERTIES_EXPAND	
	}
	#undef	ZEN_ASSETPROPERTIES_EXPAND_CODE
	return 0;
}

zenUInt PropertyValue::ToXml(zenUInt _zLen, char* _zOutString)const
{
	ZENAssert( mpDefinition && mpValue );
	char zValue[256];
	zenUInt uWritten(sizeof(zValue));

	#define ZEN_ASSETPROPERTIES_EXPAND_CODE(_TypeName_)	case zenConst::keAssProp_##_TypeName_: uWritten = static_cast<const Property##_TypeName_*>(mpDefinition)->ToXml( *mpValue##_TypeName_, sizeof(zValue), zValue ); break;
	switch( mpDefinition->meType )
	{
		ZEN_ASSETPROPERTIES_EXPAND	
	}
	#undef	ZEN_ASSETPROPERTIES_EXPAND_CODE

	ZENAssertMsg(uWritten <  sizeof(zValue), "Value string biffer than buffer, increase zValue buffer (or nothing written)");
	return ::sprintf_s(_zOutString, _zLen, "<Property Name=\"%s\" Type=\"%s\" Value=\"%s\" />", 
		mpDefinition->mName.mzName, 
		GetPropertyTypeName(mpDefinition->meType), 
		zValue);
	return 0;
}

PropertyBool::PropertyBool(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, bool _bDefault)
: PropertyBase(_zName, zenConst::keAssProp_Bool, _zDisplayName, _zDescription, _bShowInAssetDesc )
, mDefault(_bDefault)
{
}


zenUInt PropertyBool::ToString(const Data& _Value, zenUInt _zLen, char* _zOutString)const
{
	return ::sprintf_s(_zOutString, _zLen, "%s", zenConst::kzFalseTrue[_Value]);
}

zenUInt PropertyBool::ToXml(const Data& _Value, zenUInt _zLen, char* _zOutString)const
{
	return ::sprintf_s(_zOutString, _zLen, "%i", _Value);
}

zenUInt PropertyFile::ToString(const Data& _Value, zenUInt _zLen, char* _zOutString)const
{
	return ::sprintf_s(_zOutString, _zLen, "%s", (const char*)_Value);
}

zenUInt PropertyFile::ToXml(const Data& _Value, zenUInt _zLen, char* _zOutString)const
{
	return ::sprintf_s(_zOutString, _zLen, "%s", (const char*)_Value);
}

PropertyFile::PropertyFile(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, const char* _zDefault, const char* _zFileExt )
: PropertyBase(_zName, zenConst::keAssProp_File, _zDisplayName, _zDescription, _bShowInAssetDesc )
, mDefault(_zDefault)
, mzFileExt(_zFileExt)
{
}

}} //namespace zen { namespace zenAss

#endif
