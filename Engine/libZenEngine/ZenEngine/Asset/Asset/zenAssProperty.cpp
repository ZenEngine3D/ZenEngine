#include "libZenEngine.h"

#if ZEN_ENGINETOOL

#define ZEN_ASSETPROPERTIES_EXPAND_CODE(_TypeName_)	zStringHash32( ZENStringDefine(_TypeName_) ),
static const zStringHash32 saPropertyName[]={ ZEN_ASSETPROPERTIES_EXPAND };
#undef	ZEN_ASSETPROPERTIES_EXPAND_CODE

namespace zen { namespace zenAss
{
const char* GetPropertyTypeName(zenConst::eAssetPropertyType _eType)
{
	ZENAssert( _eType < zenConst::keAssProp__Count);
	return saPropertyName[_eType].mzName;
}

PropertyBase::PropertyBase( const char* _zName, zenConst::eAssetPropertyType _eType, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc )
: mName(_zName)	
, meType(_eType)
, mzDisplayName( _zDisplayName && _zDisplayName[0] != NULL ? _zDisplayName : _zName )
, mzDescription(_zDescription)
, mbShowInAssetDesc(_bShowInAssetDesc)
{
}

PropertyValue::~PropertyValue()
{
	zenDelNull(mpValue);
}


PropertyValue2::PropertyValue2()
: mpDefinition(NULL)
, mpValue(NULL)
, mpRefCount(NULL)
{
} 

PropertyValue2::PropertyValue2(const class PropertyDefinition* _pParentDef, void* _pValue, zU32* _pRefCount)
{
	mpDefinition				= _pParentDef;
	mpValue						= _pValue;
	mpRefCount					= _pRefCount;
	if( mpRefCount )
		*mpRefCount = 1;
}

PropertyValue2::~PropertyValue2()
{
	ReferenceDecrease();
	mpDefinition	= NULL;
	mpValue			= NULL;
	mpRefCount		= NULL;
};

const PropertyValue2& PropertyValue2::operator=( const PropertyValue2& _Copy )
{		
	ReferenceDecrease();
	mpDefinition				= _Copy.mpDefinition;
	mpValue						= _Copy.mpValue;
	mpRefCount					= _Copy.mpRefCount;
	if( mpRefCount )
		++(*mpRefCount);

	return *this;
}

bool PropertyValue2::IsValid()const
{
	ZENAssert( (mpValue==NULL) == (mpDefinition==NULL) && (mpDefinition==NULL) == (mpRefCount==NULL) );
	return mpValue != NULL;
}

bool PropertyValue2::IsDefault()const
{
	return false;
}

bool PropertyValue2::operator==(const PropertyValue2& _Cmp)const
{
	return false;
}

void PropertyValue2::ReferenceDecrease()
{
	if( mpRefCount )
	{
		--(*mpRefCount);
		if( *mpRefCount == 0 )
			zenDelNull(mpValue);
	}
}

PropertyValue2 PropertyDefinition::CreateValue(const PropertyDefinition* _pParentDef, void* _pValue, zU32* _pRefCount)const
{
	return PropertyValue2(_pParentDef, _pValue, _pRefCount);
}


//=================================================================================================
// PROPERTY BOOL
//=================================================================================================
PropertyBool::PropertyBool(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, Data _Default)
: PropertyBase(_zName, zenConst::keAssProp_Bool, _zDisplayName, _zDescription, _bShowInAssetDesc )
, mDefault(_Default)
{
}

zUInt PropertyBool::ToString(const void* _pValue, zUInt _uLen, char* _zOutString)const
{
	const Data* pValue = static_cast<const Data*>(_pValue);
	return ::sprintf_s(_zOutString, _uLen, "%s", zenConst::kzFalseTrue[*pValue]);
}

bool PropertyBool::IsDefault( const void* _pValue )const
{
	const Data* pValue = static_cast<const Data*>(_pValue);
	return *pValue == mDefault;
}

//=================================================================================================
// PROPERTY FLOAT
//=================================================================================================
PropertyFloat::PropertyFloat(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, float _Default, float _Inc, float _Min, float _Max )
: PropertyBase(_zName, zenConst::keAssProp_Float, _zDisplayName, _zDescription, _bShowInAssetDesc )
, mDefault(_Default)
, mValMin(_Min)
, mValMax(_Max)
, mValInc(_Inc)
{
}

zUInt PropertyFloat::ToString(const void* _pValue, zUInt _uLen, char* _zOutString)const
{
	const Data* pValue = static_cast<const Data*>(_pValue);
	return ::sprintf_s(_zOutString, _uLen, "%f", *pValue);
}

bool PropertyFloat::IsDefault( const void* _pValue )const
{
	const Data* pValue = static_cast<const Data*>(_pValue);
	return *pValue == mDefault;
}

//=================================================================================================
// PROPERTY INT
//=================================================================================================
PropertyInt::PropertyInt(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, Data _Default, Data _Inc, Data _Min, Data _Max )
: PropertyBase(_zName, zenConst::keAssProp_Int, _zDisplayName, _zDescription, _bShowInAssetDesc )
, mDefault(_Default)
, mValMin(_Min)
, mValMax(_Max)
, mValInc(_Inc)
{
}

zUInt PropertyInt::ToString(const void* _pValue, zUInt _uLen, char* _zOutString)const
{
	const Data* pValue = static_cast<const Data*>(_pValue);
	return ::sprintf_s(_zOutString, _uLen, "%i", *pValue);
}

bool PropertyInt::IsDefault( const void* _pValue )const
{
	const Data* pValue = static_cast<const Data*>(_pValue);
	return *pValue == mDefault;
}

//=================================================================================================
// PROPERTY FILE
//=================================================================================================
PropertyFile::PropertyFile(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, const char* _zDefault, const char* _zFileExt )
: PropertyBase(_zName, zenConst::keAssProp_File, _zDisplayName, _zDescription, _bShowInAssetDesc )
, mDefault(_zDefault)
, mzFileExt(_zFileExt)
{
}

zUInt PropertyFile::ToString(const void* _pValue, zUInt _uLen, char* _zOutString)const
{
	const Data* pValue = static_cast<const Data*>(_pValue);
	return ::sprintf_s(_zOutString, _uLen, "%s", (const char*)pValue);
}

bool PropertyFile::IsDefault( const void* _pValue )const
{
	const Data* pValue = static_cast<const Data*>(_pValue);
	return *pValue == mDefault;
}

//=================================================================================================
// PROPERTY ARRAY
//=================================================================================================
/*
PropertyArray::PropertyFile(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, const PropertyBase& _Property )
: PropertyBase(_zName, zenConst::keAssProp_File, _zDisplayName, _zDescription, _bShowInAssetDesc )
, mProperty(_Property)
{
}

zUInt PropertyArray::ToString(const void* _pValue, zUInt _uLen, char* _zOutString)const
{
	const Data* pValue = static_cast<const Data*>(_pValue);
	return ::sprintf_s(_zOutString, _uLen, "%s", (const char*)pValue);
}

bool PropertyArray::IsDefault( const void* _pValue )const
{
	const Data* pValue = static_cast<const Data*>(_pValue);
	return *pValue == mDefault;
}

class  : public PropertyBase
{
	ZENClassDeclare(PropertyArray, PropertyBase)
public:
	typedef zString	Data;		
	PropertyArray(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, const PropertyBase& _Property );

	PropertyBase&		mProperty;
*/

//=================================================================================================
// PROPERTY ENUM
//=================================================================================================
PropertyEnum::PropertyEnum(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, Data _Default, const Entry* _pEntries, zUInt _uEntryCount )
: PropertyBase(_zName, zenConst::keAssProp_Enum, _zDisplayName, _zDescription, _bShowInAssetDesc )
, mDefault(_Default)
{
	maEnumValues.Copy(_pEntries, _uEntryCount);
	mdKeyToIndex.Init(_uEntryCount*2);
	maValueToIndex.Init(_uEntryCount*2);
	for(zUInt idx(0); idx<_uEntryCount; ++idx)
	{
		mdKeyToIndex.Set(_pEntries[idx].mStrValue.mhName, idx);
		maValueToIndex.Set(_pEntries[idx].miValue, idx);
	}
	ZENAssertMsg( maValueToIndex.Exist(mDefault), "Specified default value doesn't exist in Asset Property Definition");
	maValueToIndex.SetDefaultValue( maValueToIndex[mDefault] );
	mdKeyToIndex.SetDefaultValue( maValueToIndex[mDefault] );
}

const PropertyEnum::Entry& PropertyEnum::GetEnumEntry(Data _Value)const
{
	return maEnumValues[ maValueToIndex[_Value] ];
}

const PropertyEnum::Entry& PropertyEnum::GetEnumEntry(zHash32 _hValue)const
{
	return maEnumValues[ mdKeyToIndex[_hValue] ];
}

zUInt PropertyEnum::ToString(const void* _pValue, zUInt _uLen, char* _zOutString)const
{
	const Data* pValue = static_cast<const Data*>(_pValue);
	zUInt uIndex = maValueToIndex[ *pValue ];
	return ::sprintf_s(_zOutString, _uLen, "%s", maEnumValues[uIndex].mStrValue.mzName );
}

bool PropertyEnum::IsDefault(const void* _pValue)const
{
	const Data* pValue = static_cast<const Data*>(_pValue);
	return *pValue == mDefault;
}

}} //namespace zen { namespace zenAss

#endif //ZEN_ENGINETOOL
