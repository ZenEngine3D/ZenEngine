#pragma once
#ifndef __zCore_Exp_SerialItem_Manager_h__
#define __zCore_Exp_SerialItem_Manager_h__

namespace zcExp
{	
class ManagerSerialItem : public zbType::Manager
{
ZENClassDeclare(ManagerSerialItem, zbType::Manager)
public:
										ManagerSerialItem();			
	void								SetItem(zcExp::SerialItem* _pItem);
	
	SerialItem*							GetItemBase(const zResID _ResID);
	zcExp::SerialItem*					GetItemBaseAnySource(const zResID _ResID);
	bool								IsValid(const zResID _ResID);
	bool								IsValid(const zArrayBase<zResID>& _aResID);

	//! @todo clean should store and return ref objects
	template<class TType>
	TType* GetItem(const zResID _ResID)
	{
		//! @todo Important: Add type check back, (with resource mask supports)
		SerialItem* pSerialItem = GetItemBase(_ResID);
		return static_cast<TType*>(pSerialItem);
	} 

	template<class TType>
	TType* GetItemAnySource(const zResID _ResID)
	{
		//! @todo Important: Add type check back, (with resource mask supports)
		SerialItem* pSerialItem = GetItemBaseAnySource(_ResID);
		return static_cast<TType*>(pSerialItem);
	}

	
protected:
	zMap<zcExp::SerialItem*>::Key64	mdSerialItems;		//!< Loaded and created serial items
	
//---------------------------------------------------------
// ManagerBase Section
//---------------------------------------------------------
public:
	virtual	bool	Load();
	virtual	bool	Unload();

};	
}

namespace EMgr{ extern zcExp::ManagerSerialItem SerialItems; }

#endif