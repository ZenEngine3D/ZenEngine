#include "libZenEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss
{

PropertyDefRef PropertyStruct::Create( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, bool _bIsEditable, const PropertyDefRef* _prPropertyDef, zUInt _uPropertyDefCount )
{	
	ZENAssert(_prPropertyDef && _uPropertyDefCount>0);
	static zenMem::zAllocatorPool sAllocPool( "PropertyDefinition::Create", sizeof(PropertyStruct), 256, 256 );
	PropertyStruct* pNewDefinition = zenNew(&sAllocPool) zenAss::PropertyStruct(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _bIsEditable);
	pNewDefinition->maPropertyDef.Copy(_prPropertyDef, _uPropertyDefCount);
	pNewDefinition->mDefault.SetCount(_uPropertyDefCount);
	pNewDefinition->mdPropertyDefIndex.Init(16);
	pNewDefinition->mdPropertyDefIndex.SetDefaultValue(0xFFFFFFFF);
	for(zUInt idx(0); idx<_uPropertyDefCount; ++idx)
	{
		ZENAssert( _prPropertyDef[idx].IsValid() );
		pNewDefinition->mDefault[idx] = pNewDefinition->maPropertyDef[idx]->Allocate();				
		pNewDefinition->mdPropertyDefIndex.Set(_prPropertyDef[idx]->mName.mhName, idx);
	}

	return pNewDefinition;
}

//! @detail Mostly the same logic as base class, but must assign default values 
//!			differently since the array contains ValueRef.
PropertyValueRef PropertyStruct::Allocate()const
{	
	static zenMem::zAllocatorPool sAllocPool( "PropertyArray::Allocate", sizeof(ValueProperty), 256, 256 );
	//! @todo Clean : Removing const qualifier, since 'zGameRefConst' doesn't take a const reference at the moment...
	//					We know that all accessor won't modify the object, except for ReferenceAdd/Rem with mRerCount needing to be mutable.
	ValueRef rArrayVal		= zenNew(&sAllocPool) ValueProperty((PropertyDefinition*)this);
	ValueStorage& aArrayVal = rArrayVal.GetValue();
	aArrayVal.SetCount( mDefault.Count() );
	for(zUInt idx(0), count(mDefault.Count()); idx<count; ++idx)
		aArrayVal[idx] = mDefault[idx].Clone();

	return rArrayVal;
}

PropertyValueRef PropertyStruct::Clone(const PropertyValueRef& _rValue)const
{
	ZENAssert( _rValue.GetType() == kPropertyType );
	static zenMem::zAllocatorPool sAllocPool( "PropertyArray::Clone", sizeof(ValueProperty), 256, 256 );	
	ValueRef rValueCloned 				= zenNew(&sAllocPool) ValueProperty((PropertyDefinition*)this);
	ValueRef rValueToClone				= _rValue;	
	const ValueStorage& aArrayToClone	= rValueToClone.GetValue();	
	ValueStorage& aArrayCloned			= rValueCloned.GetValue();
	aArrayCloned.SetCount( aArrayToClone.Count() );
	for(zUInt idx(0), count(aArrayToClone.Count()); idx<count; ++idx)
		aArrayCloned[idx] = aArrayToClone[idx].Clone();

	return rValueCloned;
}

bool PropertyStruct::IsDefault(const class PropertyValueRef& _ValueRef)const
{
	ValueRef rArrayValue			= _ValueRef;
	const ValueStorage& aArrayValue = rArrayValue.GetValue();
	bool bIsDefault = (aArrayValue.Count() == mDefault.Count());

	for(zUInt idx(0), count(aArrayValue.Count()); bIsDefault && idx<count; ++idx)
		bIsDefault = (mDefault[idx] == aArrayValue[idx]);

	return bIsDefault;
}

bool PropertyStruct::IsEqual(const PropertyValueRef& _rValue1, const PropertyValueRef& _rValue2)const
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

PropertyValueRef PropertyStruct::ValueRef::GetStructValue(zHash32 _hPropertyName)
{
	ZENAssert( IsValid() );
	zUInt uIndex;
	const PropertyStruct& Definition = GetDefinition();
	if( Definition.mdPropertyDefIndex.Get(_hPropertyName, uIndex) )
		return GetValue()[uIndex];
	
	return NULL;
}


}} //namespace zen { namespace zenAss

#endif //ZEN_ENGINETOOL
