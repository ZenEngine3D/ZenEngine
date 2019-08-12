#include "zeEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss
{

PropertyDefRef PropertyStruct::Create( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, bool _bIsEditable, const PropertyDefRef* _prPropertyDef, zUInt _uPropertyDefCount )
{	
	zenAssert(_prPropertyDef && _uPropertyDefCount>0);
	PropertyStruct* pNewDefinition = zenMem::NewPool<zenAss::PropertyStruct>(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _bIsEditable);
	pNewDefinition->maPropertyDef.Copy(_prPropertyDef, _uPropertyDefCount);
	pNewDefinition->mDefault.resize(_uPropertyDefCount);
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
	//! @todo Clean : Removing const qualifier, since 'zGameConstRef' doesn't take a const reference at the moment...
	//					We know that all accessors won't modify the object, except for ReferenceAdd/Rem with mRefCount needing to be mutable.
	ValueRef rStructVal = zenMem::NewPool<ValueProperty>(_rOwnerAsset, this);
	ValueStorage aStructVal;
	aStructVal.resize( mDefault.size() );
	for(zUInt idx(0), count(mDefault.size()); idx<count; ++idx)
		aStructVal[idx] = mDefault[idx].Clone(_rOwnerAsset);
	
	rStructVal = aStructVal;
	return rStructVal;
}

PropertyValueRef PropertyStruct::Clone(const PropertyValueRef& _rValue)const
{
	zenAssert( _rValue.GetType() == kPropertyType );
	ValueRef rValueToClone				= _rValue;	
	ValueRef rValueCloned 				= zenMem::NewPool<ValueProperty>(rValueToClone->mrOwnerAsset, this);	
	const ValueStorage& aStructToClone	= rValueToClone.GetValue();	
	ValueStorage aStructCloned;
	aStructCloned.resize( aStructToClone.size() );
	for(zUInt idx(0), count(aStructToClone.size()); idx<count; ++idx)
		aStructCloned[idx] = aStructToClone[idx].Clone(_rValue->mrOwnerAsset);
	
	rValueCloned = aStructCloned;
	return rValueCloned;
}

bool PropertyStruct::IsDefault(const class PropertyValueRef& _ValueRef)const
{
	ValueRef rArrayValue			= _ValueRef;
	const ValueStorage& aArrayValue = rArrayValue.GetValue();
	bool bIsDefault = (aArrayValue.size() == mDefault.size());

	for(zUInt idx(0), count(aArrayValue.size()); bIsDefault && idx<count; ++idx)
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
		bool bEqual							= aArrayValue1.size() == aArrayValue2.size();
		for(zUInt idx(0), count(aArrayValue1.size()); bEqual && idx<count; ++idx)
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
