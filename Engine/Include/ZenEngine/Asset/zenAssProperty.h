#pragma once
#ifndef __zenEngine_Asset_Property_h__
#define __zenEngine_Asset_Property_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{
	typedef zGameRefConst<class PropertyDefinition>		PropertyDefRef;
	typedef const zArrayStatic<PropertyDefRef>			PropertyDefArray;

	//=============================================================================
	// PropertyValue 
	//! @todo Asset: move to engine side?
	//=============================================================================
	class PropertyValue : public zRefCountedAutoDel
	{
	ZENClassDeclare(PropertyValue, zRefCountedAutoDel);
	public:
												PropertyValue(const PropertyDefRef& _rParent);
	//protected:
		PropertyDefRef							mrDefinition;
	};
	
	//=============================================================================
	// Templated PropertyValue
	//=============================================================================
	template<class TClassDefinition, class TClassValueStorage>
	class TPropertyValue : public PropertyValue
	{
	ZENClassDeclare(TPropertyValue, PropertyValue);
	public:
												TPropertyValue(const PropertyDefRef&);
		TClassValueStorage						mValue;
	};

	//=============================================================================
	// PropertyValueRef
	//=============================================================================
	class PropertyValueRef : public zGameRef<PropertyValue>
	{
	ZENClassDeclare(PropertyValueRef, zGameRef<PropertyValue>);
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
		virtual PropertyValueRef				Allocate()const=0;
		virtual zenConst::eAssetPropertyType	GetType()const=0;
		const zString&							GetTypeName()const;
		static const zString&					GetTypeName(zenConst::eAssetPropertyType _eType);
		static zenConst::eAssetPropertyType		GetTypeFromName(const char* _zName);
		virtual bool							IsDefault(const class PropertyValueRef& _ValueRef)const=0;
		zStringHash32							mName;
		zString									mzDisplayName;
		zString									mzDescription;		
		bool									mbShowInAssetDesc;

	protected:
		PropertyDefinition(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc);
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
												
 		virtual PropertyValueRef				Allocate() const;
		virtual bool							IsDefault(const PropertyValueRef& _rValueRef)const;		
		virtual zenConst::eAssetPropertyType	GetType()const;
	
	protected:
		TPropertyDefinition(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc);
 	};

#define ZENPropertyDefinitionDeclare( ClassName, ... )																	\
	ZENClassDeclare(ClassName, TPropertyDefinition);																	\
	private:																											\
		ClassName(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc )	\
		: TPropertyDefinition(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc){}								\
	public:																												\
		ValueStorage mDefault;																							\
		//static PropertyDefRef Create( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, __VA_ARGS__ )

	
}} //namespace zen { namespace zenAss

#endif

#include "zenAssProperty.inl"

#endif