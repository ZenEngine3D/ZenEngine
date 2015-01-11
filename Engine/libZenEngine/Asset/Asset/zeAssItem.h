#pragma once
#ifndef __LibZenEngine_Asset_Item_h__
#define __LibZenEngine_Asset_Item_h__


#if ZEN_ENGINETOOL
namespace zen { namespace zeAss
{

//=================================================================================================
// CLASS: Asset
//=================================================================================================
class Asset : public zRefCountedAutoDel, public zenSig::zSlot
{
ZENClassDeclareNoParent(Asset)
public:
	virtual											~Asset();

	ZENInline const zenAss::zAssetID&				GetID()const;
	ZENInline zU64									GetIDUInt()const;
	ZENInline const zString&						GetName()const;
	ZENInline const zString&						GetDescription()const;
	ZENInline zenConst::eAssetType					GetType()const;

	void											InitDefault();
	void											Init(zenAss::zAssetID _AssetID, const char* _zName, const zenAss::zPackageRef& _ParentPkg);
	void											SetPackage(const zenAss::zPackageRef& _rParentPkg);
	ZENForceInline const zenAss::zPackageRef&		GetPackage(){ return mrPackage; }

	ZENForceInline zUInt							GetValueCount()const;
	ZENForceInline zenAss::PropertyValueRef			GetValue(zUInt _uValIndex);
	ZENForceInline zenAss::PropertyValueRef			GetValue(zHash32 _hPropertyName);

	virtual const zenAss::PropertyDefArray&			GetProperties()const = 0;								//!< Child class return the list of property definition they are made of
	virtual zInt									GetValueIndex(zHash32 _hPropertyName)const = 0;
	
	void											SetName(const char* _zName);	
	
	ZENInline zenSig::zSignal1<zenAss::PropertyValueRef>& GetSignalPropertyUpdate();	
protected:										
	Asset();
	void												RebuiltDescription();
	void												slotPropertyUpdate(zenAss::PropertyValueRef _rUpdated);		//!< Called when properties have been updated, if asset need to reprocess some local infos

	zenAss::zPackageRef									mrPackage;					//!< Parent package this asset is kept inside
	zenAss::zAssetID									mID;						//!< Unique ID for this Asset instance
	zString												mzName;						//!< Asset name
	zString												mzDescription;				//!< Asset description, built from propertydef/values
	zArrayStatic<zenAss::PropertyValueRef>				maPropertyValue;			//!< List of values pointer for this asset
	zenSig::zSignalEmitter1<zenAss::PropertyValueRef>	msigPropertyUpdate;			//!< When a property has been updated. 	
	friend class zenAss::PropertyValueRef;											//!< Needed to emit property update signals
//-----------------------------------------------------------------------------
// Static
//-----------------------------------------------------------------------------
public:
	static Asset*									CreateItem		(zenConst::eAssetType _eAssetType);
};

//=================================================================================================
// CLASS: AssetTyped
//=================================================================================================
template <zenConst::eAssetType AssetTypedType>
class AssetTyped : public Asset
{
ZENClassDeclare(AssetTyped, Asset)
public:
	enum { kAssetType = AssetTypedType };
	virtual zInt								GetValueIndex(zHash32 _hPropertyName)const;	
protected:
												AssetTyped();
};

//=================================================================================================
// CLASS: TestProperty
//=================================================================================================
class TestProperty : public AssetTyped<zenConst::keAssType_TestProperty>
{
ZENClassDeclare(TestProperty, Asset)
public:	
												TestProperty();
	virtual const zenAss::PropertyDefArray&		GetProperties()const;	
	void										slotPropertyUpdate(zenAss::PropertyValueRef _rUpdated);	
};	

}} //namespace zen { namespace zeAss

#endif

#include "zeAssItem.inl"

#endif
