#pragma once

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{
	
class PropertyArray : public TPropertyDefinition<zenConst::keAssProp_Array, PropertyArray, zArrayDyn<PropertyValueRef> >
{
	ZENPropertyDefinitionDeclare( PropertyArray )

	class ValueRef : public TPropertyDefinition::ValueRef
	{	
	zenClassDeclare(ValueRef, TPropertyDefinition::ValueRef);
	public:
								ValueRef():TPropertyDefinition::ValueRef(){};
								ValueRef(const PropertyValueRef& _Copy):TPropertyDefinition::ValueRef(_Copy){};
		PropertyValueRef		AddEntry();
		void					RemEntry(const PropertyValueRef& _ToRemove);
		using Super::operator=;
	};

	virtual PropertyValueRef	Allocate(const zAssetItemRef& _rOwnerAsset)const;
	static PropertyDefRef		Create( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, bool _bIsEditable, 
										const PropertyDefRef& _rPropertyDef, zUInt _uEntryCountMin=0, zUInt _uEntryCountMax=9999 );
	
	PropertyDefRef				mrArrayPropertyDef;
	zUInt						muEntryCountMin;
	zUInt						muEntryCountMax;

protected:
	virtual PropertyValueRef	Clone(const zAssetItemRef& _rOwnerAsset, const PropertyValueRef& _rValue)const;
	virtual bool				IsDefault(const class PropertyValueRef& _ValueRef)const;
	virtual bool				IsEqual(const PropertyValueRef& _rValue1, const PropertyValueRef& _rValue2)const;
};

}} //namespace zen { namespace zenAss

#endif
