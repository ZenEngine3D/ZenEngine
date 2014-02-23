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
	
	SerialItem*							GetItemBase(const zenResID _ResID);
	EExp::SerialItem*					GetItemBaseAnySource(const zenResID _ResID);
	bool								IsValid(const zenResID _ResID);
	bool								IsValid(const zenArrayBase<zenResID>& _aResID);
	template<class TType>
	TType* GetItem(const zenResID _ResID)
	{
		//! @todo Add type check back, using mask
		SerialItem* pSerialItem = GetItemBase(_ResID);
		return static_cast<TType*>(pSerialItem);
	} 

	template<class TType>
	TType* GetItemAnySource(const zenResID _ResID)
	{
		//! @todo Add type check back, using mask
		SerialItem* pSerialItem = GetItemBaseAnySource(_ResID);
		return static_cast<TType*>(pSerialItem);
	}

	
protected:
	zenMap<EExp::SerialItem*>::Key64	mdSerialItems;		//!< Loaded and created serial items
	
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