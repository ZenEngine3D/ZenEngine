#include "libZenEngine.h"

#if ZEN_ENGINETOOL

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
	"Struct",	// keAssProp_Struct
	"Array",	// keAssProp_Array,
};

namespace zen { namespace zenAss
{

const char* GetPropertyTypeName(zenConst::eAssetPropertyType _eType)
{
	ZENStaticAssert( ZENArrayCount(saPropertyName)==zenConst::keAssProp__Count );
	ZENAssert( _eType < zenConst::keAssProp__Count);
	return saPropertyName[_eType].mzName;
}

//=============================================================================
// PropertyValue
//=============================================================================
PropertyValue::PropertyValue(const class PropertyDefinition& _Parent)
: mDefinition(_Parent)
{
}

//=============================================================================
// PropertyValueRef
//=============================================================================
zenConst::eAssetPropertyType PropertyValueRef::GetType() const
{
	return IsValid() ? mpReference->mDefinition.GetType() : zenConst::keAssProp__Invalid;
}

bool PropertyValueRef::IsDefault()const
{
	return mpReference->mDefinition.IsDefault( *this );
}

const PropertyDefinition& PropertyValueRef::GetDefinition() const
{
	ZENAssert(IsValid());
	return mpReference->mDefinition;
}

//=============================================================================
// PropertyDefinition
//=============================================================================
PropertyDefinition::PropertyDefinition(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc)
: mName(_zName)
, mzDisplayName( (_zDisplayName && _zDisplayName[0]!=0) ? _zDisplayName : _zName)
, mzDescription(_zDescription)
, mbShowInAssetDesc(_bShowInAssetDesc)
{
}

}} //namespace zen { namespace zenAss

#endif //ZEN_ENGINETOOL
