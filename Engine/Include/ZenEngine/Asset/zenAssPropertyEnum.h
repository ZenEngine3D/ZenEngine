#pragma once
#ifndef __zenEngine_Asset_PropertyEnum_h__
#define __zenEngine_Asset_PropertyEnum_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{

class PropertyEnum : public TPropertyDefinition<zenConst::keAssProp_Enum, PropertyEnum, zU32>
{
ZENClassDeclare(PropertyEnum, TPropertyDefinition);	
public:	
	struct Entry 
	{ 
		Entry(){}
		Entry(ValueStorage _Value, const char* _zName, const char* _zDescription)
			: mValue(_Value)
			, mValueKey(_zName)
			, mzDescription(_zDescription)
		{
		}
		ValueStorage	mValue; 
		zStringHash32	mValueKey;
		zString			mzDescription;
	};

	PropertyEnum(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, ValueStorage _Default, const Entry* _pEntries, zUInt _uEntryCount )
		: TPropertyDefinition(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc)
		, mDefault(_Default)
	{
		maEnumValues.Copy(_pEntries, _uEntryCount);
		mdKeyToIndex.Init(_uEntryCount*2);
		maValueToIndex.Init(_uEntryCount*2);
		for(zUInt idx(0); idx<_uEntryCount; ++idx)
		{
			mdKeyToIndex.Set(_pEntries[idx].mValueKey.mhName, idx);
			maValueToIndex.Set(_pEntries[idx].mValue, idx);
		}
		ZENAssertMsg( maValueToIndex.Exist(mDefault), "Specified default value doesn't exist in Asset Property Definition");
		maValueToIndex.SetDefaultValue( maValueToIndex[mDefault] );
		mdKeyToIndex.SetDefaultValue( maValueToIndex[mDefault] );
	}

	const Entry& PropertyEnum::GetEnumEntry(ValueStorage _Value)const
	{
		return maEnumValues[ maValueToIndex[_Value] ];
	}

	const Entry& PropertyEnum::GetEnumEntry(zHash32 _hValue)const
	{
		return maEnumValues[ mdKeyToIndex[_hValue] ];
	}

	ValueStorage					mDefault;
	zArrayStatic<Entry>			maEnumValues;
protected:
	zMap<zUInt>::Key32			mdKeyToIndex;
	zArraySparse<zUInt>::Key32	maValueToIndex;
};

}} //namespace zen { namespace zenAss

#endif
#endif