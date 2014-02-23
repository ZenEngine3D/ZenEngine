#pragma once
#ifndef __LibCore_Algo_Sort_h__
#define __LibCore_Algo_Sort_h__

namespace CAlg
{

//! @todo Improve quicksort by using buble sort on child 5-10 last elements
//! @todo Improve quicksort by using loop instead of recursive call (http://stackoverflow.com/questions/1688264/improving-the-quick-sort)
//! @todo implement radix sort
template<class TType>
void Quicksort(TType* _pFirst, TType* _pLast)
{
	#define Swap(VAL1,VAL2){ TType Temp(VAL1); VAL1=VAL2; VAL2=Temp;}
	if( _pFirst >= _pLast )
		return;

	TType* pPivot;
	TType* pItemLeft	= _pFirst;
	TType* pItemRight	= _pLast;
	TType* pItemMid		= _pFirst + (_pLast-_pFirst)/2;

	//Pick mid value between first, middle and last value	
	if(*pItemLeft > *pItemRight)
	{
		if(*pItemMid > *pItemLeft)		pPivot = pItemLeft;
		else if(*pItemMid < *pItemRight)pPivot = pItemRight;
		else							pPivot = pItemMid;
	}
	else
	{
		if(*pItemMid > *pItemRight)		pPivot = pItemRight;
		else if(*pItemMid < *pItemLeft)	pPivot = pItemLeft;
		else							pPivot = pItemMid;
	}

	//Store pivot at the end of the array
	Swap(*pPivot, *pItemRight);
	pPivot = pItemRight;

	//Place value either each side of the array, depending on their relation to pivot value
	--pItemRight;
	while( pItemLeft < pItemRight )
	{				
		if( !(*pItemLeft < *pPivot) && !(*pItemRight >= *pPivot ) )
			Swap(*pItemLeft, *pItemRight);				
		pItemLeft	+= (*pItemLeft < *pPivot) ? 1 : 0;
		pItemRight	-= (*pItemRight >= *pPivot )? 1 : 0;
	}
	//Put pivot value between left and right side, and recurse sorting on both array side
	pItemRight += (*pItemRight < *pPivot) ? 1 : 0;
	Swap(*pPivot, *pItemRight);
	Quicksort(_pFirst, pItemRight-1);
	Quicksort(pItemRight+1, _pLast);
}

}
#endif