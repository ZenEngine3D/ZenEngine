#pragma once
#ifndef __LibZenEngine_Asset_Item_h__
#define __LibZenEngine_Asset_Item_h__

#if ZEN_ENGINETOOL
namespace zen { namespace zeAss
{
class AssetItem : public zRefCountedAutoDel
{
public:

ZENClassDeclareNoParent(AssetItem)
public:
	virtual										~AssetItem();	

	ZENInline zHash64							GetID()const;
	ZENInline const zArrayStatic<zString>&		GetGroupAndName()const;
	ZENInline zHash32							GetGroupID()const;
	ZENInline const zString&					GetName()const;
	ZENInline const zString&					GetDescription()const;

	void										InitDefault();	
	void										Init(Package& _ParentPkg, const char* _zName, const char* _zGroup);
	void										SetPackage(Package* _pParentPkg);

	ZENForceInline zUInt						GetValueCount()const {	return maPropertyValue.Count(); }
	ZENForceInline zenAss::PropertyValue&		GetValue(zUInt _uValIndex){ ZENAssert( _uValIndex< maPropertyValue.Count()); return maPropertyValue[_uValIndex];}
	
	virtual zenConst::eAssetType				GetType()const=0;										//!< Child class return the Asset type they represent
	virtual const zenAss::PropertyArray&		GetProperties()const=0;									//!< Child class return the list of property definition they are made of
	virtual zInt								GetValueIndex(zHash32 _hPropertyName)const=0;
protected:										
												AssetItem();
	void										RebuiltDescription();
	bool										InitPropertyMap(zMap<zInt>::Key32& _dPropertyMap)const;
	
	zenAss::zPackage							mrPackage;					//!< Parent package this asset is kept inside
	zHash64										mhID;						//!< Unique ID for this Asset instance
	zHash32										mhGroupID;					//!< Group ID of this asset
	zString										mzDescription;				//!< Asset description, built from propertydef/values
	zArrayStatic<zString>						maGroup;					//!< Asset belongs to a group hierarchy for easier finding of asset, like package (last element is asset name)	
	zArrayStatic<zenAss::PropertyValue>			maPropertyValue;			//!< List of values pointer for this asset	
	
	friend class Package;

//-----------------------------------------------------------------------------
// Static
//-----------------------------------------------------------------------------
public:
	static AssetItem*							CreateItem		(zenConst::eAssetType _eAssetType);
};

class TestProperty : public AssetItem
{
ZENClassDeclare(TestProperty, AssetItem)
public:
	virtual zenConst::eAssetType				GetType()const{return zenConst::keAssType_TestProperty;}
	virtual const zenAss::PropertyArray&		GetProperties()const;
	virtual zInt								GetValueIndex(zHash32 _hPropertyName)const;
};	

extern const char* kzXmlName_Node_Package;
extern const char* kzXmlName_PkgAtr_Group;

extern const char* kzXmlName_Node_Asset;
extern const char* kzXmlName_AssetAtr_Group;
extern const char* kzXmlName_AssetAtr_Name;
extern const char* kzXmlName_AssetAtr_Type;

extern const char* kzXmlName_Node_Property;
extern const char* kzXmlName_PropAtr_Name;
extern const char* kzXmlName_PropAtr_Type;
extern const char* kzXmlName_PropAtr_Value;
}} //namespace zen { namespace zeAss

#endif

#include "zeAssItem.inl"

#endif