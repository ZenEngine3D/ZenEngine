#include "libZenBase.h"

namespace zen { namespace awtype {

//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
awList1x::awList1x( void )
{
	moFirst.mpLstNext = NULL;
}

//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
awList1x::~awList1x( void )
{
	Clear();
}

//==================================================================================================
//! @brief		Remove every items from the list.
//--------------------------------------------------------------------------------------------------
//! @param[in]	_bDelete - If true, will also delete each item.
//==================================================================================================
void awList1x::Clear( bool _bDelete )
{
	if( _bDelete )
	{
		while( !IsEmpty() )
		{
			awList1xNode* pItemDel	= moFirst.LstNext();
			moFirst.mpLstNext		= pItemDel->LstNext();
			delete pItemDel;
		}
	}
	else
	{
		while( !IsEmpty() )
		{
			awList1xNode* pItemDel	= moFirst.LstNext();
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
void awList1x::InsertAfter( awList1xNode* AWRestrict _pAdd, awList1xNode* AWRestrict _pItem )
{
	AWAssert(_pAdd && _pItem && _pAdd != _pItem);
	AWAssertMsg(_pAdd->mpLstNext == NULL, "Remove item from its list before adding it to a new one");
	_pAdd->mpLstNext	= _pItem->mpLstNext;	
	_pItem->mpLstNext	= _pAdd;
}

//==================================================================================================
//! @brief		Return the first item of the list.
//!-----------------------------------------------------------------------------
//! @return		Next item. Equal to GetInvalid() if last item of the list.
//==================================================================================================
awList1xNode*	awList1xNode::LstNext()
{
	return mpLstNext;
}

//==================================================================================================
//! @brief		Return the first item of the list.
//--------------------------------------------------------------------------------------------------
//! @return		First item of the list (check if == GetInvalid() for empty list).
//==================================================================================================
awList1xNode*	awList1x::GetHead() const
{
	return moFirst.mpLstNext;
}

//==================================================================================================
//! @brief		Return pointer to the invalid item of this list.
//! @details	Use to detect iterating past the last element of the list.
//--------------------------------------------------------------------------------------------------
//! @return Value of the invalid item in this list.
//==================================================================================================
const awList1xNode* awList1x::GetInvalid() const
{
	return NULL;
}

//==================================================================================================
//! @brief		Return true if there no item in the list.
//--------------------------------------------------------------------------------------------------
//! @return		True when list is empty
//==================================================================================================
bool awList1x::IsEmpty() const
{
	return GetHead() == GetInvalid();
}

//==================================================================================================
//! @brief		Remove the first item of the list and return it.
//--------------------------------------------------------------------------------------------------
//! @return		Item removed
//==================================================================================================
awList1xNode*	awList1x::PopHead()
{
	awList1xNode* pItem	= moFirst.mpLstNext;
	moFirst.mpLstNext	= moFirst.mpLstNext ? moFirst.mpLstNext->mpLstNext : &moFirst;
	return pItem;
}

//==================================================================================================
//! @brief		Remove the item from the list.
//--------------------------------------------------------------------------------------------------
//! @return		True if item found and then removed
//==================================================================================================
bool awList1x::Remove( awList1xNode* _pRemove )
{
	awList1xNode* pItemCur	= &moFirst;
	awList1xNode* pItemPrev	= NULL;
	while( pItemCur )
	{
		pItemPrev	= pItemCur;
		pItemCur	= pItemCur->mpLstNext;
		if( pItemCur == _pRemove)
		{
			pItemPrev->mpLstNext	= pItemCur->mpLstNext;
			pItemCur				= NULL;
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
void awList1x::AddHead( awList1xNode* _pAdd	)
{
	_pAdd->mpLstNext	= moFirst.LstNext();
	moFirst.mpLstNext	= _pAdd;
};

} } //namespace zen, Type
