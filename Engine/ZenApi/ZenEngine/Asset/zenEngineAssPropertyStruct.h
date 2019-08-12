#pragma once

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{

class PropertyStruct : public TPropertyDefinition<zenConst::keAssProp_Struct, PropertyStruct, zArrayDyn<PropertyValueRef>>
{
	ZENPropertyDefinitionDeclare( PropertyStruct )

	class ValueRef : public TPropertyDefinition::ValueRef
	{
	zenClassDeclare(ValueRef, TPropertyDefinition::ValueRef);
	public:
									ValueRef():TPropertyDefinition::ValueRef(){};
									ValueRef(const PropertyValueRef& _Copy):TPropertyDefinition::ValueRef(_Copy){};		
		PropertyValueRef			GetStructValue(zHash32 _hPropertyName);
		using Super::operator=;
	};

	virtual PropertyValueRef		Allocate(const zAssetItemRef& _rOwnerAsset)const;
	static PropertyDefRef			Create( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, bool _bIsEditable, const PropertyDefRef* _prPropertyDef, zUInt _uPropertyDefCount );
	zArrayDyn<PropertyDefRef>		maPropertyDef;
	zMap<zUInt>::Key32				mdPropertyDefIndex;

protected:
	virtual PropertyValueRef		Clone(const PropertyValueRef& _rValue)const;
	virtual bool					IsDefault(const PropertyValueRef& _rValueRef)const;		
	virtual bool					IsEqual(const PropertyValueRef& _rValue1, const PropertyValueRef& _rValue2)const;
};

}} //namespace zen { namespace zenAss

#endif
