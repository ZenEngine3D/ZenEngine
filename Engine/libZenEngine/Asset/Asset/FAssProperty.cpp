#include "libZenEngine.h"
#if AW_ENGINETOOL

namespace FAss
{

#define AWAssetTypeExpand(_TypeName_) zenMem::AllocatorPool PropertyDef##_TypeName_##::sPoolAlloc( "PropertyDef"ZENStringDefine2(_TypeName_)"Pool", sizeof(PropertyDef##_TypeName_##::Value), 100, 100);
AWAssetTypes
#undef	AWAssetTypeExpand

static zenStringHash32 sTypeDescription[]={
	#define AWAssetTypeExpand(_TypeName_)	zenStringHash32(#_TypeName_),
	AWAssetTypes	
	#undef AWAssetTypeExpand
};

//=================================================================================================
//! @brief		zenString representation of all PropertyDef type enum
//! @details	Constructed automatically from 'AWAssetTypes' define
//-------------------------------------------------------------------------------------------------
//! @param		_ePropertyType	- Property we want name from
//! @return						- zenString representation of the enum value
//=================================================================================================
const char* PropertyDefBase::GetTypeDesc(PropertyDefBase::enumType _ePropertyType)
{	
	ZENAssert(_ePropertyType<keType__Count);
	return sTypeDescription[_ePropertyType].mzName;
}

//=================================================================================================
//! @brief		Find which enum has the same name hash
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param		_hPropertyName	- Property name we want enum value from
//=================================================================================================
PropertyDefBase::enumType PropertyDefBase::GetType(zenHash32 _hPropertyName)
{
	zenUInt uIndex = zenStringHash32::Find(_hPropertyName, sTypeDescription, ZENArrayCount(sTypeDescription));
	return uIndex < keType__Count ? static_cast<PropertyDefBase::enumType>(uIndex) : keType__Invalid;
}

//=================================================================================================
//! @brief		Save a parameter value to an xml node
//! @details	Setup common parameter value attribute, then call virtual value export
//-------------------------------------------------------------------------------------------------
//! @param		_NodeParent - XML Node that will receive the parameter value child node
//=================================================================================================
pugi::xml_node PropertyDefBase::Value::ValueToXml(pugi::xml_node& _NodeParent)const
{
	pugi::xml_node nodeProp = _NodeParent.append_child(kzXmlName_Node_Property);
	nodeProp.append_attribute(kzXmlName_PropAtr_Name).set_value(mParentDef.mzName);
	nodeProp.append_attribute(kzXmlName_PropAtr_Type).set_value(mParentDef.GetTypeDesc());
	mParentDef.ValueToXml( *this, nodeProp );
	return nodeProp;
}

//=================================================================================================
//! @brief		Load a property value from an xml node
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param		_NodeParameter - XML Node containing all needed parameter info
//=================================================================================================
bool PropertyDefBase::Value::ValueFromXml(const pugi::xml_node& _NodeProperty)
{
	const char* zPropType(_NodeProperty.attribute(kzXmlName_PropAtr_Type).as_string());
	PropertyDefBase::enumType ePropType(PropertyDefBase::GetType(zenHash32(zPropType)));
	if( mParentDef.GetType() == ePropType )
		return mParentDef.ValueFromXml( *this, _NodeProperty );

	return false;
}

//=================================================================================================
//! @brief		Add the value attribute to the current xml parameter node
//! @details	Default behavior output the regular string representation
//!				Child class should overload this for more complex scenario
//-------------------------------------------------------------------------------------------------
//! @param		_Value		- Value to save
//! @param		_ParamNode	- XML Node that will receive the parameter value attribute
//=================================================================================================
void PropertyDefBase::ValueToXml(const PropertyDefBase::Value& _Value, pugi::xml_node& _ParamNode ) const
{
	char zBuffer[128];
	_Value.ValueToString(zBuffer, sizeof(zBuffer));
	_ParamNode.append_attribute("Value").set_value(zBuffer);	
}

//=================================================================================================
//! @brief		Output a string representation of the value using this PropertyDefinition
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param		_Value		- Value to create a string for (type must be same as this parameterdef)
//! @param		_zBuffer	- Buffer to store string in (instead of allocation a string every time)
//! @param		_uSizebuffer- Buffer size
//=================================================================================================
void PropertyDefBool::ValueToString(const PropertyDefBase::Value& _Value, char* _zBuffer, zenUInt _uSizebuffer) const 
{ 
	ZENAssert(_Value.mParentDef.GetType() == GetType());
	const Value& value = static_cast<const Value&>(_Value);
	sprintf(_zBuffer, "%s", awconst::kzFalseTrue[value.mValue]);
}

//=================================================================================================
//! @brief		Load a property value from an xml node
//! @details	Looks for attributes specific to property type, and load data accordingly
//-------------------------------------------------------------------------------------------------
//! @return 	- True if successful
//=================================================================================================
bool PropertyDefBool::ValueFromXml(PropertyDefBase::Value& _Value, const pugi::xml_node& _NodeProperty)const
{
	ZENAssert(_Value.mParentDef.GetType() == GetType());
	Value& value					= static_cast<Value&>(_Value);
	pugi::xml_attribute attrValue	= _NodeProperty.attribute("Value");
	if( attrValue )
	{		
		value.mValue = attrValue.as_bool();
		return true;
	}
	return false;
}

//=================================================================================================
//! @brief		Output a string representation of the value using this PropertyDefinition
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param		_Value		- Value to create a string for (type must be same as this parameterdef)
//! @param		_zBuffer	- Buffer to store string in (instead of allocation a string every time)
//! @param		_uSizebuffer- Buffer size
//=================================================================================================
void PropertyDefInt::ValueToString(const PropertyDefBase::Value& _Value, char* _zBuffer, zenUInt _uSizebuffer) const 
{ 
	ZENAssert(_Value.mParentDef.GetType() == GetType());
	const Value& value = static_cast<const Value&>(_Value);
	sprintf(_zBuffer, "%i", value.mValue);
}

//=================================================================================================
//! @brief		Load a property value from an xml node
//! @details	Looks for attributes specific to property type, and load data accordingly
//-------------------------------------------------------------------------------------------------
//! @return 	- True if successful
//=================================================================================================
bool PropertyDefInt::ValueFromXml(PropertyDefBase::Value& _Value, const pugi::xml_node& _NodeProperty)const
{
	ZENAssert(_Value.mParentDef.GetType() == GetType());
	Value& value					= static_cast<Value&>(_Value);
	pugi::xml_attribute attrValue	= _NodeProperty.attribute("Value");
	if( attrValue )
	{		
		value.mValue = attrValue.as_int();
		return true;
	}
	return false;
}

//=================================================================================================
//! @brief		Output a string representation of the value using this PropertyDefinition
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param		_Value		- Value to create a string for (type must be same as this parameterdef)
//! @param		_zBuffer	- Buffer to store string in (instead of allocation a string every time)
//! @param		_uSizebuffer- Buffer size
//=================================================================================================
void PropertyDefFloat::ValueToString(const PropertyDefBase::Value& _Value, char* _zBuffer, zenUInt _uSizebuffer) const 
{ 
	ZENAssert(_Value.mParentDef.GetType() == GetType());
	const Value& value = static_cast<const Value&>(_Value);
	sprintf(_zBuffer, "%f", value.mValue);
}

//=================================================================================================
//! @brief		Load a property value from an xml node
//! @details	Looks for attributes specific to property type, and load data accordingly
//-------------------------------------------------------------------------------------------------
//! @return 	- True if successful
//=================================================================================================
bool PropertyDefFloat::ValueFromXml(PropertyDefBase::Value& _Value, const pugi::xml_node& _NodeProperty)const
{
	ZENAssert(_Value.mParentDef.GetType() == GetType());
	Value& value					= static_cast<Value&>(_Value);
	pugi::xml_attribute attrValue	= _NodeProperty.attribute("Value");
	if( attrValue )
	{		
		value.mValue = attrValue.as_float();
		return true;
	}
	return false;
}

//=================================================================================================
//! @brief		Output a string representation of the value using this PropertyDefinition
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param		_Value		- Value to create a string for (type must be same as this parameterdef)
//! @param		_zBuffer	- Buffer to store string in (instead of allocation a string every time)
//! @param		_uSizebuffer- Buffer size
//=================================================================================================
void PropertyDefEnum::ValueToString(const PropertyDefBase::Value& _Value, char* _zBuffer, zenUInt _uSizebuffer) const 
{ 
	ZENAssert(_Value.mParentDef.GetType() == GetType());
	const Value& value = static_cast<const Value&>(_Value);
	zenUInt foundIdx(maEntry.Count()-1);
	for(zenUInt idx(0); idx<foundIdx; ++idx)
		if( maEntry[idx].hKey == value.mValue )
			foundIdx = idx;
	sprintf(_zBuffer, "%s", maEntry[foundIdx].zName);
}

//=================================================================================================
//! @brief		Load a property value from an xml node
//! @details	Looks for attributes specific to property type, and load data accordingly
//-------------------------------------------------------------------------------------------------
//! @return 	- True if successful
//=================================================================================================
bool PropertyDefEnum::ValueFromXml(PropertyDefBase::Value& _Value, const pugi::xml_node& _NodeProperty)const
{
	ZENAssert(_Value.mParentDef.GetType() == GetType());
	Value& value					= static_cast<Value&>(_Value);
	pugi::xml_attribute attrValue	= _NodeProperty.attribute("Value");
	if( attrValue )
	{		
		value.mValue = zenHash32(attrValue.as_string());
		return true;
	}
	return false;
}

//=================================================================================================
//! @brief		Output a string representation of the value using this PropertyDefinition
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param		_Value		- Value to create a string for (type must be same as this parameterdef)
//! @param		_zBuffer	- Buffer to store string in (instead of allocation a string every time)
//! @param		_uSizebuffer- Buffer size
//=================================================================================================
void PropertyDefFile::ValueToString(const PropertyDefBase::Value& _Value, char* _zBuffer, zenUInt _uSizebuffer) const 
{ 
	ZENAssert(_Value.mParentDef.GetType() == GetType());
	const Value& value = static_cast<const Value&>(_Value);
	sprintf(_zBuffer, "%s", (const char*)value.mValue);
}

//=================================================================================================
//! @brief		Load a property value from an xml node
//! @details	Looks for attributes specific to property type, and load data accordingly
//-------------------------------------------------------------------------------------------------
//! @return 	- True if successful
//=================================================================================================
bool PropertyDefFile::ValueFromXml(PropertyDefBase::Value& _Value, const pugi::xml_node& _NodeProperty)const
{
	ZENAssert(_Value.mParentDef.GetType() == GetType());
	Value& value					= static_cast<Value&>(_Value);
	pugi::xml_attribute attrValue	= _NodeProperty.attribute("Value");
	if( attrValue )
	{		
		value.mValue = attrValue.as_string();
		return true;
	}
	return false;
}

//=================================================================================================
//! @brief		Output a string representation of the value using this PropertyDefinition
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param		_Value		- Value to create a string for (type must be same as this parameterdef)
//! @param		_zBuffer	- Buffer to store string in (instead of allocation a string every time)
//! @param		_uSizebuffer- Buffer size
//=================================================================================================
void PropertyDefFloat2::ValueToString(const PropertyDefBase::Value& _Value, char* _zBuffer, zenUInt _uSizebuffer) const 
{ 
	ZENAssert(_Value.mParentDef.GetType() == GetType());
	const Value& value = static_cast<const Value&>(_Value);
	sprintf(_zBuffer, "%f, %f", value.mValue.x, value.mValue.y);
}

void PropertyDefFloat2::ValueToXml(const PropertyDefBase::Value& _Value, pugi::xml_node& _ParamNode) const
{	
	ZENAssert(_Value.mParentDef.GetType() == GetType());
	const Value& PropertyValue = static_cast<const Value&>(_Value);
	_ParamNode.append_attribute("x").set_value(PropertyValue.mValue.x);
	_ParamNode.append_attribute("y").set_value(PropertyValue.mValue.y);
}

//=================================================================================================
//! @brief		Load a property value from an xml node
//! @details	Looks for attributes specific to property type, and load data accordingly
//-------------------------------------------------------------------------------------------------
//! @return 	- True if successful
//=================================================================================================
bool PropertyDefFloat2::ValueFromXml(PropertyDefBase::Value& _Value, const pugi::xml_node& _NodeProperty)const
{
	ZENAssert(_Value.mParentDef.GetType() == GetType());
	Value& value				= static_cast<Value&>(_Value);
	pugi::xml_attribute attrX	= _NodeProperty.attribute("x");
	pugi::xml_attribute attrY	= _NodeProperty.attribute("y");
	if( attrX && attrY)
	{		
		value.mValue.x = attrX.as_float();
		value.mValue.y = attrY.as_float();
		return true;
	}
	return false;
}

}

#endif //AW_ENGINETOOL