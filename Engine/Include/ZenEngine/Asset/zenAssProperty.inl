
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
	Get() = GetDefinition().mDefault;
}

template<class TClassDefinition, class TClassValueStorage>
TPropertyValueRef<TClassDefinition, TClassValueStorage>& TPropertyValueRef<TClassDefinition, TClassValueStorage>::operator=( const TPropertyValueRef& _Copy )
{
	ZENAssert( !_Copy.IsValid() || _Copy.GetType() == TClassDefinition::kPropertyType );
	Super::operator=( _Copy );
	return *this;
}

template<class TClassDefinition, class TClassValueStorage>
TClassValueStorage& TPropertyValueRef<TClassDefinition, TClassValueStorage>::Get()
{
	ZENAssert(IsValid());
	return static_cast<Value*>(mpReference)->mValue;
}

template<class TClassDefinition, class TClassValueStorage>
const TClassValueStorage& TPropertyValueRef<TClassDefinition, TClassValueStorage>::Get()const
{
	ZENAssert(IsValid());
	return static_cast<const Value*>(zGameRefConst::Get())->mValue;
}

template<class TClassDefinition, class TClassValueStorage>
const TClassDefinition& TPropertyValueRef<TClassDefinition, TClassValueStorage>::GetDefinition() const
{
	ZENAssert(IsValid());
	return *static_cast<const TClassDefinition*>( zGameRefConst::Get()->mrDefinition.Get() );
}

//=============================================================================
// Templated PropertyDefinition
//=============================================================================
template<zenConst::eAssetPropertyType TPropertyType, class TClassDefinition, class TClassValue>
TPropertyDefinition<TPropertyType, TClassDefinition, TClassValue>::TPropertyDefinition(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc)
: PropertyDefinition(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc)
{
}

template<zenConst::eAssetPropertyType TPropertyType, class TClassDefinition, class TClassValue>
PropertyValueRef TPropertyDefinition<TPropertyType, TClassDefinition, TClassValue>::Allocate() const
{
	typedef TPropertyValue<TClassDefinition, TClassValue> Value;
	static zenMem::zAllocatorPool sAllocPool( "TPropertyDefinition::Allocate", sizeof(Value), 256, 256 );
	//! @todo Clean : Removing const qualifier, since 'zGameRefConst' doesn't take a const reference at the moment...
	//					We know that all accessor won't modify the object, except for ReferenceAdd/Rem with mRerCount needing to be mutable.
	return zenNew(&sAllocPool) Value((PropertyDefinition*)this); 
}

template<zenConst::eAssetPropertyType TPropertyType, class TClassDefinition, class TClassValue>
bool TPropertyDefinition<TPropertyType, TClassDefinition, TClassValue>::IsDefault(const PropertyValueRef& _rValueRef)const
{
	ValueRef rValue = _rValueRef;
	return static_cast<const TClassDefinition*>(this)->mDefault == rValue.Get();
}

template<zenConst::eAssetPropertyType TPropertyType, class TClassDefinition, class TClassValue>
zenConst::eAssetPropertyType TPropertyDefinition<TPropertyType, TClassDefinition, TClassValue>::GetType()const
{
	return TPropertyType;
}


}} //namespace zen { namespace zenAss

#endif
