#pragma once
#ifndef __zenEngine_Asset_Property_h__
#define __zenEngine_Asset_Property_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{
	const char* GetPropertyTypeName(zenConst::eAssetPropertyType _eType);

	//=============================================================================================
	// Property : Base
	//=============================================================================================
	class PropertyBase
	{
	ZENClassDeclareNoParent(PropertyBase)	
	protected:
												PropertyBase( const char* _zName, zenConst::eAssetPropertyType _eType, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc );
	public:		
		zenStringHash32							mName;
		zenConst::eAssetPropertyType			meType;
		const char*								mzDisplayName;
		const char*								mzDescription;		
		bool									mbShowInAssetDesc;
		
	};

	//=============================================================================================
	// Property : Bool
	//=============================================================================================
	class PropertyBool : public PropertyBase
	{
	ZENClassDeclare(PropertyBool, PropertyBase)
	public:
		typedef bool		Data;
		Data				mDefault;
		
		ZENInline zenUInt	ToString	(const Data& _Value, zenUInt _zLen, char* _zOutString)const;
		ZENInline zenUInt	ToXml		(const Data& _Value, zenUInt _zLen, char* _zOutString)const;
		ZENInline			PropertyBool(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, Data _bDefault);
	};

	//=============================================================================================
	// Property : File
	//=============================================================================================
	class PropertyFile : public PropertyBase
	{
	ZENClassDeclare(PropertyFile, PropertyBase)
	public:
		typedef zenString	Data;
		Data				mDefault;
		const char*			mzFileExt;	//!< List of supported files extensions to display in file dialog
		
		ZENInline zenUInt	ToString	(const Data& _Value, zenUInt _zLen, char* _zOutString)const;
		ZENInline zenUInt	ToXml		(const Data& _Value, zenUInt _zLen, char* _zOutString)const;
		ZENInline			PropertyFile(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, const char* _zDefault, const char* _zFileExt="Any(*.*)|*.*" );
	};

	//=============================================================================================
	// Property Value
	//=============================================================================================
	class PropertyValue
	{
	public:
		ZENInline									PropertyValue();
													~PropertyValue();
		ZENForceInline zenConst::eAssetPropertyType	GetType		()const;
		ZENForceInline void							Allocate	(const PropertyBase& _PropertyDef);
		ZENForceInline void							Reset		();
		ZENForceInline const PropertyBase*			GetProperty	()const;
		ZENInline zenUInt							ToString	(zenUInt _zLen, char* _zOutString)const;
		ZENInline zenUInt							ToXml		(zenUInt _zLen, char* _zOutString)const;

	#define ZEN_ASSETPROPERTIES_EXPAND_CODE(_TypeName_)																							\
	ZENInline bool									Is##_TypeName_()const			{return GetType()==zenConst::keAssProp_##_TypeName_##;}			\
	ZENInline const Property##_TypeName_##::Data&	GetValue##_TypeName_()const		{ZENAssert(Is##_TypeName_##()); return *mpValue##_TypeName_;}	\
	ZENInline Property##_TypeName_##::Data&			GetValue##_TypeName_()			{ZENAssert(Is##_TypeName_##()); return *mpValue##_TypeName_;}	\
	ZENInline const Property##_TypeName_&			GetProperty##_TypeName_()const	{ZENAssert(Is##_TypeName_##()); return *static_cast<const Property##_TypeName_*>(mpDefinition);}
	ZEN_ASSETPROPERTIES_EXPAND
	#undef	ZEN_ASSETPROPERTIES_EXPAND_CODE

	protected:		
		#define ZEN_ASSETPROPERTIES_EXPAND_CODE(_TypeName_) Property##_TypeName_::Data* mpValue##_TypeName_;
		const PropertyBase*							mpDefinition;		
		union 
		{
			void*									mpValue;
			ZEN_ASSETPROPERTIES_EXPAND	
		};
		#undef	ZEN_ASSETPROPERTIES_EXPAND_CODE
	};

	typedef const zenArrayStatic<const PropertyBase*> PropertyArray;
}} //namespace zen { namespace zenAss

#endif

#include "zenAssProperty.inl"

#endif