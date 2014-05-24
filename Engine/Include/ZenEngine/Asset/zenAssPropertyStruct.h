#pragma once
#ifndef __zenEngine_Asset_PropertyStruct_h__
#define __zenEngine_Asset_PropertyStruct_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{

class PropertyStruct : public TPropertyDefinition<zenConst::keAssProp_Struct, PropertyStruct, zArrayStatic<PropertyValueRef>>
{
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

	ZENPropertyDefinitionDeclare( PropertyStruct )

	static PropertyDefRef Create( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, const PropertyDefRef* _prPropertyDef, zUInt _uPropertyDefCount )
	{	
		ZENAssert(_prPropertyDef && _uPropertyDefCount>0);
		static zenMem::zAllocatorPool sAllocPool( "PropertyDefinition::Create", sizeof(PropertyStruct), 256, 256 );
		PropertyStruct* pNewDefinition		= zenNew(&sAllocPool) zenAss::PropertyStruct(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc);
		pNewDefinition->maPropertyDef.Copy(_prPropertyDef, _uPropertyDefCount);
		pNewDefinition->mDefault.SetCount(_uPropertyDefCount);
		pNewDefinition->mdPropertyDefIndex.Init(16);
		pNewDefinition->mdPropertyDefIndex.SetDefaultValue(0xFFFFFFFF);
		for(zUInt idx(0); idx<_uPropertyDefCount; ++idx)
		{
			ZENAssert( _prPropertyDef[idx].IsValid() );
			pNewDefinition->mDefault[idx] = pNewDefinition->maPropertyDef[idx]->Allocate();				
			pNewDefinition->mdPropertyDefIndex.Set(_prPropertyDef[idx]->mName.mhName, idx);
		}

		return pNewDefinition;
	}

	zArrayStatic<PropertyDefRef>	maPropertyDef;
	zMap<zUInt>::Key32				mdPropertyDefIndex;
};

}} //namespace zen { namespace zenAss

#endif
#endif