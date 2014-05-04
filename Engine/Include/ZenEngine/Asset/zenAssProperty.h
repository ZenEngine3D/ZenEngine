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
	class PropertyEnum : public PropertyBase
	{
	ZENClassDeclare(PropertyEnum, PropertyBase)
	public:
		typedef zI32	Data;
		struct Entry 
		{ 
			Entry(){}
			Entry(Data _iValue, const char* _zName, const char* _zDescription)
			: miValue(_iValue)
			, mStrValue(_zName)
			, mzDescription(_zDescription)
			{
			}
			Data			miValue; 
			zStringHash32	mStrValue;
			zString			mzDescription;
		};
		
									PropertyEnum(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, Data _Default, const Entry* _pEntries, zUInt _uEntryCount );
		Data						mDefault;
		zArrayStatic<Entry>			maEnumValues;
		const Entry&				GetEnumEntry(Data _Value)const;
		const Entry&				GetEnumEntry(zHash32 _hValue)const;
		
	protected:
		zMap<zUInt>::Key32			mdKeyToIndex;
		zArraySparse<zUInt>::Key32	maValueToIndex;
		
		virtual zUInt				ToString	(const void* _pValue, zUInt _uLen, char* _zOutString)const;
		virtual bool				IsDefault	(const void* _pValue)const;	
	};
	
	//=============================================================================================
	// Property : Array
	//=============================================================================================
	/*
	class PropertyArray : public PropertyBase
	{
	ZENClassDeclare(PropertyArray, PropertyBase)
	public:
		typedef zArrayStatic<PropertyValue>	Data;		
		PropertyArray(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, const PropertyBase& _Property );

		PropertyBase&		mProperty;
				
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
		#define ZEN_ASSETPROPERTIES_EXPAND_CODE(_TypeName_)																									\
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
	

	//################################################################################################
	//################################################################################################
	//################################################################################################
	class PropertyValue2
	{
	public:
												PropertyValue2();
		virtual									~PropertyValue2();
		const PropertyValue2&					operator=( const PropertyValue2& _Copy );

		ZENInline const class PropertyDefinition* GetDefinition()const
		{
			return mpDefinition;
		}
		ZENInline const void* GetValue()const
		{
			return mpValue;
		}
		bool									IsValid()const;
		virtual bool							IsDefault()const;
		virtual bool							operator==(const PropertyValue2& _Cmp)const;
	protected:
		void									ReferenceDecrease();
												PropertyValue2(const class PropertyDefinition* _pParentDef, void* _pValue, zU32* _pRefCount);

	protected:
		const class PropertyDefinition*			mpDefinition;
		void*									mpValue;
		zU32*									mpRefCount;
		friend class PropertyDefinition;
		//	friend class ValueCommon;		
	};

	class PropertyDefinition
	{
	public:
												PropertyDefinition(){};
		virtual zenConst::eAssetPropertyType	GetType()const=0;
		virtual PropertyValue2					Allocate()const=0;
	protected:
		PropertyValue2							CreateValue(const PropertyDefinition* _pParentDef, void* _pValue, zU32* _pRefCount)const;
	};
	//################################################################################################
	template<zenConst::eAssetPropertyType TPropertyType, class TClassDefinition, class TClassStorage>
	class TPropertyValue : public PropertyValue2
	{
	public:
		TPropertyValue()
		{
			ZENDbgCode(mpValueCast		= static_cast<TClassStorage*>(mpValue));
			ZENDbgCode(mpDefinitionCast = static_cast<const TClassDefinition*>(mpDefinition));
		}

		TPropertyValue(const PropertyValue2& _Copy)
		{
			*this = _Copy;
		}

		ZENInline const TClassDefinition* GetDefinition()const
		{
			return static_cast<const TClassDefinition*>(mpDefinition);
		}

		ZENInline const TClassStorage* GetValue()const
		{
			return static_cast<const TClassStorage*>(mpValue);
		}

		ZENInline TPropertyValue& operator=( const TClassStorage& _Copy )
		{
			ZENAssert(IsValid());
			*static_cast<TClassStorage*>(mpValue) = _Copy;
			return *this;
		}

		ZENInline TPropertyValue& operator=( const PropertyValue2& _Copy )
		{
			ZENAssert( _Copy.GetDefinition()==NULL || _Copy.GetDefinition()->GetType() == TPropertyType );
			*static_cast<PropertyValue2*>(this)	= _Copy;
			ZENDbgCode(mpValueCast		= static_cast<TClassStorage*>(mpValue));
			ZENDbgCode(mpDefinitionCast = GetDefinition() );
			return *this;
		}

		ZENInline TPropertyValue& operator=( const TPropertyValue& _Copy )
		{	
			*static_cast<PropertyValue2*>(this)	= _Copy;
			ZENDbgCode(mpValueCast		= static_cast<TClassStorage*>(mpValue));
			ZENDbgCode(mpDefinitionCast = static_cast<const TClassDefinition*>(mpDefinition));
			return *this;
		}

		virtual bool operator==(const PropertyValue2& _Cmp)const
		{
			if( !IsValid() || !_Cmp.IsValid() )
				return IsValid() == _Cmp.IsValid();

			return	(mpDefinition == _Cmp.GetDefinition()) &&
					(*static_cast<TClassStorage*>(mpValue) == *static_cast<const TClassStorage*>(_Cmp.GetValue()));
		}

		virtual bool IsDefault() const
		{
			if( IsValid() )
			{
				const TClassDefinition*	pDefinitionCast = GetDefinition();
				const TClassStorage* pDataCast			= GetValue();
				return pDefinitionCast->mDefault == *pDataCast;
			}
			return false;		
		}

	protected:
		ZENDbgCode(TClassStorage*			mpValueCast);		//!< Usefull for debuging
		ZENDbgCode(const TClassDefinition*	mpDefinitionCast);	//!< Usefull for debuging
	};

	//################################################################################################
	template<zenConst::eAssetPropertyType TPropertyType, class TClassDefinition, class TClassValue>
	class TPropertyDefinition : public PropertyDefinition
	{
	public:
		typedef TClassValue													TypeStorage;
		typedef TPropertyValue<TPropertyType,TClassDefinition,TypeStorage>	TypeValue;

		TPropertyDefinition(){}

		virtual zenConst::eAssetPropertyType GetType() const
		{
			return TPropertyType;
		}

		PropertyValue2 Allocate() const
		{		
			const TClassDefinition* pDefinition = static_cast<const TClassDefinition*>(this);
			TypeStorage* pValue					= AllocateInternal();
			zU32* pRefCountMem					= reinterpret_cast<zU32*>(pValue + 1);
			*pValue								= pDefinition->mDefault;
			return CreateValue(pDefinition, pValue, pRefCountMem);
		}

	protected:
		static TypeStorage* AllocateInternal()
		{				
			static zenMem::zAllocatorPool sAllocPool( "TPropertyDefinition::Allocate", sizeof(TypeStorage)+sizeof(zU32), 256, 256 );
			return zenNew(&sAllocPool) TypeStorage();
		}
	};
	//################################################################################################
	class Property2Int : public TPropertyDefinition<zenConst::keAssProp_Int, Property2Int, int>
	{
	public:	
		Property2Int(TypeStorage default)	
		: TPropertyDefinition()
		, mDefault(default)
		{}

		TypeStorage mDefault;
	};

}} //namespace zen { namespace zenAss

#endif

#include "zenAssProperty.inl"

#endif