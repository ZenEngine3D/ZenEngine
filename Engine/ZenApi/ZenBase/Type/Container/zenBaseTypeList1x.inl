#pragma once
#ifndef __zenBase_Type_List1x_inl__
#define __zenBase_Type_List1x_inl__

namespace zen {	namespace zenType 
{

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
	zList1xNode*	pItemCur	= moFirst.LstNext();	// 1st element
	zList1xNode* pItemPrev	= &moFirst;
	zU32 uOffset				= (zU32)_pReference - (zU32)_pAdd;
	while( (pItemCur != GetInvalid() ) && *(_Type_*)((zU32)pItemCur+uOffset) < *_pReference )
	{
		pItemPrev   = pItemCur;
		pItemCur    = pItemCur->LstNext();
	}    
	InsertAfter(_pAdd, pItemPrev);
}
  
#include "zenBaseTypeList1x.inl"

} } //namespace zen, Type

#endif