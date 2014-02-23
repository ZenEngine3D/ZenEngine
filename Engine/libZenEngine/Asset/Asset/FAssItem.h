#pragma once
#if !defined(__LibAsset_Asset_Item_h__) && AW_ENGINETOOL
#define __LibAsset_Asset_Item_h__

namespace FAss
{

class AssetItem
{
public:
enum enumType
{
	keType_Texture2D,
	keType_Mesh,
	keType__Count,
	keType__Invalid
};

AWClassDeclareNoParent(AssetItem)
public:
	virtual									~AssetItem();
	virtual bool							Load(const pugi::xml_node& _NodeAsset);
	virtual bool							Save(pugi::xml_document& _Doc);
	inline awHash64							GetID()const{return mhID;}
	inline const awArrayStatic<awString>&		GetGroupAndName()const{return maGroup;}
	inline awHash32							GetGroupID()const{return mhGroupID;}
	inline const awString&					GetName()const{return *maGroup.Last();}
	inline const awString&					GetDescription()const{return mzDescription;}
	inline const char*						GetTypeDesc()const {return GetTypeDesc(GetType());}
	inline awArrayStatic<FAss::ValuePointer>&	GetPropertyValues(){return maPropertyValue;}
	FAss::ValuePointer						GetPropertyValue(awHash32 _hPropertyName)const;
	void									ResetPropertyValues();

	virtual FAss::AssetItem::enumType		GetType()const=0;									//!< Child class return the Asset type they represent
	virtual FAss::PropertyArray&			GetPropertyDef()const=0;							//!< Child class return the list of property definition they are made of
	virtual awUInt							GetPropertyDefIndex(awHash32 _hPropertyName)const=0;	//!< Child class return index of PropertyDef with same name (using dictionary)
protected:
											AssetItem();
	void									FreePropertyValues();
	void									RebuiltDescription();
	void									InitPropertyDefIndex(Hashmap<awUInt>::Key32& _dPropertyMap)const;
	class Package*							mpPackage;					//!< Parent package this asset is kept inside
	awHash64									mhID;						//!< Unique ID for this Asset instance
	awHash32									mhGroupID;					//!< Group ID of this asset
	awString									mzDescription;				//!< Asset description, built from propertydef/values
	awArrayStatic<awString>						maGroup;					//!< Asset belongs to a group hierarchy for easier finding of asset, like package (last element is asset name)
	awArrayStatic<FAss::ValuePointer>			maPropertyValue;			//!< Asset properties values
	friend class Package;

//-----------------------------------------------------------------------------
// Static
//-----------------------------------------------------------------------------
public:
	static AssetItem*						CreateItem	(enumType _eAssetType, Package& _Owner);
	static const char*						GetTypeDesc	(AssetItem::enumType _ePropertyType);
	static AssetItem::enumType				GetType		(awHash32 _hPropertyName);	
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

}

#endif