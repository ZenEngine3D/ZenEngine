#pragma once
#ifndef __zenEngine_Asset_PropertyArray_h__
#define __zenEngine_Asset_PropertyArray_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{
	
class PropertyArray : public TPropertyDefinition<zenConst::keAssProp_Array, PropertyArray, zArrayDynamic<PropertyValueRef> >
{
	ZENPropertyDefinitionDeclare( PropertyArray )

	class ValueRef : public TPropertyDefinition::ValueRef
	{	
	ZENClassDeclare(ValueRef, TPropertyDefinition::ValueRef);
	public:
								ValueRef():TPropertyDefinition::ValueRef(){};
								ValueRef(const PropertyValueRef& _Copy):TPropertyDefinition::ValueRef(_Copy){};
		PropertyValueRef		AddEntry();
		void					RemEntry(const PropertyValueRef& _ToRemove);
	};

	virtual PropertyValueRef	Allocate()const;
	static PropertyDefRef		Create( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, 
										const PropertyDefRef& _rPropertyDef, zUInt _uEntryCountMin=0, zUInt _uEntryCountMax=9999 );
	
	PropertyDefRef				mrArrayPropertyDef;
	zUInt						muEntryCountMin;
	zUInt						muEntryCountMax;

protected:
	virtual PropertyValueRef	Clone(const PropertyValueRef& _rValue)const;
	virtual bool				IsDefault(const class PropertyValueRef& _ValueRef)const;
	virtual bool				IsEqual(const PropertyValueRef& _rValue1, const PropertyValueRef& _rValue2)const;
};

}} //namespace zen { namespace zenAss

#endif
#endif