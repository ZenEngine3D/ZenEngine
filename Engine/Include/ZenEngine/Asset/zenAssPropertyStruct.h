#pragma once
#ifndef __zenEngine_Asset_PropertyStruct_h__
#define __zenEngine_Asset_PropertyStruct_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{

class PropertyStruct : public TPropertyDefinition<zenConst::keAssProp_Struct, PropertyStruct, zArrayStatic<PropertyValueRef>>
{
ZENClassDeclare(PropertyStruct, TPropertyDefinition);	
public:	
	class ValueRef : public TPropertyDefinition::ValueRef
	{
	ZENClassDeclare(ValueRef, TPropertyDefinition::ValueRef);
	public:
		ValueRef():TPropertyDefinition::ValueRef(){};
		ValueRef(const PropertyValueRef& _Copy):TPropertyDefinition::ValueRef(_Copy){};
		/*
		PropertyValue GetValue(zHash32 _hPropertyName)
		{
			ZENAssert( IsValid() );
			const PropertyStruct*				pDefinition	= GetDefinition();
			const PropertyStruct::ValueStorage&	structArray	= *TPropertyDefinition::TypeValue::GetValue();
			zUInt uIndex = pDefinition->mdPropertyDefIndex[_hPropertyName];
			if( uIndex < structArray.Count() )
				return structArray[uIndex];

			return TypeValue();
		}*/
	};

	PropertyStruct(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, const PropertyDefinition** _pPropertyDef, zUInt _uPropertyDefCount)
	: TPropertyDefinition(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc)
	, mdPropertyDefIndex(16)
	{
		ZENAssert(_pPropertyDef && _uPropertyDefCount>0);
		maPropertyDef.Copy(_pPropertyDef, _uPropertyDefCount);
		mDefault.SetCount(_uPropertyDefCount);
		mdPropertyDefIndex.SetDefaultValue(0xFFFFFFFF);
		for(zUInt idx(0); idx<_uPropertyDefCount; ++idx)
		{
			mDefault[idx] = maPropertyDef[idx]->Allocate();				
			mdPropertyDefIndex.Set(_pPropertyDef[idx]->mName.mhName, idx);
		}
	}

	ValueStorage							mDefault;
	zArrayStatic<const PropertyDefinition*>	maPropertyDef;
	zMap<zUInt>::Key32						mdPropertyDefIndex;
};

}} //namespace zen { namespace zenAss

#endif
#endif