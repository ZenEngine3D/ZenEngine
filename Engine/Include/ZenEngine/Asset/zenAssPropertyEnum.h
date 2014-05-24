#pragma once
#ifndef __zenEngine_Asset_PropertyEnum_h__
#define __zenEngine_Asset_PropertyEnum_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{

class PropertyEnum : public TPropertyDefinition<zenConst::keAssProp_Enum, PropertyEnum, zU32>
{
	ZENPropertyDefinitionDeclare( PropertyEnum)
	struct Entry 
	{ 
		Entry(){}
		Entry(ValueStorage _Value, const char* _zName, const char* _zDescription);
		ValueStorage			mValue; 
		zStringHash32			mValueKey;
		zString					mzDescription;
	};

	const Entry&				GetEnumEntry(ValueStorage _Value)const;
	const Entry&				GetEnumEntry(zHash32 _hValue)const;	
	static PropertyDefRef		Create( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, 
										ValueStorage _Default, const Entry* _pEntries, zUInt _uEntryCount );

	zArrayStatic<Entry>			maEnumValues;
protected:
	zMap<zUInt>::Key32			mdKeyToIndex;
	zArraySparse<zUInt>::Key32	maValueToIndex;
};

}} //namespace zen { namespace zenAss

#endif
#endif