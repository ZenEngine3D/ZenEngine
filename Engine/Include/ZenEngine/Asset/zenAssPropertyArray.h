#pragma once
#ifndef __zenEngine_Asset_PropertyArray_h__
#define __zenEngine_Asset_PropertyArray_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{
	
class PropertyArray : public TPropertyDefinition<zenConst::keAssProp_Array, PropertyArray, zArrayDynamic<PropertyValueRef> >
{
ZENClassDeclare(PropertyArray, TPropertyDefinition);	
public:			
	class ValueRef : public TPropertyDefinition::ValueRef
	{	
	ZENClassDeclare(ValueRef, TPropertyDefinition::ValueRef);
	public:
		ValueRef():TPropertyDefinition::ValueRef(){};
		ValueRef(const PropertyValueRef& _Copy):TPropertyDefinition::ValueRef(_Copy){};

		PropertyValueRef AddEntry()
		{
			ZENAssert( IsValid() );
			const PropertyArray& Definition		= GetDefinition();
			PropertyValueRef newValue			= Definition.mArrayPropertyDef.Allocate();
			ValueStorage& arrayValue			= Get();
			arrayValue.Append(newValue);
			return newValue;
		}

		void RemEntry(const PropertyValueRef& _ToRemove)
		{
			ZENAssert( IsValid() );
			ValueStorage& arrayValue = Get();
			for( zUInt idx(0), count(arrayValue.Count()); idx<count; ++idx)
			{
				if( arrayValue[idx] == _ToRemove )
				{
					for(zUInt idxCopy(idx); idxCopy+1<count; ++idxCopy)
						arrayValue[idxCopy] = arrayValue[idxCopy+1];
					arrayValue.Pop();
					break;
				}
			}
		}
	};

	PropertyArray(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, ValueStorage _Default, const PropertyDefinition& _PropertyDef)
	: TPropertyDefinition(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc)
	, mDefault(_Default)
	, mArrayPropertyDef(_PropertyDef)
	{}

	ValueStorage				mDefault;
	const PropertyDefinition&	mArrayPropertyDef;
};

}} //namespace zen { namespace zenAss

#endif
#endif