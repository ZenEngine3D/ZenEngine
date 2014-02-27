#pragma once
#if !defined(__LibAsset_Asset_Property_h__) && AW_ENGINETOOL
#define __LibAsset_Asset_Property_h__

#include "AWEngine/External/pugixml/src/pugixml.hpp"

namespace FAss
{
//! @note: When adding support for new parameter type in assets :
//!			1-Add name here
//!			2-Add matching ParameterDefinition class to support it, with same ending name
//!

//!	To use this macro, define 'AWAssetTypeExpand' to the content you want expanded with 
//! each supported parameter name, and then undef it.
#define AWAssetTypes	AWAssetTypeExpand(Bool)		\
						AWAssetTypeExpand(Int)		\
						AWAssetTypeExpand(Float)	\
						AWAssetTypeExpand(Float2)	\
 						AWAssetTypeExpand(Enum)		\
 						AWAssetTypeExpand(File)		
	
// 							AWAssetTypeExpand(Int2)		\
// 							AWAssetTypeExpand(Int3)		\
// 							AWAssetTypeExpand(Int4)		\						

// 							AWAssetTypeExpand(Float3)	\
// 							AWAssetTypeExpand(Float4)	\
// 							AWAssetTypeExpand(AssetRef)	

//=================================================================================================
// Common class declaration of all asset value definition
//=================================================================================================
#define AWPropertyDefDeclare(_PropertyDef_)																			\
	AWClassDeclare(_PropertyDef_, TPropertyDefBase)																	\
	public:																											\
		virtual PropertyDefBase::Value* Allocate()const{return zenNew(&sPoolAlloc)Value(*this);}						\
		virtual void ValueToString(const PropertyDefBase::Value& _Value, char* _zBuffer, zenUInt _uSizebuffer)const;	\
	protected:																										\
		static zenMem::AllocatorPool	sPoolAlloc;													


class PropertyDefBase
{
AWClassDeclareNoParent(PropertyDefBase)
public:
//-----------------------------------------------------------------------------
// Enum of resource types
//-----------------------------------------------------------------------------
	#define AWAssetTypeExpand(_TypeName_)	keType_##_TypeName_,
	enum enumType
	{
		AWAssetTypes
		keType__Count,
		keType__Invalid
	};
	#undef AWAssetTypeExpand
	
//-----------------------------------------------------------------------------
// Base class for properties value
//-----------------------------------------------------------------------------
	class Value
	{	
	AWClassDeclareNoParent(Value)
	public:		
		inline void	ValueToString(char* _zBuffer, zenUInt _uSizebuffer)const 
		{ 
			mParentDef.ValueToString( *this, _zBuffer, _uSizebuffer); 
		}
		pugi::xml_node	ValueToXml(pugi::xml_node& _NodeParent)const;
		bool			ValueFromXml(const pugi::xml_node& _NodeProperty);
		
		const PropertyDefBase& mParentDef;
	protected:
		Value(const PropertyDefBase& _ParentDef)
		: mParentDef(_ParentDef)
		{
		}
	};

//-----------------------------------------------------------------------------
// Property definition methods
//-----------------------------------------------------------------------------

							PropertyDefBase( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc )
							: mzName(_zName)	
							, mhName(_zName)
							, mzDisplayName( _zDisplayName && _zDisplayName[0] != NULL ? _zDisplayName : _zName )
							, mzDescription(_zDescription)
							, mbShowInAssetDesc(_bShowInAssetDesc)
							{ }
	virtual 				~PropertyDefBase(){}

	virtual	enumType		GetType()const=0;
	virtual Value*			Allocate()const=0;
	
	virtual bool			ValueFromXml	(PropertyDefBase::Value& _Value, const pugi::xml_node& _Property)const=0;	
	virtual void			ValueToString	(const PropertyDefBase::Value& _Value, char* _zBuffer, zenUInt _uSizebuffer)const=0;
	virtual void			ValueToXml		(const PropertyDefBase::Value& _Value, pugi::xml_node& _ParamNode)const;	

	inline const char*		GetTypeDesc		()const {return GetTypeDesc(GetType());}

	const char*				mzName;
	zenHash32			mhName;
	const char*				mzDisplayName;
	const char*				mzDescription;
	bool					mbShowInAssetDesc;
#if AW_DEBUGINFOON
	enumType				meType;
#endif

//-----------------------------------------------------------------------------
// Static methods
//-----------------------------------------------------------------------------
	static const char*					GetTypeDesc	(PropertyDefBase::enumType _ePropertyType);
	static PropertyDefBase::enumType	GetType(zenHash32 _hPropertyName);
// 	static pugi::xml_node				ValueToXml	(const PropertyDefBase::Value& _Value, pugi::xml_node& _NodeParent )const;
// 	static bool							ValueFromXml(PropertyDefBase::Value& _Value, pugi::xml_node& _NodeParameter )const;
};

template<PropertyDefBase::enumType TEType, class TValue>
class TPropertyDefBase : public PropertyDefBase
{
AWClassDeclare(TPropertyDefBase, PropertyDefBase)
public:
						class Value : public PropertyDefBase::Value
						{
						AWClassDeclare(Value, PropertyDefBase::Value)
						public:
							Value(const TPropertyDefBase& _ParentDef):PropertyDefBase::Value(_ParentDef), mValue(_ParentDef.mDefault){}
							TValue mValue;
						};

						TPropertyDefBase(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, TValue _Default)
						: PropertyDefBase(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc )
						, mDefault(_Default)
						{
						#if AW_DEBUGINFOON
							meType = TEType;
						#endif
						}	
	virtual	enumType	GetType			()const	{ return TEType; }		
	TValue				mDefault;		//!< Default value assigned when none present		
};

class PropertyDefBool : public TPropertyDefBase<PropertyDefBase::keType_Bool, bool>
{
AWPropertyDefDeclare(PropertyDefBool)
public:
						PropertyDefBool(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, bool _bDefault)
						: TPropertyDefBase(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc,_bDefault )
						{}
	virtual bool		ValueFromXml(PropertyDefBase::Value& _Value, const pugi::xml_node& _NodeProperty)const;
};

class PropertyDefInt : public TPropertyDefBase<PropertyDefBase::keType_Int, zenS32>
{
AWPropertyDefDeclare(PropertyDefInt)
public:
						PropertyDefInt(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, int _iDefault, int _iMin=0, int _iMax=100, zenUInt _iIncrement=1.f)
						: TPropertyDefBase(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _iDefault )
						, miMin(_iMin)
						, miMax(_iMax)
						, miIncrement(_iIncrement)
						{}
	virtual bool		ValueFromXml	(PropertyDefBase::Value& _Value, const pugi::xml_node& _Property)const;
	int					miMin;			//!< Minimum acceptable value
	int					miMax;			//!< Maximum acceptable value
	int					miIncrement;	//!< Increment value used when using ticker component 	
};

class PropertyDefFloat : public TPropertyDefBase<PropertyDefBase::keType_Float, float>
{
AWPropertyDefDeclare(PropertyDefFloat)
public:
						PropertyDefFloat(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, float _fDefault, float _fMin=0.f, float _fMax=100.f, float _fIncrement=1.f)
						: TPropertyDefBase(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _fDefault )
						, mfMin(_fMin)
						, mfMax(_fMax)
						, mfIncrement(_fIncrement)
						{}
	virtual bool		ValueFromXml	(PropertyDefBase::Value& _Value, const pugi::xml_node& _Property)const;
	float				mfMin;			//!< Minimum acceptable value
	float				mfMax;			//!< Maximum acceptable value
	float				mfIncrement;	//!< Increment value used when using ticker component 
};

class PropertyDefEnum : public TPropertyDefBase<PropertyDefBase::keType_Enum, zenHash32>
{
AWPropertyDefDeclare(PropertyDefEnum)
public:
	struct Entry 
	{ 
		Entry(){}
		Entry(zenU32 _uValue, const char* _zName, const char* _zDescription)
		: uValue(_uValue)
		, hKey(_zName)
		, zName(_zName)
		, zDescription(_zDescription)
		{
		}
		zenU32 uValue; 
	 zenHash32 hKey; 
		const char* zName;
		const char* zDescription; 
	};

						PropertyDefEnum(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, zenHash32 _hDefaultKey, const Entry* _pEntries, zenUInt _uEntryCount)
						: TPropertyDefBase( _zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _hDefaultKey )
						, maEntry(_pEntries, _uEntryCount)
						{
							AWAssert(_uEntryCount > 0);
						}
	virtual bool		ValueFromXml	(PropertyDefBase::Value& _Value, const pugi::xml_node& _Property)const;
	zenArrayStatic<Entry>	maEntry;
};

class PropertyDefFile : public TPropertyDefBase<PropertyDefBase::keType_File, zenString>
{
AWPropertyDefDeclare(PropertyDefFile)
public:
						PropertyDefFile(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, const char* _zDefault, const char* _ZFileExt="Any(*.*)|*.*")
						: TPropertyDefBase(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _zDefault)
						, mzFileExt(_ZFileExt)
						{}
	virtual bool		ValueFromXml	(PropertyDefBase::Value& _Value, const pugi::xml_node& _Property)const;
	const char*			mzFileExt;	// List of supported files extensions to display in file dialog
};

class PropertyDefFloat2 : public TPropertyDefBase<PropertyDefBase::keType_Float2, zenVec2F>
{
AWPropertyDefDeclare(PropertyDefFloat2)
public:
						PropertyDefFloat2(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, zenVec2F _vfDefault, zenVec2F _vfMin=0.f, zenVec2F _vfMax=100.f, zenVec2F _vfIncrement=1.f)
						: TPropertyDefBase(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _vfDefault )
						, mvfMin(_vfMin)
						, mvfMax(_vfMax)
						, mvfIncrement(_vfIncrement)
						{}
	virtual bool		ValueFromXml	(PropertyDefBase::Value& _Value, const pugi::xml_node& _Property)const;
	virtual void		ValueToXml		(const PropertyDefBase::Value& _Value, pugi::xml_node& _ParamNode)const;
	zenVec2F			mvfMin;			//!< Minimum acceptable value
	zenVec2F			mvfMax;			//!< Maximum acceptable value
	zenVec2F			mvfIncrement;	//!< Increment value used when using ticker component 
};


/*
class PropertyDefInt2 : public PropertyDefBase
{
AWClassDeclare(PropertyDefInt2, PropertyDefBase)
public:
				PropertyDefInt2(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, zenVec2S32 _mviDefault, int _iMin=0, int _iMax=100)
				: PropertyDefBase(keType_Int2, _zName, _zDisplayName, _zDescription, _bShowInAssetDesc )
				, mviDefault(_mviDefault)
				, miMin(_iMin)
				, miMax(_iMax)
				{}

	zenVec2S32	mviDefault;		//!< Default value assigned when none present
	int			miMin;			//!< Minimum acceptable value
	int			miMax;			//!< Maximum acceptable value
};

class PropertyDefInt3 : public PropertyDefBase
{
AWClassDeclare(PropertyDefInt3, PropertyDefBase)
public:
				PropertyDefInt3(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, zenVec3S32 _mviDefault, int _iMin=0, int _iMax=100)
				: PropertyDefBase(keType_Int3, _zName, _zDisplayName, _zDescription, _bShowInAssetDesc )
				, mviDefault(_mviDefault)
				, miMin(_iMin)
				, miMax(_iMax)
				{}

	zenVec3S32	mviDefault;		//!< Default value assigned when none present
	int			miMin;			//!< Minimum acceptable value
	int			miMax;			//!< Maximum acceptable value
};

class PropertyDefInt4 : public PropertyDefBase
{
AWClassDeclare(PropertyDefInt4, PropertyDefBase)
public:
				PropertyDefInt4(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, zenVec4S32 _mviDefault, int _iMin=0, int _iMax=100)
				: PropertyDefBase(keType_Int4, _zName, _zDisplayName, _zDescription, _bShowInAssetDesc )
				, mviDefault(_mviDefault)
				, miMin(_iMin)
				, miMax(_iMax)
				{}

	zenVec4S32	mviDefault;		//!< Default value assigned when none present
	int			miMin;			//!< Minimum acceptable value
	int			miMax;			//!< Maximum acceptable value
};

class PropertyDefFloat3 : public PropertyDefBase
{
AWClassDeclare(PropertyDefFloat3, PropertyDefBase)
public:
				PropertyDefFloat3(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, zenVec3F _vfDefault, float _fMin=0.f, float _fMax=100.f, float _fIncrement=1.f)
				: PropertyDefBase(keType_Float3, _zName, _zDisplayName, _zDescription, _bShowInAssetDesc )
				, mvfDefault(_vfDefault)
				, mfMin(_fMin)
				, mfMax(_fMax)
				, mfIncrement(_fIncrement)
				{}

	zenVec3F	mvfDefault;		//!< Default value assigned when none present
	float		mfMin;			//!< Minimum acceptable value
	float		mfMax;			//!< Maximum acceptable value
	float		mfIncrement;	//!< Increment value used when using ticker component 
};

class PropertyDefFloat4 : public PropertyDefBase
{
AWClassDeclare(PropertyDefFloat4, PropertyDefBase)
public:
				PropertyDefFloat4(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, zenVec4F _vfDefault, float _fMin=0.f, float _fMax=100.f, float _fIncrement=1.f)
				: PropertyDefBase(keType_Float4, _zName, _zDisplayName, _zDescription, _bShowInAssetDesc )
				, mvfDefault(_vfDefault)
				, mfMin(_fMin)
				, mfMax(_fMax)
				, mfIncrement(_fIncrement)
				{}

	zenVec4F	mvfDefault;		//!< Default value assigned when none present
	float		mfMin;			//!< Minimum acceptable value
	float		mfMax;			//!< Maximum acceptable value
	float		mfIncrement;	//!< Increment value used when using ticker component 
};*/

union ValuePointer
{
public:
											ValuePointer():mpValueBase(NULL)			{											}
											ValuePointer(PropertyDefBase::Value* _pValue):mpValueBase(_pValue){						}
	ValuePointer&							operator=(PropertyDefBase::Value* _pValue)	{ mpValueBase = _pValue; return *this;		}
	PropertyDefBase::enumType				GetType() const								{ return mpValueBase->mParentDef.GetType(); }
	void									Set(PropertyDefBase::Value* _pValue)		{ mpValueBase = _pValue;					}
	PropertyDefBase::Value*					GetBase()									{ return mpValueBase;						}
	const PropertyDefBase::Value*			GetBase()const								{ return mpValueBase;						}
	
#define AWAssetTypeExpand(_TypeName_)																											\
	bool									Is##_TypeName_##() const			{return GetType()==PropertyDefBase::keType_##_TypeName_##;}			\
	PropertyDef##_TypeName_##::Value*		Get##_TypeName_##()					{AWAssert(Is##_TypeName_##()); return mpValue##_TypeName_##;}	\
	const PropertyDef##_TypeName_##::Value*	Get##_TypeName_##()const			{AWAssert(Is##_TypeName_##()); return mpValue##_TypeName_##;}
	AWAssetTypes
#undef	AWAssetTypeExpand

protected:
	PropertyDefBase::Value*	mpValueBase;
	#define AWAssetTypeExpand(_TypeName_) PropertyDef##_TypeName_::Value* mpValue##_TypeName_;
	AWAssetTypes
	#undef	AWAssetTypeExpand
};

typedef const zenArrayStatic<const PropertyDefBase*> PropertyArray;
}

#endif