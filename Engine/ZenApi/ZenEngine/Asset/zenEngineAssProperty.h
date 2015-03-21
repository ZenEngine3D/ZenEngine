#pragma once
#ifndef __zenEngine_Asset_Property_h__
#define __zenEngine_Asset_Property_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{
	typedef zGameConstRef<class PropertyDefinition>		PropertyDefRef;
	typedef const zArrayStatic<PropertyDefRef>			PropertyDefArray;

	//=============================================================================
	// PropertyValue 
	//! @todo Asset: move to engine side?
	//=============================================================================
	class PropertyValue : public zRefCountedAutoDel
	{
	ZENClassDeclare(PropertyValue, zRefCountedAutoDel);
	public:
							PropertyValue(const zAssetItemRef& _rOwnerAsset, const PropertyDefRef& _rOwnerDefinition);
												//!< @todo Cleanup : Asset ptr really needed? Pointer to signal object instead?
		zAssetItemRef		mrOwnerAsset;		//!< Asset property belongs to (null if this is a default property value) 
		PropertyDefRef		mrOwnerDefinition;	//!< Property Definition describing value
	};
	
	template<class TClassDefinition, class TClassValueStorage>
	class TPropertyValue : public PropertyValue
	{
	ZENClassDeclare(TPropertyValue, PropertyValue);
	public:
							TPropertyValue(const zAssetItemRef& _rOwnerAsset, const PropertyDefRef& _rOwnerDefinition);
		TClassValueStorage	mValue;
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
												PropertyValueRef(const PropertyValueRef& _rCopy):Super(_rCopy){}
		bool									operator==(const PropertyValueRef& _rCmp)const;
		bool									IsDefault()const;
		PropertyValueRef						Clone(const zAssetItemRef& _rOwnerAsset)const;
		zenConst::eAssetPropertyType			GetType() const;	
		const PropertyDefinition&				GetDefinition()const;
		const PropertyValue&					GetValueProperty()const;
	protected:
		void									EmitPropertyChanged();
	};
	
	template<class TClassDefinition, class TClassValueStorage>
	class TPropertyValueRef : public PropertyValueRef
	{
		ZENClassDeclare(TPropertyValueRef, PropertyValueRef);	
	public:
		typedef TPropertyValue<TClassDefinition, TClassValueStorage> Value;

		TPropertyValueRef(){}			
		TPropertyValueRef( const PropertyValueRef& _Copy );
		TPropertyValueRef&						operator=( const TPropertyValueRef& _Copy );
		TPropertyValueRef&						operator=( const TClassValueStorage& _Assign );

		const TClassValueStorage&				GetValue()const;
		const TClassDefinition&					GetDefinition()const;
	};

	//=============================================================================
	// Property Definition
	//=============================================================================
	class PropertyDefinition : public zRefCountedAutoDel
	{
	ZENClassDeclare(PropertyDefinition, zRefCountedAutoDel);
	public:												
		virtual PropertyValueRef				Allocate(const zAssetItemRef& _rOwnerAsset)const=0;
		virtual zenConst::eAssetPropertyType	GetType()const=0;
		const zString&							GetTypeName()const;
		static const zString&					GetTypeName(zenConst::eAssetPropertyType _eType);
		static zenConst::eAssetPropertyType		GetTypeFromName(const char* _zName);		
		zStringHash32							mName;
		zString									mzDisplayName;
		zString									mzDescription;		
		bool									mbShowInAssetDesc;
		bool									mbIsEditable;		
	protected:
		virtual PropertyValueRef				Clone(const PropertyValueRef& _rValue)const=0;
		virtual bool							IsDefault(const PropertyValueRef& _rValue)const=0;
		virtual bool							IsEqual(const PropertyValueRef& _rValue1, const PropertyValueRef& _rValue2) const=0;
		PropertyDefinition(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, bool _bIsEditable);
		friend PropertyValueRef;
	};

 	template<zenConst::eAssetPropertyType TPropertyType, class TClassDefinition, class TClassValue>
 	class TPropertyDefinition : public PropertyDefinition
 	{
	ZENClassDeclare(TPropertyDefinition, PropertyDefinition);	
	public:		
		typedef TPropertyValue<TClassDefinition, TClassValue>		ValueProperty;	//!< Wrapper around PropertyValue (Definition+RefCount+Value)
		typedef TPropertyValueRef<TClassDefinition, TClassValue>	ValueRef;		//!< PropertyRef to access the value
		typedef TClassValue											ValueStorage;	//!< Type of Data value stored for this property
		enum { kPropertyType = TPropertyType };
												
 		virtual PropertyValueRef				Allocate(const zAssetItemRef& _rOwnerAsset) const;
		virtual zenConst::eAssetPropertyType	GetType()const;
	protected:
		virtual PropertyValueRef				Clone(const PropertyValueRef& _rValue)const;
		virtual bool							IsDefault(const PropertyValueRef& _rValueRef)const;		
		virtual bool							IsEqual(const PropertyValueRef& _rValue1, const PropertyValueRef& _rValue2)const;
		TPropertyDefinition(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, bool _bIsEditable);
 	};

#define ZENPropertyDefinitionDeclare( ClassName, ... )																	\
	ZENClassDeclare(ClassName, TPropertyDefinition);																	\
	private:																											\
		ClassName(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, bool _bIsEditable )	\
		: TPropertyDefinition(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _bIsEditable){}					\
	public:																												\
		ValueStorage mDefault;																							\
	
}} //namespace zen { namespace zenAss

#endif

#include "zenEngineAssProperty.inl"

#endif