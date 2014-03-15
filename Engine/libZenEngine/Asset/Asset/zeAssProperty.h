#pragma once
#ifndef __LibZenEngine_Asset_Property_h__
#define __LibZenEngine_Asset_Property_h__

#if ZEN_ENGINETOOL
#include "Engine/ThirdParty/pugixml/src/pugixml.hpp"

namespace zen { namespace zeAss
{

#if 0

//=================================================================================================
// Common class declaration of all asset value definition
//=================================================================================================
#define AWPropertyDefDeclare(_PropertyDef_)																			\
	ZENClassDeclare(_PropertyDef_, TPropertyDefBase)																\
	public:																											\
		virtual PropertyDefBase::Value* Allocate()const{return zenNew(&sPoolAlloc)Value(*this);}					\
		virtual void ValueToString(const PropertyDefBase::Value& _Value, char* _zBuffer, zUInt _uSizebuffer)const;\
	protected:																										\
		static zenMem::zAllocatorPool	sPoolAlloc;													


class PropertyDefBase
{
ZENClassDeclareNoParent(PropertyDefBase)
public:
//-----------------------------------------------------------------------------
// Base class for properties value
//-----------------------------------------------------------------------------
	class Value
	{	
	ZENClassDeclareNoParent(Value)
	public:		
		ZENInline void	ValueToString(char* _zBuffer, zUInt _uSizebuffer)const 
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

	virtual	zenConst::eAssetPropertyType		GetType()const=0;
	virtual Value*			Allocate()const=0;
	
	virtual bool			ValueFromXml	(PropertyDefBase::Value& _Value, const pugi::xml_node& _Property)const=0;	
	virtual void			ValueToString	(const PropertyDefBase::Value& _Value, char* _zBuffer, zUInt _uSizebuffer)const=0;
	virtual void			ValueToXml		(const PropertyDefBase::Value& _Value, pugi::xml_node& _ParamNode)const;	

	ZENInline const char*		GetTypeDesc		()const {return GetTypeDesc(GetType());}

	
	const char*				mzName;
	zHash32				mhName;
	const char*				mzDisplayName;
	const char*				mzDescription;
	bool					mbShowInAssetDesc;
#if AW_DEBUGINFOON
	zenConst::eAssetPropertyType				meType;
#endif

//-----------------------------------------------------------------------------
// Static methods
//-----------------------------------------------------------------------------
	static const char*					GetTypeDesc	(zenConst::eAssetPropertyType _ePropertyType);
	static zenConst::eAssetPropertyType	GetType(zHash32 _hPropertyName);
// 	static pugi::xml_node				ValueToXml	(const PropertyDefBase::Value& _Value, pugi::xml_node& _NodeParent )const;
// 	static bool							ValueFromXml(PropertyDefBase::Value& _Value, pugi::xml_node& _NodeParameter )const;
};

template<zenConst::eAssetPropertyType TEType, class TValue>
class TPropertyDefBase : public PropertyDefBase
{
ZENClassDeclare(TPropertyDefBase, PropertyDefBase)
public:
						class Value : public PropertyDefBase::Value
						{
						ZENClassDeclare(Value, PropertyDefBase::Value)
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
	virtual	zenConst::eAssetPropertyType	GetType			()const	{ return TEType; }		
	TValue				mDefault;		//!< Default value assigned when none present		
};

class PropertyDefBool : public TPropertyDefBase<zenConst::keAssProp_Bool, bool>
{
AWPropertyDefDeclare(PropertyDefBool)
public:
						PropertyDefBool(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, bool _bDefault)
						: TPropertyDefBase(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc,_bDefault )
						{}
	virtual bool		ValueFromXml(PropertyDefBase::Value& _Value, const pugi::xml_node& _NodeProperty)const;
};

class PropertyDefFile : public TPropertyDefBase<zenConst::keAssProp_File, zString>
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

#if 0
class PropertyDefInt : public TPropertyDefBase<zenConst::keAssProp_Int, zS32>
{
AWPropertyDefDeclare(PropertyDefInt)
public:
						PropertyDefInt(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, int _iDefault, int _iMin=0, int _iMax=100, zUInt _iIncrement=1.f)
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

class PropertyDefFloat : public TPropertyDefBase<zenConst::keAssProp_Float, float>
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

class PropertyDefEnum : public TPropertyDefBase<zenConst::keAssProp_Enum, zHash32>
{
AWPropertyDefDeclare(PropertyDefEnum)
public:
	struct Entry 
	{ 
		Entry(){}
		Entry(zU32 _uValue, const char* _zName, const char* _zDescription)
		: uValue(_uValue)
		, hKey(_zName)
		, zName(_zName)
		, zDescription(_zDescription)
		{
		}
		zU32 uValue; 
	 zHash32 hKey; 
		const char* zName;
		const char* zDescription; 
	};

						PropertyDefEnum(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, zHash32 _hDefaultKey, const Entry* _pEntries, zUInt _uEntryCount)
						: TPropertyDefBase( _zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _hDefaultKey )
						, maEntry(_pEntries, _uEntryCount)
						{
							ZENAssert(_uEntryCount > 0);
						}
	virtual bool		ValueFromXml	(PropertyDefBase::Value& _Value, const pugi::xml_node& _Property)const;
	zArrayStatic<Entry>	maEntry;
};

class PropertyDefFloat2 : public TPropertyDefBase<zenConst::keAssProp_Float2, zVec2F>
{
AWPropertyDefDeclare(PropertyDefFloat2)
public:
						PropertyDefFloat2(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, zVec2F _vfDefault, zVec2F _vfMin=0.f, zVec2F _vfMax=100.f, zVec2F _vfIncrement=1.f)
						: TPropertyDefBase(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _vfDefault )
						, mvfMin(_vfMin)
						, mvfMax(_vfMax)
						, mvfIncrement(_vfIncrement)
						{}
	virtual bool		ValueFromXml	(PropertyDefBase::Value& _Value, const pugi::xml_node& _Property)const;
	virtual void		ValueToXml		(const PropertyDefBase::Value& _Value, pugi::xml_node& _ParamNode)const;
	zVec2F			mvfMin;			//!< Minimum acceptable value
	zVec2F			mvfMax;			//!< Maximum acceptable value
	zVec2F			mvfIncrement;	//!< Increment value used when using ticker component 
};
#endif // 0

/*
class PropertyDefInt2 : public PropertyDefBase
{
ZENClassDeclare(PropertyDefInt2, PropertyDefBase)
public:
				PropertyDefInt2(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, zVec2S32 _mviDefault, int _iMin=0, int _iMax=100)
				: PropertyDefBase(keType_Int2, _zName, _zDisplayName, _zDescription, _bShowInAssetDesc )
				, mviDefault(_mviDefault)
				, miMin(_iMin)
				, miMax(_iMax)
				{}

	zVec2S32	mviDefault;		//!< Default value assigned when none present
	int			miMin;			//!< Minimum acceptable value
	int			miMax;			//!< Maximum acceptable value
};

class PropertyDefInt3 : public PropertyDefBase
{
ZENClassDeclare(PropertyDefInt3, PropertyDefBase)
public:
				PropertyDefInt3(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, zVec3S32 _mviDefault, int _iMin=0, int _iMax=100)
				: PropertyDefBase(keType_Int3, _zName, _zDisplayName, _zDescription, _bShowInAssetDesc )
				, mviDefault(_mviDefault)
				, miMin(_iMin)
				, miMax(_iMax)
				{}

	zVec3S32	mviDefault;		//!< Default value assigned when none present
	int			miMin;			//!< Minimum acceptable value
	int			miMax;			//!< Maximum acceptable value
};

class PropertyDefInt4 : public PropertyDefBase
{
ZENClassDeclare(PropertyDefInt4, PropertyDefBase)
public:
				PropertyDefInt4(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, zVec4S32 _mviDefault, int _iMin=0, int _iMax=100)
				: PropertyDefBase(keType_Int4, _zName, _zDisplayName, _zDescription, _bShowInAssetDesc )
				, mviDefault(_mviDefault)
				, miMin(_iMin)
				, miMax(_iMax)
				{}

	zVec4S32	mviDefault;		//!< Default value assigned when none present
	int			miMin;			//!< Minimum acceptable value
	int			miMax;			//!< Maximum acceptable value
};

class PropertyDefFloat3 : public PropertyDefBase
{
ZENClassDeclare(PropertyDefFloat3, PropertyDefBase)
public:
				PropertyDefFloat3(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, zVec3F _vfDefault, float _fMin=0.f, float _fMax=100.f, float _fIncrement=1.f)
				: PropertyDefBase(keType_Float3, _zName, _zDisplayName, _zDescription, _bShowInAssetDesc )
				, mvfDefault(_vfDefault)
				, mfMin(_fMin)
				, mfMax(_fMax)
				, mfIncrement(_fIncrement)
				{}

	zVec3F	mvfDefault;		//!< Default value assigned when none present
	float		mfMin;			//!< Minimum acceptable value
	float		mfMax;			//!< Maximum acceptable value
	float		mfIncrement;	//!< Increment value used when using ticker component 
};

class PropertyDefFloat4 : public PropertyDefBase
{
ZENClassDeclare(PropertyDefFloat4, PropertyDefBase)
public:
				PropertyDefFloat4(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, zVec4F _vfDefault, float _fMin=0.f, float _fMax=100.f, float _fIncrement=1.f)
				: PropertyDefBase(keType_Float4, _zName, _zDisplayName, _zDescription, _bShowInAssetDesc )
				, mvfDefault(_vfDefault)
				, mfMin(_fMin)
				, mfMax(_fMax)
				, mfIncrement(_fIncrement)
				{}

	zVec4F	mvfDefault;		//!< Default value assigned when none present
	float		mfMin;			//!< Minimum acceptable value
	float		mfMax;			//!< Maximum acceptable value
	float		mfIncrement;	//!< Increment value used when using ticker component 
};*/

class ValuePointer
{
public:
											ValuePointer():mpValueBase(NULL)			{											}
											ValuePointer(PropertyDefBase::Value* _pValue):mpValueBase(_pValue){						}
	ValuePointer&							operator=(PropertyDefBase::Value* _pValue)	{ mpValueBase = _pValue; return *this;		}
	zenConst::eAssetPropertyType			GetType() const								{ return mpValueBase->mParentDef.GetType(); }
	void									Set(PropertyDefBase::Value* _pValue)		{ mpValueBase = _pValue;					}
	PropertyDefBase::Value*					GetBase()									{ return mpValueBase;						}
	const PropertyDefBase::Value*			GetBase()const								{ return mpValueBase;						}
	
#define ZEN_ASSETPROPERTIES_EXPAND_CODE(_TypeName_)																								\
	bool									Is##_TypeName_() const			{return GetType()==zenConst::keAssProp_##_TypeName_;}		\
	PropertyDef##_TypeName_##::Value*		Get##_TypeName_()					{ZENAssert(Is##_TypeName_##()); return mpValue##_TypeName_;}	\
	const PropertyDef##_TypeName_##::Value*	Get##_TypeName_()const			{ZENAssert(Is##_TypeName_##()); return mpValue##_TypeName_;}
	ZEN_ASSETPROPERTIES_EXPAND
#undef	ZEN_ASSETPROPERTIES_EXPAND_CODE

protected:
	#define ZEN_ASSETPROPERTIES_EXPAND_CODE(_TypeName_) PropertyDef##_TypeName_::Value* mpValue##_TypeName_;
	union
	{
		PropertyDefBase::Value*	mpValueBase;	
		ZEN_ASSETPROPERTIES_EXPAND	
	};
	#undef	ZEN_ASSETPROPERTIES_EXPAND_CODE
};

typedef const zArrayStatic<const PropertyDefBase*> PropertyArray;

#endif

}} //namespace zen { namespace zeAss

#endif
#endif