#include "zeEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss
{

PropertyDefRef PropertyEnum::Create( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, bool _bIsEditable, ValueStorage _Default, const Entry* _pEntries, zUInt _uEntryCount )
{		
	static zenMem::zAllocatorPool sAllocPool( "PropertyDefinition::Create", sizeof(PropertyEnum), 256, 256 );
	PropertyEnum* pNewDefinition	= zenNew(&sAllocPool) zenAss::PropertyEnum(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _bIsEditable);
	pNewDefinition->mDefault		= _Default;
	pNewDefinition->maEnumValues.Copy(_pEntries, _uEntryCount);
	pNewDefinition->mdKeyToIndex.Init(_uEntryCount*2);
	pNewDefinition->maValueToIndex.Init(_uEntryCount*2);
	for(zUInt idx(0); idx<_uEntryCount; ++idx)
	{
		pNewDefinition->mdKeyToIndex.Set(_pEntries[idx].mValueKey.mhName, idx);
		pNewDefinition->maValueToIndex.Set(_pEntries[idx].mValue, idx);
	}
	zenAssertMsg( pNewDefinition->maValueToIndex.Exist(pNewDefinition->mDefault), "Specified default value doesn't exist in Asset Property Definition");
	pNewDefinition->maValueToIndex.SetDefaultValue( pNewDefinition->maValueToIndex[_Default] );
	pNewDefinition->mdKeyToIndex.SetDefaultValue( pNewDefinition->maValueToIndex[_Default] );
	return pNewDefinition;
}

const PropertyEnum::Entry& PropertyEnum::GetEnumEntry(ValueStorage _Value)const
{
	return maEnumValues[ maValueToIndex[_Value] ];
}

const PropertyEnum::Entry& PropertyEnum::GetEnumEntry(zHash32 _hValue)const
{
	return maEnumValues[ mdKeyToIndex[_hValue] ];
}

PropertyEnum::Entry::Entry(ValueStorage _Value, const char* _zName, const char* _zDescription)
: mValue(_Value)
, mValueKey(_zName)
, mzDescription(_zDescription)
{
}

const PropertyEnum::Entry& PropertyEnum::ValueRef::GetEnumEntry()const
{
	return GetDefinition().GetEnumEntry( GetValue() );
}

const PropertyEnum::ValueRef& PropertyEnum::ValueRef::operator=(PropertyEnum::ValueStorage _uEnumValue)
{
	if( !GetDefinition().maValueToIndex.Exist(_uEnumValue)  )
		_uEnumValue = GetDefinition().mDefault;
	Super::operator=( _uEnumValue);
	return *this;
}

const PropertyEnum::ValueRef& PropertyEnum::ValueRef::operator=(zHash32 _hEnumName)
{
	const zenAss::PropertyEnum::Entry& enumEntry = GetDefinition().GetEnumEntry(_hEnumName);
	operator=(enumEntry.mValue);
	return *this;
}

const PropertyEnum::ValueRef& PropertyEnum::ValueRef::operator=(const char* _zEnumName)
{
	zHash32 hEnumName(_zEnumName);
	return operator=( hEnumName );
}

}} //namespace zen { namespace zenAss

#endif //ZEN_ENGINETOOL
