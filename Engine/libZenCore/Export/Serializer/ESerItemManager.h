#pragma once
#ifndef __LibExport_SerialItem_Manager_h__
#define __LibExport_SerialItem_Manager_h__

namespace EExp
{	
class ManagerSerialItem : public zbType::Manager
{
ZENClassDeclare(ManagerSerialItem, zbType::Manager)
public:
										ManagerSerialItem();			
	void								SetItem(EExp::SerialItem* _pItem);
	
	SerialItem*							GetItemBase(const zResID _ResID);
	EExp::SerialItem*					GetItemBaseAnySource(const zResID _ResID);
	bool								IsValid(const zResID _ResID);
	bool								IsValid(const zArrayBase<zResID>& _aResID);
	template<class TType>
	TType* GetItem(const zResID _ResID)
	{
		//! @todo Add type check back, using mask
		SerialItem* pSerialItem = GetItemBase(_ResID);
		return static_cast<TType*>(pSerialItem);
	} 

	template<class TType>
	TType* GetItemAnySource(const zResID _ResID)
	{
		//! @todo Add type check back, using mask
		SerialItem* pSerialItem = GetItemBaseAnySource(_ResID);
		return static_cast<TType*>(pSerialItem);
	}

	
protected:
	zMap<EExp::SerialItem*>::Key64	mdSerialItems;		//!< Loaded and created serial items
	
//---------------------------------------------------------
// ManagerBase Section
//---------------------------------------------------------
public:
	virtual	bool	Load();
	virtual	bool	Unload();

};	
}

namespace EMgr{ extern EExp::ManagerSerialItem SerialItems; }

#endif