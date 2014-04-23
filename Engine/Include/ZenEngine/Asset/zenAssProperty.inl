
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

//=============================================================================================
// Property : Vector
//=============================================================================================
template<class TDataType>
PropertyVector<TDataType>::PropertyVector(zenConst::eAssetPropertyType _eType, const char* _zFmtString, const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, TDataType _vDefault, TDataType _vInc, TDataType _vMin, TDataType _vMax )
: PropertyBase(_zName, _eType, _zDisplayName, _zDescription, _bShowInAssetDesc )
, mDefault(_vDefault)
, mValMin(_vMin)
, mValMax(_vMax)
, mValInc(_vInc)
{
}

template<class TDataType>
zUInt PropertyVector<TDataType>::ToString(const void* _pValue, zUInt _zLen, char* _zOutString)const
{
	const TDataType* pValue = static_cast<const TDataType*>(_pValue);
	return ::sprintf_s(_zOutString, _zLen, mzFmtString, pValue->values[0], pValue->values[1], pValue->values[2], pValue->values[3]);
}

template<class TDataType>
bool PropertyVector<TDataType>::IsDefault( const void* _pValue )const
{
	const TDataType* pValue = static_cast<const TDataType*>(_pValue);
	return *pValue == mDefault;
}

}} //namespace zen { namespace zenAss

#endif
