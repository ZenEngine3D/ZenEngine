#pragma once
#ifndef __zenEngine_Asset_Property_h__
#define __zenEngine_Asset_Property_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{
	class PropertyBase
	{
	ZENClassDeclareNoParent(PropertyBase)
	public:
		ZENInline zenConst::eAssetPropertyType		GetType()const{return meType;}
	protected:
		//-----------------------------------------------------------------------------
		// Property definition methods
		//-----------------------------------------------------------------------------
												PropertyBase( const char* _zName, zenConst::eAssetPropertyType _eType, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc );
		
		//virtual void*							Allocate()const=0;
	protected:
		zenStringHash32							mName;
		zenConst::eAssetPropertyType			meType;
		const char*								mzDisplayName;
		const char*								mzDescription;		
		bool									mbShowInAssetDesc;
	};
	/*
	template<class TDataType, zenConst::eAssetPropertyType TAssetType >
	class PropertyTemplated : public PropertyBase
	{
	ZENClassDeclare(PropertyTemplated, PropertyBase)
	public:
		typedef TDataType Data;

		PropertyTemplated( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, const Data& _DefaultValue )
		: PropertyBase( _zName, _zDisplayName, _zDescription, _bShowInAssetDesc )
		, mDefault(_DefaultValue)
		{
		}

		virtual	zenConst::eAssetPropertyType GetType()const
		{
			return TAssetType;
		};

		virtual	void* Allocate()const
		{
			static zenMem::AllocatorPool sPoolAlloc( "PropertyTemplated Pool", sizeof(Data), 100, 100 );
			return zenNew(&sPoolAlloc) Data(mDefault);
		};

		Data mDefault;
	};
	*/
	class PropertyBool : public PropertyBase//PropertyTemplated<bool, zenConst::keAssProp_Bool>
	{
	ZENClassDeclare(PropertyBool, PropertyBase)
	public:
		PropertyBool(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, bool _bDefault)
		: PropertyBase(_zName, zenConst::keAssProp_Bool, _zDisplayName, _zDescription, _bShowInAssetDesc )
		, mDefault(_bDefault)
		{}

		typedef bool Data;
		Data mDefault;		
	};


	class PropertyFile : public PropertyBase//PropertyTemplated<zenString, zenConst::keAssProp_File>
	{
	ZENClassDeclare(PropertyFile, PropertyBase)
	public:
		PropertyFile(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, const char* _zDefault, const char* _zFileExt="Any(*.*)|*.*" )
		: PropertyBase(_zName, zenConst::keAssProp_File, _zDisplayName, _zDescription, _bShowInAssetDesc )
		, mDefault(_zDefault)
		, mzFileExt(_zFileExt)
		{}
		typedef zenString	Data;
		zenString			mDefault;
		const char*			mzFileExt;	//!< List of supported files extensions to display in file dialog
	};

	class PropertyValue
	{
	public:
		ZENInline									PropertyValue();
													~PropertyValue();
		ZENForceInline zenConst::eAssetPropertyType	GetType()const;
		ZENForceInline void							Allocate(const PropertyBase& _PropertyDef);
		ZENForceInline void							Reset();
		ZENForceInline const PropertyBase*			GetProperty();

	#define ZEN_ASSETPROPERTIES_EXPAND_CODE(_TypeName_)																							\
	ZENInline bool									Is##_TypeName_() const			{return GetType()==zenConst::keAssProp_##_TypeName_##;}			\
	ZENInline const Property##_TypeName_##::Data*	GetValue##_TypeName_()const		{ZENAssert(Is##_TypeName_##()); return mpValue##_TypeName_;}	\
	ZENInline Property##_TypeName_##::Data*			GetValue##_TypeName_()			{ZENAssert(Is##_TypeName_##()); return mpValue##_TypeName_;}	\
	ZENInline const Property##_TypeName_*			GetProperty##_TypeName_()const	{ZENAssert(Is##_TypeName_##()); return static_cast<const Property##_TypeName_*>(mpDefinition);}
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