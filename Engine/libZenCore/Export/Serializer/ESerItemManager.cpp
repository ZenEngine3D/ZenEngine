#include "libZenCore.h"

namespace EMgr{ EExp::ManagerSerialItem SerialItems; }

namespace EExp
{
	
//=================================================================================================
//! @brief		Constructor
//=================================================================================================
ManagerSerialItem::ManagerSerialItem()
: mdSerialItems(256)
{	
}

//=================================================================================================
//! @brief		Load zbType::Manager
//! @details	Part of ManagerBase Interface.
//!-----------------------------------------------------------------------------
//! @return		True if init was successful
//=================================================================================================
bool ManagerSerialItem::Load()
{	
	mdSerialItems.SetDefaultValue(NULL);
	return true;
}

//=================================================================================================
//! @brief		Unload zbType::Manager
//! @details	Part of ManagerBase Interface.
//--------------------------------------------------------------------------------------------------
//! @return		True if unload was successful
//=================================================================================================
bool ManagerSerialItem::Unload()
{
	return true;
} 

//=================================================================================================
//! @brief		Store a new SerialItem 
//! @details	If a previous serial item existed, will remove it first
//=================================================================================================
void ManagerSerialItem::SetItem(EExp::SerialItem* _pItem)
{
	EExp::SerialItem* pOldSerialItem;
	if( mdSerialItems.SetReplace(_pItem->mResID.HashID(), _pItem, pOldSerialItem) )
	{
		// Item already existed, need to delete it
		//! @todo Actually take into account resource still using this item, this will crash if not taken care of
		zenDelNull(pOldSerialItem);
	}
} 

//=================================================================================================
//! @brief		
//! @details	
//-------------------------------------------------------------------------------------------------
//! @return 	
//=================================================================================================
SerialItem* ManagerSerialItem::GetItemBase(const zenResID _ResID)
{
	EExp::SerialItem* pSerialItem(NULL);
	mdSerialItems.Get( _ResID.HashID(), pSerialItem );
	return pSerialItem;
} 

//=================================================================================================
//! @brief		Find and return the SerialItem with a particular zenResID
//! @details	This version try to look in all possible SerialItem Sources, not limited to  
//!				the one provided in the zenResID parameter.
//!				Some generated zenResID can be created on the fly or already loaded, useful
//!				for those cases where we create a resource that depends on another one
//-------------------------------------------------------------------------------------------------
//! @param		_ResID				- zenResID to look for
//! @return 	zenPointer to SerialItem
//=================================================================================================
SerialItem* ManagerSerialItem::GetItemBaseAnySource(const zenResID _ResID)
{
	AWAssert(_ResID.IsValid());
	EExp::SerialItem* pSerialItem(NULL);
	zenResID anySourceResID(_ResID);

	anySourceResID.SetSource(awconst::keResSource_Loaded);
	if( mdSerialItems.Get(_ResID.HashID(), pSerialItem) )	
		return pSerialItem;

	anySourceResID.SetSource(awconst::keResSource_Runtime);	
	if( mdSerialItems.Get(_ResID.HashID(), pSerialItem) )	
		return pSerialItem;
	
	anySourceResID.SetSource(awconst::keResSource_Offline);	
	if( mdSerialItems.Get(_ResID.HashID(), pSerialItem) )	
		return pSerialItem;
		
	return NULL;	
}

//=================================================================================================
//! @brief		Tell us if the resource exist and is valid
//! @details	Useful at export, to ensure all resource reference are valid
//-------------------------------------------------------------------------------------------------
//! @param		_ResID				- zenResID to look for
//! @return 	true if valid
//=================================================================================================
bool ManagerSerialItem::IsValid(const zenResID _ResID)
{
	return mdSerialItems.Exist(_ResID.HashID());
}

//=================================================================================================
//! @brief		Tell us if the resources exist and are valid
//! @details	Useful at export, to ensure all resource reference are valid
//-------------------------------------------------------------------------------------------------
//! @param		_ResID				- zenResID to look for
//! @return 	true if valid
//=================================================================================================
bool ManagerSerialItem::IsValid(const zenArrayBase<zenResID>& _aResID)
{
	const zenResID* pResIdCur	= _aResID.First();
	const zenResID* pResIDEnd	= _aResID.Last()+1;
	while( pResIdCur < pResIDEnd )
	{
		if( !mdSerialItems.Exist(pResIdCur->HashID()) )
			return false;
		++pResIdCur;
	}
	return true;
}

}
