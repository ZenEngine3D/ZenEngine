#include "zeEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss
{

PropertyDefRef PropertyArray::Create( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, bool _bIsEditable, const PropertyDefRef& _rPropertyDef, zUInt _uEntryCountMin, zUInt _uEntryCountMax )
{		
	static zenMem::zAllocatorPool sAllocPool( "PropertyDefinition::Create", sizeof(PropertyArray), 256, 256 );
	ZENAssert( _rPropertyDef.IsValid() );
	ZENAssert( _uEntryCountMin <= _uEntryCountMax);
	PropertyArray* pNewDefinition		= zenNew(&sAllocPool) zenAss::PropertyArray(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _bIsEditable);
	pNewDefinition->mrArrayPropertyDef	= _rPropertyDef;	
	pNewDefinition->muEntryCountMin		= _uEntryCountMin;
	pNewDefinition->muEntryCountMax		= _uEntryCountMax;
	pNewDefinition->mDefault.SetCount(_uEntryCountMin);
	for(zUInt idx(0); idx<_uEntryCountMin; ++idx)
		pNewDefinition->mDefault[idx] = _rPropertyDef->Allocate(nullptr);
	return pNewDefinition;
}

//! @detail Mostly the same logic as base class, but must assign default values 
//!			differently since the array contains ValueRef.
PropertyValueRef PropertyArray::Allocate(const zAssetItemRef& _rOwnerAsset)const
{	
	static zenMem::zAllocatorPool sAllocPool( "PropertyArray::Allocate", sizeof(ValueProperty), 256, 256 );
	//! @todo Clean : Removing const qualifier, since 'zGameConstRef' doesn't take a const reference at the moment...
	//					We know that all accessors won't modify the object, except for ReferenceAdd/Rem with mRerCount needing to be mutable.
	ValueRef rArrayVal = zenNew(&sAllocPool) ValueProperty(_rOwnerAsset, this);
	ValueStorage aArrayVal( mDefault.Count() );
	for(zUInt idx(0), count(mDefault.Count()); idx<count; ++idx)
		aArrayVal[idx] = mDefault[idx].Clone(_rOwnerAsset);

	rArrayVal = aArrayVal;
	return rArrayVal;
}

PropertyValueRef PropertyArray::Clone(const zAssetItemRef& _rOwnerAsset, const PropertyValueRef& _rValue)const
{
	ZENAssert( _rValue.GetType() == kPropertyType );
	static zenMem::zAllocatorPool sAllocPool( "PropertyArray::Clone", sizeof(ValueProperty), 256, 256 );	
	ValueRef rValueToClone				= _rValue;	
	ValueRef rValueCloned 				= zenNew(&sAllocPool) ValueProperty(rValueToClone->mrOwnerAsset, this);	
	const ValueStorage& aArrayToClone	= rValueToClone.GetValue();	
	ValueStorage aArrayCloned(aArrayToClone.Count());	
	for(zUInt idx(0), count(aArrayToClone.Count()); idx<count; ++idx)
		aArrayCloned[idx] = aArrayToClone[idx].Clone(_rValue->mrOwnerAsset);
	
	rValueCloned = aArrayCloned;
	return rValueCloned;
}

bool PropertyArray::IsDefault(const class PropertyValueRef& _ValueRef)const
{
	ValueRef rArrayValue			= _ValueRef;
	const ValueStorage& aArrayValue = rArrayValue.GetValue();
	bool bIsDefault = (aArrayValue.Count() == mDefault.Count());
	
	for(zUInt idx(0), count(aArrayValue.Count()); bIsDefault && idx<count; ++idx)
		bIsDefault = (mDefault[idx] == aArrayValue[idx]);
	
	return bIsDefault;
}

bool PropertyArray::IsEqual(const PropertyValueRef& _rValue1, const PropertyValueRef& _rValue2)const
{
	if( (_rValue1.GetType() == kPropertyType) && (_rValue1.GetType() == _rValue2.GetType()) )
	{
		ValueRef rArrayValue1				= _rValue1;
		ValueRef rArrayValue2				= _rValue2;
		const ValueStorage& aArrayValue1	= rArrayValue1.GetValue();
		const ValueStorage& aArrayValue2	= rArrayValue2.GetValue();		
		bool bEqual							= aArrayValue1.Count() == aArrayValue2.Count();
		for(zUInt idx(0), count(aArrayValue1.Count()); bEqual && idx<count; ++idx)
			bEqual = aArrayValue1[idx] == aArrayValue2[idx];

		return bEqual;	
	}
	return false;
}

PropertyValueRef PropertyArray::ValueRef::AddEntry()
{
	ZENAssert( IsValid() );
	const PropertyArray& Definition		= GetDefinition();
	PropertyValueRef newValue			= Definition.mrArrayPropertyDef->Allocate((*this)->mrOwnerAsset);
	ValueStorage& arrayValue			= static_cast<Value*>(GetSafe())->mValue;
	arrayValue.Push(newValue);
	return newValue;
}

void PropertyArray::ValueRef::RemEntry(const PropertyValueRef& _ToRemove)
{
	ZENAssert( IsValid() );
	ValueStorage& arrayValue = static_cast<Value*>(GetSafe())->mValue;
	for( zUInt idx(0), count(arrayValue.Count()); idx<count; ++idx)
	{
		if( arrayValue[idx] == _ToRemove )
		{
			for(zUInt idxCopy(idx); idxCopy+1<count; ++idxCopy)
				arrayValue[idxCopy] = arrayValue[idxCopy+1];
			arrayValue.Pop();
			break;
		}
	}
}

}} //namespace zen { namespace zenAss

#endif //ZEN_ENGINETOOL
