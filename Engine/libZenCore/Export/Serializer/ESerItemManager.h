#pragma once
#ifndef __LibExport_SerialItem_Manager_h__
#define __LibExport_SerialItem_Manager_h__

namespace EExp
{	
class ManagerSerialItem : public zbType::Manager
{
AWClassDeclare(ManagerSerialItem, zbType::Manager)
public:
										ManagerSerialItem();			
	void								SetItem(EExp::SerialItem* _pItem);
	
	SerialItem*							GetItemBase(const awResourceID _ResID);
	EExp::SerialItem*					GetItemBaseAnySource(const awResourceID _ResID);
	bool								IsValid(const awResourceID _ResID);
	bool								IsValid(const awArrayBase<awResourceID>& _aResID);
	template<class TType>
	TType* GetItem(const awResourceID _ResID)
	{
		//! @todo Add type check back, using mask
		SerialItem* pSerialItem = GetItemBase(_ResID);
		return static_cast<TType*>(pSerialItem);
	} 

	template<class TType>
	TType* GetItemAnySource(const awResourceID _ResID)
	{
		//! @todo Add type check back, using mask
		SerialItem* pSerialItem = GetItemBaseAnySource(_ResID);
		return static_cast<TType*>(pSerialItem);
	}

	
protected:
	Hashmap<EExp::SerialItem*>::Key64	mdSerialItems;		//!< Loaded and created serial items
	
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