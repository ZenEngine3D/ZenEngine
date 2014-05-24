#pragma once
#ifndef __LibZenEngine_Asset_Item_h__
#define __LibZenEngine_Asset_Item_h__


#if ZEN_ENGINETOOL
namespace zen { namespace zeAss
{

//=================================================================================================
// CLASS: Asset
//=================================================================================================
class Asset : public zRefCountedAutoDel
{
ZENClassDeclareNoParent(Asset)
public:
	virtual										~Asset();	

	ZENInline zU32								GetID()const;
	ZENInline const zArrayStatic<zString>&		GetGroupAndName()const;
	ZENInline const zString&					GetName()const;
	ZENInline const zString&					GetDescription()const;

	void										InitDefault();	
	void										Init(zU32 _uID, const char* _zName, const char* _zGroup, Package& _ParentPkg);
	void										SetPackage(Package* _pParentPkg);

	ZENForceInline zUInt						GetValueCount()const {	return maPropertyValue.Count(); }
	ZENForceInline zenAss::PropertyValueRef		GetValue(zUInt _uValIndex){ ZENAssert( _uValIndex< maPropertyValue.Count()); return maPropertyValue[_uValIndex];}
	
	virtual zenConst::eAssetType				GetType()const=0;										//!< Child class return the Asset type they represent
	virtual const zenAss::PropertyDefArray&	GetProperties()const=0;									//!< Child class return the list of property definition they are made of
	virtual zInt								GetValueIndex(zHash32 _hPropertyName)const=0;
protected:										
												Asset();
	void										RebuiltDescription();

	
	zenAss::zPackage							mrPackage;					//!< Parent package this asset is kept inside
	zU32										muID;						//!< Unique ID for this Asset instance
	zString										mzDescription;				//!< Asset description, built from propertydef/values
	zArrayStatic<zString>						maGroup;					//!< Asset belongs to a group hierarchy for easier finding of asset, like package (last element is asset name)	
	zArrayStatic<zenAss::PropertyValueRef>		maPropertyValue;			//!< List of values pointer for this asset	

//-----------------------------------------------------------------------------
// Static
//-----------------------------------------------------------------------------
public:
	static Asset*								CreateItem		(zenConst::eAssetType _eAssetType);
};

//=================================================================================================
// CLASS: TAsset
//=================================================================================================
template <zenConst::eAssetType TAssetType>
class TAsset : public Asset
{
ZENClassDeclare(TAsset, Asset)
public:
	enum { kAssetType = TAssetType };
	virtual zenConst::eAssetType				GetType()const{return TAssetType;}	
	virtual zInt								GetValueIndex(zHash32 _hPropertyName)const;	
};

//=================================================================================================
// CLASS: TestProperty
//=================================================================================================
class TestProperty : public TAsset<zenConst::keAssType_TestProperty>
{
ZENClassDeclare(TestProperty, Asset)
public:	
	virtual const zenAss::PropertyDefArray&	GetProperties()const;	
};	

}} //namespace zen { namespace zeAss

#endif

#include "zeAssItem.inl"

#endif
