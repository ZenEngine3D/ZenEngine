#pragma once
#ifndef __zenEngine_Asset_Item_h__
#define __zenEngine_Asset_Item_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{
	/*
class AssetBase
{
ZENClassDeclareNoParent(AssetBase)
public:
												AssetBase();//TEMP PUBLIC
	virtual										~AssetBase();
	ZENInline zenHash64							GetID()const;
	ZENInline const zenString&					GetName()const;
	ZENInline const zenArrayStatic<zenString>&	GetGroupAndName()const;
	ZENInline zenHash32							GetGroupID()const;	
	ZENInline const zenString&					GetDescription()const;
	ZENForceInline zenUInt						GetValueCount()const;
	ZENForceInline zenAss::PropertyValue&		GetValue(zenUInt _uValIndex);
	void										InitDefault();

	virtual zenConst::eAssetType				GetType()const=0;			//!< Child class return the Asset type they represent
	virtual const zenAss::PropertyArray&		GetProperties()const=0;		//!< Child class return the list of property definition they are made of	
	virtual void								RebuiltDescription();
protected:
	class Package*								mpPackage;					//!< Parent package this asset is kept inside
	zenHash64									mhID;						//!< Unique ID for this Asset instance
	zenHash32									mhGroupID;					//!< Group ID of this asset
	zenString									mzDescription;				//!< Asset description, built from propertydef/values
	zenArrayStatic<zenString>					maGroup;					//!< Asset belongs to a group hierarchy for easier finding of asset, like package (last element is asset name)	
	zenArrayStatic<zenAss::PropertyValue>		maPropertyValue;			//!< List of values pointer for this asset	
};

AssetBase* CreateItem(zenConst::eAssetType _eAssetType, Package& _Owner);
*/
}} //namespace zen { namespace zenAss

#endif

#include "zenAssItem.inl"

#endif