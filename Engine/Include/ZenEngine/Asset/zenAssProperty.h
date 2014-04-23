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
		virtual zUInt							ToString	( const void* _pValue, zUInt _uLen, char* _zOutString)const=0;
		virtual bool							IsDefault	( const void* _pValue )const=0;
	public:
		zStringHash32							mName;
		zenConst::eAssetPropertyType			meType;
		const char*								mzDisplayName;
		const char*								mzDescription;		
		bool									mbShowInAssetDesc;
		friend class PropertyValue;
	};

	//=============================================================================================
	// Property : Bool
	//=============================================================================================
	class PropertyBool : public PropertyBase
	{
	ZENClassDeclare(PropertyBool, PropertyBase)
	public:
		typedef bool		Data;		
							PropertyBool(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, Data _bDefault);
		Data				mDefault;
	protected:
		virtual zUInt		ToString	(const void* _pValue, zUInt _uLen, char* _zOutString)const;
		virtual bool		IsDefault	(const void* _pValue)const;	
	};

	//=============================================================================================
	// Property : Vector
	//=============================================================================================
	template<class TDataType>
	class PropertyVector : public PropertyBase
	{
	ZENClassDeclare(PropertyVector, PropertyBase)
	public:
		typedef TDataType	Data;
							PropertyVector(zenConst::eAssetPropertyType _eType, const char* _zFmtString, const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, Data _vDefault, Data _vInc, Data _vMin, Data _vMax );
		TDataType			mDefault;
		TDataType			mValMin;
		TDataType			mValMax;
		TDataType			mValInc;
	protected:
		virtual zUInt		ToString	(const void* _pValue, zUInt _uLen, char* _zOutString)const;
		virtual bool		IsDefault	(const void* _pValue)const;	
		const char*			mzFmtString;
	};
	
	//=============================================================================================
	// Property : Float
	//=============================================================================================
	class PropertyFloat : public PropertyBase
	{
		ZENClassDeclare(PropertyFloat, PropertyBase)
	public:
		typedef float		Data;		
		PropertyFloat(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, Data _Default, Data _Inc=1, Data _Min=0.f, Data _Max=1.f );
		Data				mDefault;
		Data				mValMin;
		Data				mValMax;
		Data				mValInc;
	protected:
		virtual zUInt		ToString	(const void* _pValue, zUInt _uLen, char* _zOutString)const;
		virtual bool		IsDefault	(const void* _pValue)const;	
	};

	//=============================================================================================
	// Property : Float2,3,4
	//=============================================================================================
	class PropertyFloat2 : public PropertyVector<zVec2F>
	{
	public:
							PropertyFloat2(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, Data _vDefault, Data _vInc, Data _vMin, Data _vMax)
							: PropertyVector(zenConst::keAssProp_Float2, "(%f,%f)", _zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _vDefault, _vInc, _vMin, _vMax){}	
	};
	class PropertyFloat3 : public PropertyVector<zVec3F>
	{
	public:
							PropertyFloat3(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, Data _vDefault, Data _vInc, Data _vMin, Data _vMax)
							: PropertyVector(zenConst::keAssProp_Float3, "(%f,%f,%f)", _zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _vDefault, _vInc, _vMin, _vMax){}	
	};
	class PropertyFloat4 : public PropertyVector<zVec4F>
	{
	public:
							PropertyFloat4(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, Data _vDefault, Data _vInc, Data _vMin, Data _vMax)
							: PropertyVector(zenConst::keAssProp_Float4, "(%f,%f,%f,%f)", _zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _vDefault, _vInc, _vMin, _vMax){}	
	};

	//=============================================================================================
	// Property : Int
	//=============================================================================================
	class PropertyInt : public PropertyBase
	{
	ZENClassDeclare(PropertyInt, PropertyBase)
	public:
		typedef zI32		Data;		
							PropertyInt(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, Data _Default, Data _Inc=1, Data _Min=0.f, Data _Max=1.f );
		Data				mDefault;
		Data				mValMin;
		Data				mValMax;
		Data				mValInc;
	protected:
		virtual zUInt		ToString	(const void* _pValue, zUInt _uLen, char* _zOutString)const;
		virtual bool		IsDefault	(const void* _pValue)const;	
	};

	//=============================================================================================
	// Property : Int2,3,4
	//=============================================================================================
	class PropertyInt2 : public PropertyVector<zVec2S32>
	{
	public:
							PropertyInt2(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, Data _vDefault, Data _vInc, Data _vMin, Data _vMax)
							: PropertyVector(zenConst::keAssProp_Int2, "(%i,%i)", _zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _vDefault, _vInc, _vMin, _vMax){}	
	};
	class PropertyInt3 : public PropertyVector<zVec3S32>
	{
	public:
							PropertyInt3(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, Data _vDefault, Data _vInc, Data _vMin, Data _vMax)
							: PropertyVector(zenConst::keAssProp_Int3, "(%i,%i,%i)", _zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _vDefault, _vInc, _vMin, _vMax){}	
	};
	class PropertyInt4 : public PropertyVector<zVec4S32>
	{
	public:
							PropertyInt4(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, Data _vDefault, Data _vInc, Data _vMin, Data _vMax)
							: PropertyVector(zenConst::keAssProp_Int4, "(%i,%i,%i,%i)", _zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _vDefault, _vInc, _vMin, _vMax){}	
	};

	//=============================================================================================
	// Property : File
	//=============================================================================================
	class PropertyFile : public PropertyBase
	{
	ZENClassDeclare(PropertyFile, PropertyBase)
	public:
		typedef zString	Data;		
							PropertyFile(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, const char* _zDefault, const char* _zFileExt="Any(*.*)|*.*" );

		Data				mDefault;
		const char*			mzFileExt;	//!< List of supported files extensions to display in file dialog
	protected:
		virtual zUInt		ToString	(const void* _pValue, zUInt _uLen, char* _zOutString)const;
		virtual bool		IsDefault	(const void* _pValue)const;	
	};

	//=============================================================================================
	// Property : Enum
	//=============================================================================================
	/*class PropertyEnum : public PropertyBase
	{
	ZENClassDeclare(PropertyEnum, PropertyBase)
	public:
		struct Entry 
		{ 
			Entry(){}
			Entry(awU32 _uValue, const char* _zName, const char* _zDescription)
				: uValue(_uValue)
				, hKey(_zName)
				, zName(_zName)
				, zDescription(_zDescription)
			{
			}
			zI32		iValue; 
			zStringHash	
			zHash32		hKey; 
			const char* zName;
			const char* zDescription; 
		};

		typedef zI32		Data;		
		PropertyInt(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, Data _Default, Data _Inc=1, Data _Min=0.f, Data _Max=1.f );
		Data				mDefault;
		Data				mValMin;
		Data				mValMax;
		Data				mValInc;
	protected:
		virtual zUInt		ToString	(const void* _pValue, zUInt _uLen, char* _zOutString)const;
		virtual bool		IsDefault	(const void* _pValue)const;	
	};
	*/
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
		ZENInline zUInt								ToString	(zUInt _zLen, char* _zOutString)const;
		ZENInline bool								IsDefault	()const;
		void*										GetValue	(){return mpValue;}
		const void*									GetValue	()const{return mpValue;};
	#define ZEN_ASSETPROPERTIES_EXPAND_CODE(_TypeName_)																								\
	ZENInline bool									Is##_TypeName_()const			{return GetType()==zenConst::keAssProp_##_TypeName_##;}			\
	ZENInline const Property##_TypeName_##::Data&	GetValue##_TypeName_()const		{ZENAssert(Is##_TypeName_##()); return *mpValue##_TypeName_;}	\
	ZENInline Property##_TypeName_##::Data&			GetValue##_TypeName_()			{ZENAssert(Is##_TypeName_##()); return *mpValue##_TypeName_;}	\
	ZENInline const Property##_TypeName_&			GetProperty##_TypeName_()const	{ZENAssert(Is##_TypeName_##()); return *static_cast<const Property##_TypeName_*>(mpDefinition);}
	ZEN_ASSETPROPERTIES_EXPAND
	#undef	ZEN_ASSETPROPERTIES_EXPAND_CODE

	protected:		
		const PropertyBase*							mpDefinition;				
		union 
		{
			void*									mpValue;
			#define ZEN_ASSETPROPERTIES_EXPAND_CODE(_TypeName_) Property##_TypeName_::Data* mpValue##_TypeName_;		
			ZEN_ASSETPROPERTIES_EXPAND	
			#undef	ZEN_ASSETPROPERTIES_EXPAND_CODE
		};
		
	};

	typedef const zArrayStatic<const PropertyBase*> zArrayProperty;
}} //namespace zen { namespace zenAss

#endif

#include "zenAssProperty.inl"

#endif