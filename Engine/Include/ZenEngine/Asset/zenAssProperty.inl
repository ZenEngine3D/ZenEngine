
#if ZEN_ENGINETOOL

#include <stdio.h> ///temp

namespace zen { namespace zenAss 
{

//=============================================================================
// Templated Value
//=============================================================================
template<class TClassDefinition, class TClassValueStorage>
TPropertyValue<TClassDefinition,TClassValueStorage>::TPropertyValue(const PropertyDefRef& _rParent) 
: Super(_rParent)
{	
	ZENAssert( _rParent->GetType() == TClassDefinition::kPropertyType);
}


//=============================================================================
// Templated Value Reference
//=============================================================================
template<class TClassDefinition, class TClassValueStorage>
TPropertyValueRef<TClassDefinition, TClassValueStorage>::TPropertyValueRef( const PropertyValueRef& _Copy )
: Super( _Copy )	
{			
	ZENAssert( !_Copy.IsValid() || _Copy.GetType() == TClassDefinition::kPropertyType );
}

template<class TClassDefinition, class TClassValueStorage>
TPropertyValueRef<TClassDefinition, TClassValueStorage>& TPropertyValueRef<TClassDefinition, TClassValueStorage>::operator=( const TPropertyValueRef& _Copy )
{
	ZENAssert( !_Copy.IsValid() || _Copy.GetType() == TClassDefinition::kPropertyType );
	Super::operator=( _Copy );
	return *this;
}

template<class TClassDefinition, class TClassValueStorage>
TPropertyValueRef<TClassDefinition, TClassValueStorage>& TPropertyValueRef<TClassDefinition, TClassValueStorage>::operator=( const TClassValueStorage& _Assign )
{
	ZENAssert( IsValid() );
	GetValue() = _Assign;
	return *this;
}

template<class TClassDefinition, class TClassValueStorage>
TClassValueStorage& TPropertyValueRef<TClassDefinition, TClassValueStorage>::GetValue()
{
	ZENAssert(IsValid());
	return static_cast<Value*>(mpReference)->mValue;
}

template<class TClassDefinition, class TClassValueStorage>
const TClassValueStorage& TPropertyValueRef<TClassDefinition, TClassValueStorage>::GetValue()const
{
	ZENAssert(IsValid());
	return static_cast<const Value*>(Get())->mValue;
}

template<class TClassDefinition, class TClassValueStorage>
const TClassDefinition& TPropertyValueRef<TClassDefinition, TClassValueStorage>::GetDefinition() const
{
	ZENAssert(IsValid());
	return *static_cast<const TClassDefinition*>( Get()->mrDefinition.Get() );
}

//=============================================================================
// Templated PropertyDefinition
//=============================================================================
template<zenConst::eAssetPropertyType TPropertyType, class TClassDefinition, class TClassValue>
TPropertyDefinition<TPropertyType, TClassDefinition, TClassValue>::TPropertyDefinition(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, bool _bIsEditable)
: PropertyDefinition(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _bIsEditable)
//, mpValueChangedCB(NULL)
{
}

template<zenConst::eAssetPropertyType TPropertyType, class TClassDefinition, class TClassValue>
PropertyValueRef TPropertyDefinition<TPropertyType, TClassDefinition, TClassValue>::Allocate() const
{	
	static zenMem::zAllocatorPool sAllocPool( "TPropertyDefinition::Allocate", sizeof(ValueProperty), 256, 256 );
	//! @todo Clean : Removing const qualifier, since 'zGameRefConst' doesn't take a const reference at the moment...
	//					We know that all accessor won't modify the object, except for ReferenceAdd/Rem with mRefCount needing to be mutable.
	ValueProperty* pValue	= zenNew(&sAllocPool) ValueProperty((PropertyDefinition*)this); 
	pValue->mValue			= static_cast<const TClassDefinition*>(this)->mDefault;
	return pValue;
}

template<zenConst::eAssetPropertyType TPropertyType, class TClassDefinition, class TClassValue>
PropertyValueRef TPropertyDefinition<TPropertyType, TClassDefinition, TClassValue>::Clone(const PropertyValueRef& _rValue)const
{
	ZENAssert( _rValue.GetType() == kPropertyType );
	ValueRef rValueToClone	= _rValue;
	ValueRef rValueCloned	= Allocate();
	rValueCloned.GetValue()	= rValueToClone.GetValue();
	return rValueCloned;
}

template<zenConst::eAssetPropertyType TPropertyType, class TClassDefinition, class TClassValue>
bool TPropertyDefinition<TPropertyType, TClassDefinition, TClassValue>::IsDefault(const PropertyValueRef& _rValue)const
{
	ZENAssert( _rValue.GetType() == kPropertyType );	
	ValueRef rValue = _rValue;
	return static_cast<const TClassDefinition*>(this)->mDefault == rValue.GetValue();	
}

template<zenConst::eAssetPropertyType TPropertyType, class TClassDefinition, class TClassValue>
bool TPropertyDefinition<TPropertyType, TClassDefinition, TClassValue>::IsEqual(const PropertyValueRef& _rValue1, const PropertyValueRef& _rValue2)const
{	
	ZENAssert( _rValue1.GetType() == kPropertyType );
	if( _rValue1.GetType() == _rValue2.GetType() )
	{
		ValueRef rValue1 = _rValue1;
		ValueRef rValue2 = _rValue2;
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
