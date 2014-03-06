#if ZEN_ENGINETOOL
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
	return mpDefinition->GetType();		
}

const PropertyBase*	PropertyValue::GetProperty()
{ 
	return mpDefinition;
}

void PropertyValue::Allocate(const PropertyBase& _PropertyDef)
{
	zenDelNull(mpValue);
	mpDefinition = &_PropertyDef;
	#define ZEN_ASSETPROPERTIES_EXPAND_CODE(_TypeName_)	case zenConst::keAssProp_##_TypeName_:	AllocateValue( *static_cast<const Property##_TypeName_*>(mpDefinition) );	break;
	switch( _PropertyDef.GetType() )
	{
		ZEN_ASSETPROPERTIES_EXPAND	
	}
	#undef	ZEN_ASSETPROPERTIES_EXPAND_CODE
}

void PropertyValue::Reset()
{
	ZENAssert( mpDefinition && mpValue );
	//#define ZEN_ASSETPROPERTIES_EXPAND_CODE(_TypeName_)	
	switch( mpDefinition->GetType() )
	{
		case zenConst::keAssProp_Bool:	*mpValueBool = static_cast<const PropertyBool*>(mpDefinition)->mDefault; break;
		case zenConst::keAssProp_File:	*mpValueFile = static_cast<const PropertyFile*>(mpDefinition)->mDefault; break;
		//ZEN_ASSETPROPERTIES_EXPAND	
	}
}

}} //namespace zen { namespace zenAss

#endif
