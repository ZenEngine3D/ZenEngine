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
	AssetItem();//TEMP PUBLIC
	virtual									~AssetItem();
	virtual bool							Load(const pugi::xml_node& _NodeAsset);
	virtual bool							Save(pugi::xml_document& _Doc);
	ZENInline zenHash64						GetID()const{return mhID;}
	ZENInline const zenArrayStatic<zenString>&	GetGroupAndName()const{return maGroup;}
	ZENInline zenHash32						GetGroupID()const{return mhGroupID;}
	ZENInline const zenString&					GetName()const{return *maGroup.Last();}
	ZENInline const zenString&					GetDescription()const{return mzDescription;}
	ZENInline const char*						GetTypeDesc()const {return GetTypeDesc(GetType());}
	ZENInline zenArrayStatic<ValuePointer>&	GetPropertyValues(){return maPropertyValueOld;}
	ValuePointer							GetPropertyValue(zenHash32 _hPropertyName)const;
	void									ResetPropertyValues();

	virtual zenConst::eAssetType			GetType()const=0;										//!< Child class return the Asset type they represent
	virtual PropertyArray&					GetPropertyDef()const=0;								//!< Child class return the list of property definition they are made of
	virtual zenUInt							GetPropertyDefIndex(zenHash32 _hPropertyName)const=0;	//!< Child class return index of PropertyDef with same name (using dictionary)

	virtual const zenAss::PropertyArray&	GetProperties()const=0;									//!< Child class return the list of property definition they are made of	
	void									ResetDefault();
	void									Allocate();
	ZENForceInline zenUInt					GetValueCount();
	ZENForceInline zenAss::PropertyValue&	GetValue(zenUInt _uValIndex);
protected:
											
	void									FreePropertyValues();
	void									RebuiltDescription();
	void									InitPropertyDefIndex(zenMap<zenUInt>::Key32& _dPropertyMap)const;
	
	class Package*							mpPackage;					//!< Parent package this asset is kept inside
	zenHash64								mhID;						//!< Unique ID for this Asset instance
	zenHash32								mhGroupID;					//!< Group ID of this asset
	zenString								mzDescription;				//!< Asset description, built from propertydef/values
	zenArrayStatic<zenString>				maGroup;					//!< Asset belongs to a group hierarchy for easier finding of asset, like package (last element is asset name)	
	zenArrayStatic<ValuePointer>			maPropertyValueOld;			//!< 
	zenArrayStatic<zenAss::PropertyValue>	maPropertyPointer;			//!< List of values pointer for this asset	
	
	friend class Package;

//-----------------------------------------------------------------------------
// Static
//-----------------------------------------------------------------------------
public:
	static AssetItem*						CreateItem	(zenConst::eAssetType _eAssetType, Package& _Owner);
	static const char*						GetTypeDesc	(zenConst::eAssetType _ePropertyType);
	static zenConst::eAssetType				GetType		(zenHash32 _hPropertyName);	
};
	
class TestProperty : public AssetItem
{
ZENClassDeclare(TestProperty, AssetItem)
public:
	virtual zenConst::eAssetType			GetType()const{return zenConst::keAssType_TestProperty;}
	virtual PropertyArray&					GetPropertyDef() const;		
	virtual zenUInt							GetPropertyDefIndex(zenHash32 _hPropertyName)const;	

	virtual const zenAss::PropertyArray&	GetProperties()const;
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