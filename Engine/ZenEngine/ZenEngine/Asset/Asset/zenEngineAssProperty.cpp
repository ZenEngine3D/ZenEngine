#include "zeEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss
{
static const zStringHash32 saPropertyName[]={ 
	"Bool",		// keAssProp_Bool,
	"File",		// keAssProp_File,
	"Float",	// keAssProp_Float,
	"Float2",	// keAssProp_Float2,
	"Float3",	// keAssProp_Float3,
	"Float4",	// keAssProp_Float4,
	"Int",		// keAssProp_Int,
	"Int2",		// keAssProp_Int2,
	"Int3",		// keAssProp_Int3,
	"Int4",		// keAssProp_Int4,
	"Enum",		// keAssProp_Enum,
	"Asset",	// keAssProp_Asset
	"Struct",	// keAssProp_Struct
	"Array",	// keAssProp_Array,
};

//=============================================================================
// PropertyValue
//=============================================================================
PropertyValue::PropertyValue(const zAssetItemRef& _rOwnerAsset, const PropertyDefRef& _rOwnerDefinition)
: mrOwnerAsset(_rOwnerAsset)
, mrOwnerDefinition(_rOwnerDefinition)
{	
	ZENAssert(mrOwnerDefinition.IsValid());
}

//=============================================================================
// PropertyValueRef
//=============================================================================
zenConst::eAssetPropertyType PropertyValueRef::GetType() const
{
	return IsValid() ? Get()->mrOwnerDefinition->GetType() : zenConst::keAssProp__Invalid;
}

bool PropertyValueRef::IsDefault()const
{
	return IsValid() ? Get()->mrOwnerDefinition->IsDefault( *this ) : false;
}

const PropertyDefinition& PropertyValueRef::GetDefinition() const
{
	return *(GetSafe()->mrOwnerDefinition.Get());
}

const PropertyValue& PropertyValueRef::GetValueProperty()const
{
	return *GetSafe();
}

PropertyValueRef PropertyValueRef::Clone(const zAssetItemRef& _rOwnerAsset)const
{
	PropertyValueRef rClone = GetSafe()->mrOwnerDefinition->Clone( *this );
	rClone->mrOwnerAsset	= _rOwnerAsset;
	return rClone;
}

bool PropertyValueRef::operator==(const PropertyValueRef& _rCmp)const
{
	return GetSafe()->mrOwnerDefinition->IsEqual( *this, _rCmp);
}

void PropertyValueRef::EmitPropertyChanged()
{
	if( GetSafe()->mrOwnerAsset.IsValid() )
		GetSafe()->mrOwnerAsset->msigUpdatedProperty.Emit( *this );
}


//=============================================================================
// PropertyDefinition
//=============================================================================
PropertyDefinition::PropertyDefinition(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, bool _bIsEditable)
: mName(_zName)
, mzDisplayName( (_zDisplayName && _zDisplayName[0]!=0) ? _zDisplayName : _zName)
, mzDescription(_zDescription)
, mbShowInAssetDesc(_bShowInAssetDesc)
, mbIsEditable(_bIsEditable)
{
}

const zString& PropertyDefinition::GetTypeName()const
{
	return GetTypeName( GetType() );
}

const zString& PropertyDefinition::GetTypeName(zenConst::eAssetPropertyType _eType)
{
	ZENStaticAssert( ZENArrayCount(saPropertyName)==zenConst::keAssProp__Count );
	ZENAssert( _eType < zenConst::keAssProp__Count);
	return saPropertyName[_eType].mzName;
}

zenConst::eAssetPropertyType PropertyDefinition::GetTypeFromName(const char* _zName)
{
	zUInt uFoundIdx = zStringHash32::Find( zHash32(_zName), saPropertyName, ZENArrayCount(saPropertyName) );
	return uFoundIdx < zenConst::keAssProp__Count ? static_cast<zenConst::eAssetPropertyType>(uFoundIdx) : zenConst::keAssProp__Invalid;
}

}} //namespace zen { namespace zenAss

#endif //ZEN_ENGINETOOL
