
//==================================================================================================
//! @brief		Add new items to the list, sorted by a reference value
//! @note		The value used as a reference for comparison is based on its offset from the item base address. 
//!				This means :
//!				-# Item must inherit from awList1xNode
//!				-# All item most be of same type
//!				-# awPointer to compare data must be a member of the item to add (to be able to use it as an offset)
//!				This avoid using more costly virtual functions.
//--------------------------------------------------------------------------------------------------
//! @param[in]	_pAdd		- awPointer to Item to add
//! @param[in]	_pReference - awPointer to data used to compare
//==================================================================================================
template<class _Type_>
void awList1x::AddSort( awList1xNode* _pAdd,  _Type_* _pReference )
{
	awList1xNode*	pItemCur	= moFirst.LstNext();	// 1st element
	awList1xNode* pItemPrev	= &moFirst;
	awU32 uOffset				= (awU32)_pReference - (awU32)_pAdd;
	while( (pItemCur != GetInvalid() ) && *(_Type_*)((awU32)pItemCur+uOffset) < *_pReference )
	{
		pItemPrev   = pItemCur;
		pItemCur    = pItemCur->LstNext();
	}    
	InsertAfter(_pAdd, pItemPrev);
}

  

