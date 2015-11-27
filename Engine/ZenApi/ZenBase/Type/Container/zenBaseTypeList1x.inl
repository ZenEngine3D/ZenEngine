#pragma once
#ifndef __zenBase_Type_List1x_inl__
#define __zenBase_Type_List1x_inl__

namespace zen {	namespace zenType 
{

//==================================================================================================
//! @brief		Return the first item of the list.
//!-----------------------------------------------------------------------------
//! @return		Next item. Equal to GetInvalid() if last item of the list.
//==================================================================================================
zList1xNode* zList1xNode::LstNext()
{
	return mpLstNext;
}

//==================================================================================================
//! @brief		Add new items to the list, sorted by a reference value
//! @note		The value used as a reference for comparison is based on its offset from the item base address. 
//!				This means :
//!				-# Item must inherit from zList1xNode
//!				-# All item most be of same type
//!				-# void* to compare data must be a member of the item to add (to be able to use it as an offset)
//!				This avoid using more costly virtual functions.
//--------------------------------------------------------------------------------------------------
//! @param[in]	_pAdd		- void* to Item to add
//! @param[in]	_pReference - void* to data used to compare
//==================================================================================================

template<class _Type_>
void zList1x::AddSort( zList1xNode* _pAdd,  _Type_* _pReference )
{
	zList1xNode* pItemCur	= moFirst.LstNext();	// 1st element
	zList1xNode* pItemPrev	= &moFirst;
	zI32 iOffset			= static_cast<zI32>(reinterpret_cast<zUInt>(_pReference) - reinterpret_cast<zUInt>(_pAdd));
	while( (pItemCur != GetInvalid() ) && *(_Type_*)(reinterpret_cast<zUInt>(pItemCur) + iOffset) < *_pReference )
	{
		pItemPrev   = pItemCur;
		pItemCur    = pItemCur->LstNext();
	}    
	InsertAfter(_pAdd, pItemPrev);
}
  
#include "zenBaseTypeList1x.inl"

} } //namespace zen, Type

#endif