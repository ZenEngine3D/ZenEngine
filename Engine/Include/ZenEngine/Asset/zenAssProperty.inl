
#if ZEN_ENGINETOOL

#include <stdio.h> ///temp

namespace zen { namespace zenAss 
{
template<class TProperty>
ZENForceInline void* AllocateValue(const TProperty& _PropertyDef)
{
	static zenMem::zAllocatorPool sAllocPool( "PropertyPool", sizeof(TProperty::Data), 256, 256 );
	return zenNew(&sAllocPool) TProperty::Data( static_cast<const TProperty*>(&_PropertyDef)->mDefault );		
}

PropertyValue::PropertyValue()
: mpDefinition(NULL)
, mpValue(NULL)
{
}

const PropertyBase*	PropertyValue::GetProperty()const
{ 
	return mpDefinition;
}

zenConst::eAssetPropertyType PropertyValue::GetType() const					
{ 
	ZENAssert( mpDefinition && mpValue );
	return mpDefinition->meType;		
}

bool PropertyValue::IsDefault() const
{
	ZENAssert( mpDefinition && mpValue );
	return mpDefinition->IsDefault(mpValue);
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

zUInt PropertyValue::ToString(zUInt _uLen, char* _zOutString)const
{
	ZENAssert( mpDefinition && mpValue );
	return mpDefinition->ToString(mpValue, _uLen, _zOutString);
}

}} //namespace zen { namespace zenAss

#endif
