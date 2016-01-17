#pragma once
#ifndef __zenBase_Type_List2x_inl__
#define __zenBase_Type_List2x_inl__

namespace zen { namespace zenType 
{

//==================================================================================================
//! @brief		Add new items to the list, sorted by a reference value
//! @note		The value used as a reference for comparison is based on its offset from the item base address. 
//!				This means :
//!				-# Item must inherit from zList2xNode
//!				-# All item most be of same type
//!				-# void* to compare data must be a member of the item to add (to be able to use it as an offset)
//!				This avoid using more costly virtual functions.
//--------------------------------------------------------------------------------------------------
//! @param[in]	_pAdd		- void* to Item to add
//! @param[in]	_pReference - void* to data used to compare
//==================================================================================================
template<class _Type_>
void zList2x::AddSort( zList2xNode* _pAdd,  _Type_* _pReference )
{
	zList2xNode* pItemCur	= moRoot.LstNext();	// 1st element
	zList2xNode* pItemPrev	= &moRoot;
	zU32 uOffset			= static_cast<zU32>(reinterpret_cast<zUInt>(_pReference) - reinterpret_cast<zUInt>(_pAdd));
	while( (pItemCur != GetInvalid() ) && *(_Type_*)(reinterpret_cast<zUInt>(pItemCur)+uOffset) < *_pReference )
	{
		pItemPrev   = pItemCur;
		pItemCur    = pItemCur->LstNext();
	}    
	InsertAfter(pItemPrev, _pAdd);
}

  
template<class TItem, bool TAutoDel>
zList2xItem<TItem, TAutoDel>::zList2xItem(TItem* _pObject)
: mpObject( _pObject )
{
}

template<class TItem, bool TAutoDel>
zList2xItem<TItem, TAutoDel>::~zList2xItem(void)
{
	if( TAutoDel )
		zenDel( mpObject );
}

template<class TItem, bool TAutoDel>
TItem* zList2xItem<TItem, TAutoDel>::Get()const	
{ 
	return mpObject; 
}

} } //namespace zen, Type

#endif