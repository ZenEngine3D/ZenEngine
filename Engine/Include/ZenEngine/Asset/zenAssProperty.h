#pragma once
#ifndef __zenEngine_Asset_Property_h__
#define __zenEngine_Asset_Property_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{
	const char* GetPropertyTypeName(zenConst::eAssetPropertyType _eType);

	//=============================================================================
	// PropertyValue 
	//! @todo Asset: move to engine side?
	//=============================================================================
	class PropertyValue : public zRefCountedAutoDel
	{
	ZENClassDeclare(PropertyValue, zRefCountedAutoDel);
	public:
												PropertyValue(const class PropertyDefinition& _Parent);
	//protected:
		const class PropertyDefinition&			mDefinition;
	};
	
	//=============================================================================
	// Templated PropertyValue
	//=============================================================================
	template<class TClassDefinition, class TClassValueStorage>
	class TPropertyValue : public PropertyValue
	{
	ZENClassDeclare(TPropertyValue, PropertyValue);
	public:
												TPropertyValue(const PropertyDefinition& _Parent);
		TClassValueStorage						mValue;
	};

	//=============================================================================
	// PropertyValueRef
	//=============================================================================
	class PropertyValueRef : public zRefOwner<PropertyValue>
	{
	ZENClassDeclare(PropertyValueRef, zRefOwner<PropertyValue>);
	public:
												PropertyValueRef(){}
												PropertyValueRef(PropertyValue* _pReference ):Super(_pReference){}
												PropertyValueRef(const PropertyValueRef& _Copy):Super(_Copy){}
		bool									IsDefault()const;
		zenConst::eAssetPropertyType			GetType() const;	
		const PropertyDefinition&				GetDefinition()const;
	};

	//=============================================================================
	// Property Definition
	//=============================================================================
	class PropertyDefinition : public zRefCountedAutoDel
	{
	ZENClassDeclare(PropertyDefinition, zRefCountedAutoDel);
	public:
												PropertyDefinition(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc);
		virtual PropertyValueRef				Allocate()const=0;
		virtual zenConst::eAssetPropertyType	GetType()const=0;
		virtual bool							IsDefault(const class PropertyValueRef& _ValueRef)const=0;
		zStringHash32							mName;
		zString									mzDisplayName;
		zString									mzDescription;		
		bool									mbShowInAssetDesc;
	};


	//=============================================================================
	// Templated Value Reference
	//=============================================================================
	template<class TClassDefinition, class TClassValueStorage>
	class TPropertyValueRef : public PropertyValueRef
	{
	ZENClassDeclare(TPropertyValueRef, PropertyValueRef);	
	public:
		typedef TPropertyValue<TClassDefinition, TClassValueStorage> Value;

												TPropertyValueRef(){}			
												TPropertyValueRef( const PropertyValueRef& _Copy );
		TPropertyValueRef&						operator=( const TPropertyValueRef& _Copy );
		
		TClassValueStorage&						Get();		
		const TClassValueStorage&				Get()const;
		const TClassDefinition&					GetDefinition() const;
	};
	
	//=============================================================================
	// Templated PropertyDefinition
	//=============================================================================
 	template<zenConst::eAssetPropertyType TPropertyType, class TClassDefinition, class TClassValue>
 	class TPropertyDefinition : public PropertyDefinition
 	{
	ZENClassDeclare(TPropertyDefinition, PropertyDefinition);	
	public:		
		typedef TPropertyValueRef<TClassDefinition, TClassValue>	ValueRef;
		typedef TClassValue											ValueStorage;
		enum { kPropertyType = TPropertyType };

												TPropertyDefinition(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc);
 		virtual PropertyValueRef				Allocate() const;
		virtual bool							IsDefault(const PropertyValueRef& _rValueRef)const;		
		virtual zenConst::eAssetPropertyType	GetType()const;
 	};

	typedef const zArrayStatic<const PropertyDefinition*> zPropertyArray;
}} //namespace zen { namespace zenAss

#endif

#include "zenAssProperty.inl"

#endif