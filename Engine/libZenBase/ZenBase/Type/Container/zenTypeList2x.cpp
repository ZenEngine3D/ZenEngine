#include "libZenBase.h"

namespace zen { namespace zenType {

//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
zenList2xNode::zenList2xNode()
: mpLstNext(NULL)
, mpLstPrev(NULL)
{
}

//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
zenList2xNode::~zenList2xNode( void )
{
	LstRemove();
}

//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
zenList2x::zenList2x( void )
{
	moRoot.mpLstNext	= &moRoot;
	moRoot.mpLstPrev	= &moRoot;
}

//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
zenList2x::~zenList2x( void )
{
	Clear();
}

//==================================================================================================
//! @brief		Remove every items from the list.
//--------------------------------------------------------------------------------------------------
//! @param[in]	_bDelete - If true, will also delete each item.
//==================================================================================================
void zenList2x::Clear( bool _bDelete )
{
	if( _bDelete )
	{
		while( !IsEmpty() )
			delete GetHead();
	}
	else
	{
		while( !IsEmpty() )
			GetHead()->LstRemove();
	}
}

//==================================================================================================
//! @brief		Insert _n item right before another one.
//--------------------------------------------------------------------------------------------------
//! @param[in]	_pAdd - Item to Add.
//! @param[in]	_pItem - Item position to insert before.
//==================================================================================================
void zenList2x::InsertBefore( zenList2xNode* AWRestrict _pAdd, zenList2xNode* AWRestrict _pItem )
{       
	ZENAssert(_pAdd && _pItem && _pAdd != _pItem);
	_pAdd->LstRemove();	
	_pAdd->mpLstNext				= _pItem;	
	_pAdd->mpLstPrev				= _pItem->mpLstPrev;	
	_pItem->mpLstPrev				= _pAdd;
	_pAdd->mpLstPrev->mpLstNext		= _pAdd;
	
}

//==================================================================================================
//! @brief		Insert an item right after another one.
//--------------------------------------------------------------------------------------------------
//! @param[in]	_pAdd - Item to Add.
//! @param[in]	_pItem - Item position to insert after.
//==================================================================================================
void zenList2x::InsertAfter( zenList2xNode* AWRestrict _pAdd, zenList2xNode* AWRestrict _pItem )
{
	ZENAssert(_pAdd && _pItem && _pAdd != _pItem);
	_pAdd->LstRemove();	
	_pAdd->mpLstPrev				= _pItem;
	_pAdd->mpLstNext				= _pItem->mpLstNext;	
	_pItem->mpLstNext				= _pAdd;
	_pAdd->mpLstNext->mpLstPrev		= _pAdd;
}

//==================================================================================================
//! @brief		Return the first item of the list.
//!-----------------------------------------------------------------------------
//! @return		Next item. Equal to GetInvalid() if last item of the list.
//==================================================================================================
zenList2xNode*	zenList2xNode::LstNext()
{
	return mpLstNext;
}

//==================================================================================================
//! @brief		Return the first item of the list.
//!-----------------------------------------------------------------------------
//! @return		Previous item. Equal to GetInvalid() if first item of the list.
//==================================================================================================
zenList2xNode* zenList2xNode::LstPrev()
{
	return mpLstPrev;
}

//==================================================================================================
//! @brief		Remove the item from its parent list.
//==================================================================================================
void zenList2xNode::LstRemove()
{
	if( mpLstPrev )
	{
		mpLstPrev->mpLstNext	= mpLstNext;
		mpLstNext->mpLstPrev	= mpLstPrev;
		mpLstNext				= NULL;
		mpLstPrev				= NULL;
	}
}

//==================================================================================================
//! @brief		Remove the item from its parent list.
//==================================================================================================
void zenList2xNode::LstReset()
{
	mpLstNext	= NULL;
	mpLstPrev	= NULL;
}

//==================================================================================================
//! @brief		Return the first item of the list.
//--------------------------------------------------------------------------------------------------
//! @return		First item of the list (check if == GetInvalid() for empty list).
//==================================================================================================
zenList2xNode*	zenList2x::GetHead() const
{
	return moRoot.mpLstNext;
}

//==================================================================================================
//! @brief		Return the last item of the list.
//--------------------------------------------------------------------------------------------------
//! @return		Last item of the list (check if == GetInvalid() for empty list).
//==================================================================================================
zenList2xNode*	zenList2x::GetTail() const
{
	return moRoot.mpLstPrev;
}

//==================================================================================================
//! @brief		Return pointer to the invalid item of this list.
//! @details	Use to detect iterating past the last element of the list.
//--------------------------------------------------------------------------------------------------
//! @return Value of the invalid item in this list.
//==================================================================================================
const zenList2xNode* zenList2x::GetInvalid() const
{
	return &moRoot;
}

//==================================================================================================
//! @brief		Return true if there no item in the list.
//--------------------------------------------------------------------------------------------------
//! @return		True when list is empty
//==================================================================================================
bool zenList2x::IsEmpty()const
{
	return GetHead() == GetInvalid();
}

//==================================================================================================
//! @brief		Add an item to the head of the list.
//! @details	The item will be removed automatically from its current owner list before being added.
//--------------------------------------------------------------------------------------------------
//! @param[in]	_pAdd - Item to add at the start of the list.
//==================================================================================================
void zenList2x::AddHead( zenList2xNode* _pAdd	)
{
	InsertBefore(_pAdd, GetHead());
};

//==================================================================================================
//! @brief		Add an item to the end of the list.
//! @details	The item will be removed automatically from its current owner list before being added.
//--------------------------------------------------------------------------------------------------
//! @param[in]	_pAdd - Item to add at the end of the list.
//==================================================================================================
void zenList2x::AddTail( zenList2xNode* _pAdd	)
{
	InsertAfter(_pAdd, GetTail());
};

//==================================================================================================
//! @brief		Remove first item in the list, and return it
//! @details	
//--------------------------------------------------------------------------------------------------
//! @return		- First item in the list
//==================================================================================================
zenList2xNode*	zenList2x::PopHead( void )
{
	ZENAssert(!IsEmpty());
	zenList2xNode* pItem = moRoot.mpLstNext;
	pItem->LstRemove();
	return pItem;
}

//==================================================================================================
//! @brief		Remove last item in the list, and return it
//! @details	
//--------------------------------------------------------------------------------------------------
//! @return		- Last item in the list
//==================================================================================================
zenList2xNode*	zenList2x::PopTail( void )
{
	ZENAssert(!IsEmpty());
	zenList2xNode* pItem = moRoot.mpLstPrev;
	pItem->LstRemove();
	return pItem;
}

} } //namespace zen, Type
