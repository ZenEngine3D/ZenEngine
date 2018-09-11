
#if ZEN_ENGINETOOL

#include <stdio.h> ///temp

namespace zen { namespace zenAss 
{

//=============================================================================
// Templated Value
//=============================================================================
template<class TClassDefinition, class TClassValueStorage>
TPropertyValue<TClassDefinition,TClassValueStorage>::TPropertyValue(const zAssetItemRef& _rOwnerAsset, const PropertyDefRef& _rOwnerDefinition) 
: Super( _rOwnerAsset, _rOwnerDefinition)
{	
	zenAssert( _rOwnerDefinition->GetType() == TClassDefinition::kPropertyType);
}


//=============================================================================
// Templated Value Reference
//=============================================================================
template<class TClassDefinition, class TClassValueStorage>
TPropertyValueRef<TClassDefinition, TClassValueStorage>::TPropertyValueRef( const PropertyValueRef& _Copy )
: Super( _Copy )	
{			
	zenAssert( !_Copy.IsValid() || _Copy.GetType() == TClassDefinition::kPropertyType );
}

template<class TClassDefinition, class TClassValueStorage>
TPropertyValueRef<TClassDefinition, TClassValueStorage>& TPropertyValueRef<TClassDefinition, TClassValueStorage>::operator=( const TPropertyValueRef& _Copy )
{
	zenAssert( !_Copy.IsValid() || _Copy.GetType() == TClassDefinition::kPropertyType );
	Super::operator=( _Copy );
	return *this;
}

template<class TClassDefinition, class TClassValueStorage>
TPropertyValueRef<TClassDefinition, TClassValueStorage>& TPropertyValueRef<TClassDefinition, TClassValueStorage>::operator=( const TClassValueStorage& _Assign )
{
	zenAssert(IsValid());
	if( GetValue() != _Assign )
	{
		static_cast<Value*>(GetSafe())->mValue = _Assign;
		EmitPropertyChanged();			
	}	
	return *this;
}

template<class TClassDefinition, class TClassValueStorage>
const TClassValueStorage& TPropertyValueRef<TClassDefinition, TClassValueStorage>::GetValue()const
{
	return static_cast<const Value*>(GetSafe())->mValue;
}

template<class TClassDefinition, class TClassValueStorage>
const TClassDefinition& TPropertyValueRef<TClassDefinition, TClassValueStorage>::GetDefinition() const
{
	return *static_cast<const TClassDefinition*>( GetSafe()->mrOwnerDefinition.Get() );
}

//=============================================================================
// Templated PropertyDefinition
//=============================================================================
template<zenConst::eAssetPropertyType TPropertyType, class TClassDefinition, class TClassValue>
TPropertyDefinition<TPropertyType, TClassDefinition, TClassValue>::TPropertyDefinition(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, bool _bIsEditable)
: PropertyDefinition(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _bIsEditable)
{
}

template<zenConst::eAssetPropertyType TPropertyType, class TClassDefinition, class TClassValue>
PropertyValueRef TPropertyDefinition<TPropertyType, TClassDefinition, TClassValue>::Allocate(const zAssetItemRef& _rOwnerAsset) const
{	
	//static zenMem::zAllocatorPool sAllocPool( "TPropertyDefinition::Allocate", sizeof(ValueProperty), 256, 256 );
	ValueProperty* pValue	= zenNewPool ValueProperty(_rOwnerAsset, this); 
	pValue->mValue			= static_cast<const TClassDefinition*>(this)->mDefault;
	return pValue;
}

template<zenConst::eAssetPropertyType TPropertyType, class TClassDefinition, class TClassValue>
PropertyValueRef TPropertyDefinition<TPropertyType, TClassDefinition, TClassValue>::Clone(const PropertyValueRef& _rValue)const
{
	zenAssert( _rValue.GetType() == kPropertyType );
	ValueRef rValueToClone	= _rValue;
	ValueRef rValueCloned	= Allocate( rValueToClone->mrOwnerAsset );
	rValueCloned			= rValueToClone.GetValue();
	return rValueCloned;
}

template<zenConst::eAssetPropertyType TPropertyType, class TClassDefinition, class TClassValue>
bool TPropertyDefinition<TPropertyType, TClassDefinition, TClassValue>::IsDefault(const PropertyValueRef& _rValue)const
{
	zenAssert( _rValue.GetType() == kPropertyType );	
	ValueRef rValue = _rValue;
	return static_cast<const TClassDefinition*>(this)->mDefault == rValue.GetValue();	
}

template<zenConst::eAssetPropertyType TPropertyType, class TClassDefinition, class TClassValue>
bool TPropertyDefinition<TPropertyType, TClassDefinition, TClassValue>::IsEqual(const PropertyValueRef& _rValue1, const PropertyValueRef& _rValue2)const
{	
	zenAssert( _rValue1.GetType() == kPropertyType );
	if( _rValue1.GetType() == _rValue2.GetType() )
	{
		const ValueRef rValue1	= _rValue1;
		const ValueRef rValue2	= _rValue2;
		return rValue1.GetValue() == rValue2.GetValue();
	}
	return false;
}

template<zenConst::eAssetPropertyType TPropertyType, class TClassDefinition, class TClassValue>
zenConst::eAssetPropertyType TPropertyDefinition<TPropertyType, TClassDefinition, TClassValue>::GetType()const
{
	return TPropertyType;
}


}} //namespace zen { namespace zenAss

#endif
