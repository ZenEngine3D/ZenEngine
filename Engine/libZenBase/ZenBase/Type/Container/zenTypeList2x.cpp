#include "libZenBase.h"

namespace zen { namespace awtype {

//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
awList2xNode::awList2xNode()
: mpLstNext(NULL)
, mpLstPrev(NULL)
{
}

//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
awList2xNode::~awList2xNode( void )
{
	LstRemove();
}

//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
awList2x::awList2x( void )
{
	moRoot.mpLstNext	= &moRoot;
	moRoot.mpLstPrev	= &moRoot;
}

//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
awList2x::~awList2x( void )
{
	Clear();
}

//==================================================================================================
//! @brief		Remove every items from the list.
//--------------------------------------------------------------------------------------------------
//! @param[in]	_bDelete - If true, will also delete each item.
//==================================================================================================
void awList2x::Clear( bool _bDelete )
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
void awList2x::InsertBefore( awList2xNode* AWRestrict _pAdd, awList2xNode* AWRestrict _pItem )
{       
	AWAssert(_pAdd && _pItem && _pAdd != _pItem);
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
void awList2x::InsertAfter( awList2xNode* AWRestrict _pAdd, awList2xNode* AWRestrict _pItem )
{
	AWAssert(_pAdd && _pItem && _pAdd != _pItem);
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
awList2xNode*	awList2xNode::LstNext()
{
	return mpLstNext;
}

//==================================================================================================
//! @brief		Return the first item of the list.
//!-----------------------------------------------------------------------------
//! @return		Previous item. Equal to GetInvalid() if first item of the list.
//==================================================================================================
awList2xNode* awList2xNode::LstPrev()
{
	return mpLstPrev;
}

//==================================================================================================
//! @brief		Remove the item from its parent list.
//==================================================================================================
void awList2xNode::LstRemove()
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
void awList2xNode::LstReset()
{
	mpLstNext	= NULL;
	mpLstPrev	= NULL;
}

//==================================================================================================
//! @brief		Return the first item of the list.
//--------------------------------------------------------------------------------------------------
//! @return		First item of the list (check if == GetInvalid() for empty list).
//==================================================================================================
awList2xNode*	awList2x::GetHead() const
{
	return moRoot.mpLstNext;
}

//==================================================================================================
//! @brief		Return the last item of the list.
//--------------------------------------------------------------------------------------------------
//! @return		Last item of the list (check if == GetInvalid() for empty list).
//==================================================================================================
awList2xNode*	awList2x::GetTail() const
{
	return moRoot.mpLstPrev;
}

//==================================================================================================
//! @brief		Return pointer to the invalid item of this list.
//! @details	Use to detect iterating past the last element of the list.
//--------------------------------------------------------------------------------------------------
//! @return Value of the invalid item in this list.
//==================================================================================================
const awList2xNode* awList2x::GetInvalid() const
{
	return &moRoot;
}

//==================================================================================================
//! @brief		Return true if there no item in the list.
//--------------------------------------------------------------------------------------------------
//! @return		True when list is empty
//==================================================================================================
bool awList2x::IsEmpty()const
{
	return GetHead() == GetInvalid();
}

//==================================================================================================
//! @brief		Add an item to the head of the list.
//! @details	The item will be removed automatically from its current owner list before being added.
//--------------------------------------------------------------------------------------------------
//! @param[in]	_pAdd - Item to add at the start of the list.
//==================================================================================================
void awList2x::AddHead( awList2xNode* _pAdd	)
{
	InsertBefore(_pAdd, GetHead());
};

//==================================================================================================
//! @brief		Add an item to the end of the list.
//! @details	The item will be removed automatically from its current owner list before being added.
//--------------------------------------------------------------------------------------------------
//! @param[in]	_pAdd - Item to add at the end of the list.
//==================================================================================================
void awList2x::AddTail( awList2xNode* _pAdd	)
{
	InsertAfter(_pAdd, GetTail());
};

//==================================================================================================
//! @brief		Remove first item in the list, and return it
//! @details	
//--------------------------------------------------------------------------------------------------
//! @return		- First item in the list
//==================================================================================================
awList2xNode*	awList2x::PopHead( void )
{
	AWAssert(!IsEmpty());
	awList2xNode* pItem = moRoot.mpLstNext;
	pItem->LstRemove();
	return pItem;
}

//==================================================================================================
//! @brief		Remove last item in the list, and return it
//! @details	
//--------------------------------------------------------------------------------------------------
//! @return		- Last item in the list
//==================================================================================================
awList2xNode*	awList2x::PopTail( void )
{
	AWAssert(!IsEmpty());
	awList2xNode* pItem = moRoot.mpLstPrev;
	pItem->LstRemove();
	return pItem;
}

} } //namespace zen, Type
