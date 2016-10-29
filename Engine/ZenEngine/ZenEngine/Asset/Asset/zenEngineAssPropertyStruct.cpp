#include "zeEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss
{

PropertyDefRef PropertyStruct::Create( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, bool _bIsEditable, const PropertyDefRef* _prPropertyDef, zUInt _uPropertyDefCount )
{	
	zenAssert(_prPropertyDef && _uPropertyDefCount>0);
	static zenMem::zAllocatorPool sAllocPool( "PropertyDefinition::Create", sizeof(PropertyStruct), 256, 256 );
	PropertyStruct* pNewDefinition = zenNew(&sAllocPool) zenAss::PropertyStruct(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _bIsEditable);
	pNewDefinition->maPropertyDef.Copy(_prPropertyDef, _uPropertyDefCount);
	pNewDefinition->mDefault.SetCount(_uPropertyDefCount);
	pNewDefinition->mdPropertyDefIndex.Init(16);
	pNewDefinition->mdPropertyDefIndex.SetDefaultValue(0xFFFFFFFF);
	for(zUInt idx(0); idx<_uPropertyDefCount; ++idx)
	{
		zenAssert( _prPropertyDef[idx].IsValid() );
		pNewDefinition->mDefault[idx] = pNewDefinition->maPropertyDef[idx]->Allocate(nullptr);				
		pNewDefinition->mdPropertyDefIndex.Set(_prPropertyDef[idx]->mName.mhName, idx);
	}

	return pNewDefinition;
}

//! @detail Mostly the same logic as base class, but must assign default values 
//!			differently since the array contains ValueRef.
PropertyValueRef PropertyStruct::Allocate(const zAssetItemRef& _rOwnerAsset)const
{	
	static zenMem::zAllocatorPool sAllocPool( "PropertyArray::Allocate", sizeof(ValueProperty), 256, 256 );
	//! @todo Clean : Removing const qualifier, since 'zGameConstRef' doesn't take a const reference at the moment...
	//					We know that all accessors won't modify the object, except for ReferenceAdd/Rem with mRefCount needing to be mutable.
	ValueRef rStructVal = zenNew(&sAllocPool) ValueProperty(_rOwnerAsset, this);
	ValueStorage aStructVal;
	aStructVal.SetCount( mDefault.Count() );
	for(zUInt idx(0), count(mDefault.Count()); idx<count; ++idx)
		aStructVal[idx] = mDefault[idx].Clone(_rOwnerAsset);
	
	rStructVal = aStructVal;
	return rStructVal;
}

PropertyValueRef PropertyStruct::Clone(const PropertyValueRef& _rValue)const
{
	zenAssert( _rValue.GetType() == kPropertyType );
	static zenMem::zAllocatorPool sAllocPool( "PropertyArray::Clone", sizeof(ValueProperty), 256, 256 );	
	ValueRef rValueToClone				= _rValue;	
	ValueRef rValueCloned 				= zenNew(&sAllocPool) ValueProperty(rValueToClone->mrOwnerAsset, this);	
	const ValueStorage& aStructToClone	= rValueToClone.GetValue();	
	ValueStorage aStructCloned;
	aStructCloned.SetCount( aStructToClone.Count() );
	for(zUInt idx(0), count(aStructToClone.Count()); idx<count; ++idx)
		aStructCloned[idx] = aStructToClone[idx].Clone(_rValue->mrOwnerAsset);
	
	rValueCloned = aStructCloned;
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
	zenAssert( IsValid() );
	zUInt uIndex;
	const PropertyStruct& Definition = GetDefinition();
	if( Definition.mdPropertyDefIndex.Get(_hPropertyName, uIndex) )
		return GetValue()[uIndex];
	
	return nullptr;
}

}} //namespace zen { namespace zenAss
#else

zenPreventEmptyFileWarning

#endif //ZEN_ENGINETOOL
