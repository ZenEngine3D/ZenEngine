#pragma once
#ifndef __LibZenEngine_Asset_Item_h__
#define __LibZenEngine_Asset_Item_h__

#if ZEN_ENGINETOOL
namespace zen { namespace zeAss
{
class AssetItem
{
public:

ZENClassDeclareNoParent(AssetItem)
public:
	//TEMP PUBLIC
												AssetItem();
	virtual										~AssetItem();
	virtual bool								Load(const pugi::xml_node& _NodeAsset);
	virtual bool								Save(pugi::xml_document& _Doc);
	//TEMP PUBLIC

	ZENInline zenHash64							GetID()const{return mhID;}
	ZENInline const zenArrayStatic<zenString>&	GetGroupAndName()const{return maGroup;}
	ZENInline zenHash32							GetGroupID()const{return mhGroupID;}
	ZENInline const zenString&					GetName()const{return *maGroup.Last();}
	ZENInline const zenString&					GetDescription()const{return mzDescription;}
	//ZENInline const char*						GetTypeDesc()const {return GetTypeDesc(GetType());}

	virtual zenConst::eAssetType				GetType()const=0;										//!< Child class return the Asset type they represent
	virtual const zenAss::PropertyArray&		GetProperties()const=0;									//!< Child class return the list of property definition they are made of
	virtual zenInt								GetValueIndex(zenHash32 _hPropertyName)const=0;

	void										InitDefault();
	
	ZENForceInline zenUInt						GetValueCount()	{	return maPropertyValue.Count(); }
	ZENForceInline zenAss::PropertyValue&		GetValue(zenUInt _uValIndex){ ZENAssert( _uValIndex< maPropertyValue.Count()); return maPropertyValue[_uValIndex];}
	
protected:										
	void										RebuiltDescription();
	bool										InitPropertyMap(zenMap<zenInt>::Key32& _dPropertyMap)const;
	
	class Package*								mpPackage;					//!< Parent package this asset is kept inside
	zenHash64									mhID;						//!< Unique ID for this Asset instance
	zenHash32									mhGroupID;					//!< Group ID of this asset
	zenString									mzDescription;				//!< Asset description, built from propertydef/values
	zenArrayStatic<zenString>					maGroup;					//!< Asset belongs to a group hierarchy for easier finding of asset, like package (last element is asset name)	
	zenArrayStatic<zenAss::PropertyValue>		maPropertyValue;			//!< List of values pointer for this asset	
	
	friend class Package;

//-----------------------------------------------------------------------------
// Static
//-----------------------------------------------------------------------------
public:
	static AssetItem*							CreateItem		(zenConst::eAssetType _eAssetType, Package& _Owner);
	static const char*							GetTypeName		(zenConst::eAssetType _ePropertyType);
	static zenConst::eAssetType					GetTypeFromName	(zenHash32 _hAssetName);	//Move elsewhere?
};

class TestProperty : public AssetItem
{
ZENClassDeclare(TestProperty, AssetItem)
public:
	virtual zenConst::eAssetType				GetType()const{return zenConst::keAssType_TestProperty;}
	virtual const zenAss::PropertyArray&		GetProperties()const;
	virtual zenInt								GetValueIndex(zenHash32 _hPropertyName)const;
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

}} //namespace zen { namespace zeAss

#endif
#endif