#pragma once

#if ZEN_ENGINETOOL
namespace zeAss
{

//=================================================================================================
// CLASS: Asset
//=================================================================================================
class Asset : public zRefCounted, public zenSig::zSlot
{
zenClassDeclareNoParent(Asset)
public:
	virtual												~Asset();

	zenInline const zenAss::zAssetID&					GetID()const;
	zenInline zU64										GetIDUInt()const;
	zenInline const zString&							GetName()const;
	zenInline const zString&							GetDescription()const;
	zenInline zenConst::eAssetType						GetType()const;

	void												InitDefault();
	void												Init(zenAss::zAssetID _AssetID, const char* _zName, const zenAss::zPackageRef& _ParentPkg);
	void												SetPackage(const zenAss::zPackageRef& _rParentPkg);
	zenForceInline const zenAss::zPackageRef&			GetPackage(){ return mrPackage; }

	zenForceInline zUInt								GetValuesize()const;
	zenForceInline zenAss::PropertyValueRef				GetValue(zUInt _uValIndex);
	zenForceInline zenAss::PropertyValueRef				GetValue(zHash32 _hPropertyName);

	virtual const zenAss::PropertyDefArray&				GetProperties()const = 0;								//!< Child class return the list of property definition they are made of
	virtual zInt										GetValueIndex(zHash32 _hPropertyName)const = 0;
	
	void												SetName(const char* _zName);	
	
	typedef zenSig::zSignal1<zenAss::PropertyValueRef>							SignalUpdatedProperty;
	typedef zenSig::zSignal1<const zArrayDyn<zenAss::PropertyValueRef>& >	SignalUpdatedProperties;
	zenInline SignalUpdatedProperty&					GetSignalUpdatedProperty();
	zenInline SignalUpdatedProperties&					GetSignalUpdatedProperties();
protected:										
														Asset();
	void												RebuiltDescription();
	void												slotPropertyUpdate(zenAss::PropertyValueRef _rUpdated);		//!< Called when properties have been updated, if asset need to reprocess some local infos

	zenAss::zPackageRef									mrPackage;					//!< Parent package this asset is kept inside
	zenAss::zAssetID									mID;						//!< Unique ID for this Asset instance
	zString												mzName;						//!< Asset name
	zString												mzDescription;				//!< Asset description, built from propertydef/values
	zArrayDyn<zenAss::PropertyValueRef>					maPropertyValue;			//!< List of values reference of this asset
	zArrayDyn<zenAss::PropertyValueRef>					maPropertyUpdated;			//!< List of values updated during the frame
	zenSig::zSignalEmitter1<zenAss::PropertyValueRef>	msigUpdatedProperty;		//!< When a property has been updated.
	zenSig::zSignalEmitter1<const zArrayDyn<zenAss::PropertyValueRef>& >	msigUpdatedProperties;		//!< When a property has been updated.
	
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
zenClassDeclare(AssetTyped, Asset)
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
zenClassDeclare(TestProperty, Asset)
public:	
												TestProperty();
	virtual const zenAss::PropertyDefArray&		GetProperties()const;	
	void										slotPropertyUpdate(zenAss::PropertyValueRef _rUpdated);	
};	

}//namespace zeAss

#endif

#include "zeAssItem.inl"
