#include "zbBase.h"

namespace zen { namespace zenType {

//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
zList1x::zList1x( void )
{
	moFirst.mpLstNext = NULL;
}

//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
zList1x::~zList1x( void )
{
	Clear();
}

//==================================================================================================
//! @brief		Remove every items from the list.
//--------------------------------------------------------------------------------------------------
//! @param[in]	_bDelete - If true, will also delete each item.
//==================================================================================================
void zList1x::Clear( bool _bDelete )
{
	if( _bDelete )
	{
		while( !IsEmpty() )
		{
			zList1xNode* pItemDel	= moFirst.LstNext();
			moFirst.mpLstNext		= pItemDel->LstNext();
			delete pItemDel;
		}
	}
	else
	{
		while( !IsEmpty() )
		{
			zList1xNode* pItemDel	= moFirst.LstNext();
			moFirst.mpLstNext		= pItemDel->LstNext();
		}
	}
}

//==================================================================================================
//! @brief		Insert an item right after another one.
//--------------------------------------------------------------------------------------------------
//! @param[in]	_pAdd - Item to Add.
//! @param[in]	_pItem - Item position to insert after.
//==================================================================================================
void zList1x::InsertAfter( zList1xNode* ZENRestrict _pAdd, zList1xNode* ZENRestrict _pItem )
{
	ZENAssert(_pAdd && _pItem && _pAdd != _pItem);
	ZENAssertMsg(_pAdd->mpLstNext == NULL, "Remove item from its list before adding it to a new one");
	_pAdd->mpLstNext	= _pItem->mpLstNext;	
	_pItem->mpLstNext	= _pAdd;
}

//==================================================================================================
//! @brief		Return the first item of the list.
//--------------------------------------------------------------------------------------------------
//! @return		First item of the list (check if == GetInvalid() for empty list).
//==================================================================================================
zList1xNode* zList1x::GetHead() const
{
	return moFirst.mpLstNext;
}

//==================================================================================================
//! @brief		Return pointer to the invalid item of this list.
//! @details	Use to detect iterating past the last element of the list.
//--------------------------------------------------------------------------------------------------
//! @return Value of the invalid item in this list.
//==================================================================================================
const zList1xNode* zList1x::GetInvalid() const
{
	return nullptr;
}

//==================================================================================================
//! @brief		Return true if there no item in the list.
//--------------------------------------------------------------------------------------------------
//! @return		True when list is empty
//==================================================================================================
bool zList1x::IsEmpty() const
{
	return GetHead() == GetInvalid();
}

//==================================================================================================
//! @brief		Remove the first item of the list and return it.
//--------------------------------------------------------------------------------------------------
//! @return		Item removed
//==================================================================================================
zList1xNode*	zList1x::PopHead()
{
	zList1xNode* pItem	= moFirst.mpLstNext;
	moFirst.mpLstNext	= pItem ? pItem->mpLstNext : nullptr;
	return pItem;
}

//==================================================================================================
//! @brief		Remove the item from the list.
//--------------------------------------------------------------------------------------------------
//! @return		True if item found and then removed
//==================================================================================================
bool zList1x::Remove( zList1xNode* _pRemove )
{
	zList1xNode* pItemCur	= &moFirst;
	zList1xNode* pItemPrev	= nullptr;
	while( pItemCur )
	{
		pItemPrev	= pItemCur;
		pItemCur	= pItemCur->mpLstNext;
		if( pItemCur == _pRemove)
		{
			pItemPrev->mpLstNext	= pItemCur->mpLstNext;
			pItemCur				= nullptr;
			return true;
		}		
	}
	return false;
}

//==================================================================================================
//! @brief		Add an item to the head of the list.
//! @details	
//--------------------------------------------------------------------------------------------------
//! @param[in]	_pAdd - Item to add at the start of the list.
//==================================================================================================
void zList1x::AddHead( zList1xNode* _pAdd	)
{
	_pAdd->mpLstNext	= moFirst.LstNext();
	moFirst.mpLstNext	= _pAdd;
};

} } //namespace zen, Type
