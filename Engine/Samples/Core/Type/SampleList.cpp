#include "zenEngine.h"
#include <list>
//=================================================================================================
//! @example SampleList.cpp
//! Informations on the usage of various list functionalities
//=================================================================================================

namespace sample
{

//==================================================================================================
//! @brief		Test the zList1x container
//!	@details	Test sorted add, removal of items, etc...
//==================================================================================================
class TestList1x : public zList1xNode
{
public:
	int miVal;	
};

//==================================================================================================
//! @brief		Test the zList2x container
//!	@details	Test sorted add, removal of items, etc...
//==================================================================================================
class TestList2x : public zList2xNode
{
public:
	int miVal;	
};

//==================================================================================================
//! @brief		Test the zList1x container
//!	@details	Test sorted add, removal of items, etc...
//==================================================================================================
void SampleList1x()
{	
	zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);
	zenIO::Log(zenConst::keLog_Game, " zList1x (single linked list) Sorting");
	zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);

	//-----------------------------------------------------
	// Create 5 items with increasing value
	//-----------------------------------------------------
	#define kuItemCount 6
	TestList1x	oItems[kuItemCount];
	zU32			uInsertOrder[kuItemCount]={3,0,5,1,4,2};
	zU32			uRemoveOrder[kuItemCount]={3,0,5,1,4,2};

	//-----------------------------------------------------
	// Add the item in random order, insert sorted add
	//-----------------------------------------------------
	zList1x moList;
	zenIO::Log(zenConst::keLog_Game, "Added   :");
	for(int i=0; i<kuItemCount; ++i)
	{
		zU32 uIndex = uInsertOrder[i];
		oItems[uIndex].miVal = uIndex;
		moList.AddSort<int>(&oItems[uIndex], &oItems[uIndex].miVal);
		zenIO::Printf(zenConst::keLog_Game, " %02i", uIndex);
	}
	
	//-----------------------------------------------------
	// Output the result, making sure it is sorted
	//-----------------------------------------------------
	zList1xNode* pItem = moList.GetHead();	
	zenIO::Log(zenConst::keLog_Game, "Sorted  :");
	while( pItem != moList.GetInvalid() )
	{
		zenIO::Printf(zenConst::keLog_Game, " %02i", static_cast<TestList1x*>(pItem)->miVal);
		pItem = pItem->LstNext();
	}
	
	//-----------------------------------------------------
	// Remove each item
	//-----------------------------------------------------
	for(int i=0; i<kuItemCount; ++i)
	{
		zU32 uIndex = uRemoveOrder[i];
		moList.Remove( &oItems[ uIndex ] );
		zenIO::Log(zenConst::keLog_Game, "Removed : (%02i) Left:", uIndex);
		zList1xNode* pItem = moList.GetHead();	
		while( pItem != moList.GetInvalid() )
		{
			zenIO::Printf(zenConst::keLog_Game, " %02i", static_cast<TestList1x*>(pItem)->miVal);
			pItem = pItem->LstNext();
		}		
	}

	zenIO::Printf(zenConst::keLog_Game, "\n");

	//-----------------------------------------------------
	// Remove all items from the list
	//-----------------------------------------------------
	moList.Clear();
}

//==================================================================================================
//! @brief		Test the zList2x container
//!	@details	Test sorted add, removal of items, etc...
//==================================================================================================
void SampleList2x()
{	
	zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);
	zenIO::Log(zenConst::keLog_Game, " zList2x (double linked list) Sorting");
	zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);

	//-----------------------------------------------------
	// Create 5 items with increasing value
	//-----------------------------------------------------
	#define kuItemCount 6
	TestList2x	oItems[kuItemCount];
	zU32			uInsertOrder[kuItemCount]={3,0,5,1,4,2};
	zU32			uRemoveOrder[kuItemCount]={3,0,5,1,4,2};

	//-----------------------------------------------------
	// Add the item in random order, insert sorted add
	//-----------------------------------------------------
	zList2x moList;
	zenIO::Log(zenConst::keLog_Game, "Added   :");
	for(int i=0; i<kuItemCount; ++i)
	{
		oItems[i].miVal = uInsertOrder[i];
		moList.AddSort<int>(&oItems[i], &oItems[i].miVal);
		zenIO::Printf(zenConst::keLog_Game, " %02i", uInsertOrder[i]);
	}

	//-----------------------------------------------------
	// Output the result, making sure it is sorted
	//-----------------------------------------------------
	zList2xNode* pItem = moList.GetHead();
	zenIO::Log(zenConst::keLog_Game, "Sorted  :");
	while( pItem != moList.GetInvalid() )
	{
		zenIO::Printf(zenConst::keLog_Game, " %02i", static_cast<TestList2x*>(pItem)->miVal);
		pItem = pItem->LstNext();
	}	

	//-----------------------------------------------------
	// Remove each item
	//-----------------------------------------------------
	for(int i=0; i<kuItemCount; ++i)
	{
		zU32 uIndex = uRemoveOrder[i];
		oItems[ uIndex ].LstRemove();
		zenIO::Log(zenConst::keLog_Game, "Removed : (%02i) Left:", uIndex);
		zList2xNode* pItem = moList.GetHead();	
		while( pItem != moList.GetInvalid() )
		{
			zenIO::Printf(zenConst::keLog_Game, " %02i", static_cast<TestList2x*>(pItem)->miVal);
			pItem = pItem->LstNext();
		}		
	}
	zenIO::Printf(zenConst::keLog_Game, "\n");

	
}

//==================================================================================================
//! @brief		Test the zListIntrusive container
//!	@details	Test sorted add, removal of items, etc...
//==================================================================================================

class IntrusiveListItem
{
public:
	IntrusiveListItem() {}
	IntrusiveListItem(int val):mValue(val){};
	
	int mValue = 0;

protected:
	zListLink mlnkList1;
//	zListLink mlnkList2;

public:
	typedef zList<IntrusiveListItem, &IntrusiveListItem::mlnkList1> TypeList1;
};

class IntrusiveListItemChild : public IntrusiveListItem
{
public:
	IntrusiveListItemChild(int val) : IntrusiveListItem(val), mValue2(val*2){};
	int mValue2;
};


class TestReferenceValue : public zRefCounted
{
public:
	int mValue = 0;
};

void SampleListIntrusive()
{
	zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);
	zenIO::Log(zenConst::keLog_Game, " zListIntrusive");
	zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);

	int valueInt								= 0;
	int* pValueInt								= &valueInt;
	zGameRef<TestReferenceValue> rValueSmart	= new TestReferenceValue;
	decltype(rValueSmart) rValue2				= rValueSmart;
	decltype(*rValueSmart) Value3				= *rValueSmart;
	decltype(*pValueInt) Value4					= *pValueInt;
	decltype((*pValueInt)) Value5				= *pValueInt;

	//-----------------------------------------------------
	// Create 5 items with increasing value
	//-----------------------------------------------------
	//#define kuItemCount 6
	//TestList2x	oItems[kuItemCount];
	//zU32		uInsertOrder[kuItemCount] = { 3, 0, 5, 1, 4, 2 };
	//zU32		uRemoveOrder[kuItemCount] = { 3, 0, 5, 1, 4, 2 };
	
	IntrusiveListItem::TypeList1	lstOne;
	IntrusiveListItem				item1(1);
	IntrusiveListItem				item2(2);
	IntrusiveListItem				item3(3);
	IntrusiveListItem				item4(4);
	IntrusiveListItemChild			item5Child(5);
	//---	
	lstOne.Empty();
	lstOne.PushHead(item1); //Making sure re-inserting works
	lstOne.PushHead(item5Child); //Making sure derived class list link works
	lstOne.PushHead(item4);
	lstOne.PushHead(item3);
	lstOne.PushHead(item2);
	lstOne.PushHead(item1);

#if 0
	IntrusiveListItem* pItem = lstOne.GetHead();
	zenIO::Log(zenConst::keLog_Game, "Add Head       : ");
	while (pItem)
	{
		printf("%02i ", pItem->mValue);
		pItem = pItem->mlnkList1.GetNext();
	}
	//---
	zenIO::Log(zenConst::keLog_Game, "Add Tail       : ");
	lstOne.UnlinkAll();
	lstOne.PushTail(item1);
	lstOne.PushTail(item2);
	lstOne.PushTail(item3);
	lstOne.PushTail(item4);
	lstOne.PushTail(item5Child);
	pItem = lstOne.GetHead();
	while( pItem )
	{
		printf("%02i ", pItem->mValue );
		pItem = pItem->mlnkList1.GetNext();
	}

	//---
	zenIO::Log(zenConst::keLog_Game, "Pop Head       : ");
	pItem = lstOne.PopHead();
	while (pItem)
	{
		printf("%02i ", pItem->mValue);
		pItem = lstOne.PopHead();
	}

	//---
	zenIO::Log(zenConst::keLog_Game, "Inserting      : ");
	lstOne.PushTail(item1);
	item1.mlnkList1.InsertAfter(item2);
	item2.mlnkList1.InsertAfter(item3);
	item3.mlnkList1.InsertAfter(item4);
	item4.mlnkList1.InsertAfter(item5Child);
	pItem = lstOne.GetHead();
	while( pItem )
	{
		printf("%02i ", pItem->mValue );
		pItem = pItem->mlnkList1.GetNext();
	}
	//---
	zenIO::Log(zenConst::keLog_Game, "Iterator back  : ");
	zList<IntrusiveListItem>::Iterator it = lstOne.GetTailIt();
	while( *it )
	{
		printf("%02i ", (*it)->mValue);
		--it;
	}	

	zenIO::Log(zenConst::keLog_Game, " ");
#endif

	const zUInt						kuTestCount = 1024*512;
	IntrusiveListItem*				aItemNode = zenNewDefault IntrusiveListItem[kuTestCount];
	TestList2x*						aItemChild = zenNewDefault TestList2x[kuTestCount];
	IntrusiveListItem::TypeList1	lstIntrusive;
	zList2x							lstChild;	

	for (int i = 0; i<kuTestCount; ++i)
		aItemNode[i].mValue = i;

	//----------------------------------------------------------------------------
	// Intrusive node
	//----------------------------------------------------------------------------
	zU64 uAddingHeadTimeIntr = zenSys::GetTimeUSec();
	for(int i=0; i<kuTestCount; ++i)
		lstIntrusive.PushHead(aItemNode[i]);
	uAddingHeadTimeIntr = zenSys::GetTimeUSec() - uAddingHeadTimeIntr;
	
	lstIntrusive.Empty();
	zU64 uAddingTailTimeIntr = zenSys::GetTimeUSec();
	for (int i = 0; i<kuTestCount; ++i)
		lstIntrusive.PushTail(aItemNode[i]);
	uAddingTailTimeIntr = zenSys::GetTimeUSec() - uAddingTailTimeIntr;

	lstIntrusive.Empty();
	zU64 uAddingMidTimeIntr = zenSys::GetTimeUSec();
	lstIntrusive.PushHead(aItemNode[0]);
	for (int i = 1; i<kuTestCount; ++i)
		lstIntrusive.GetHeadIt().InsertAfter(aItemNode[i]);
	uAddingMidTimeIntr = zenSys::GetTimeUSec() - uAddingMidTimeIntr;
	
	lstIntrusive.Empty();
	zU64 uAddingMidTimeNoIt = zenSys::GetTimeUSec();
	lstIntrusive.PushHead(aItemNode[0]);
	for (int i = 1; i<kuTestCount; ++i)
		IntrusiveListItem::TypeList1::InsertAfter(aItemNode[0], aItemNode[i]);
	uAddingMidTimeNoIt = zenSys::GetTimeUSec() - uAddingMidTimeNoIt;

	zU64 uTraverseFwndTimeIntr = zenSys::GetTimeUSec();
	auto intrIt = lstIntrusive.GetHeadIt();
	int val = 0;
	int i = 0;
	while( *intrIt )
	{
		val += intrIt->mValue;
		++intrIt;		
	}
	uTraverseFwndTimeIntr = zenSys::GetTimeUSec() - uTraverseFwndTimeIntr;
	
	zU64 uTraverseBackTimeIntr = zenSys::GetTimeUSec();
	intrIt = lstIntrusive.GetTailIt();
	while( *intrIt )
	{
		val += intrIt->mValue;
		--intrIt;
	}
	uTraverseBackTimeIntr = zenSys::GetTimeUSec() - uTraverseBackTimeIntr;	

	
	zU64 uTraverseFwndNoItTimeIntr = zenSys::GetTimeUSec();
	IntrusiveListItem* pCurrentItem = lstIntrusive.GetHead();
	while(pCurrentItem)
	{
		val += pCurrentItem->mValue;
		pCurrentItem = IntrusiveListItem::TypeList1::GetNext(*pCurrentItem);
	}
	uTraverseFwndNoItTimeIntr = zenSys::GetTimeUSec() - uTraverseFwndNoItTimeIntr;

	//----------------------------------------------------------------------------
	// Regular 2X list
	//----------------------------------------------------------------------------
	zU64 uAddingHeadTimeChild = zenSys::GetTimeUSec();
	for (int i = 0; i<kuTestCount; ++i)
	{
		//aItemChild[i].miVal = i;
		lstChild.AddHead(&aItemChild[i]);
	}
	uAddingHeadTimeChild = zenSys::GetTimeUSec() - uAddingHeadTimeChild;

	lstChild.Clear();
	zU64 uAddingTailTimeChild = zenSys::GetTimeUSec();
	for (int i = 0; i<kuTestCount; ++i)
		lstChild.AddTail(&aItemChild[i]);
	uAddingTailTimeChild = zenSys::GetTimeUSec() - uAddingTailTimeChild;

	lstChild.Clear();
	zU64 uAddingMidTimeChild = zenSys::GetTimeUSec();
	lstChild.AddHead(&aItemChild[0]);
	for (int i = 1; i<kuTestCount; ++i)
		lstChild.InsertAfter(lstChild.GetHead(), &aItemChild[i]);
	uAddingMidTimeChild = zenSys::GetTimeUSec() - uAddingMidTimeChild;

	zU64 uTraverseFwndTimeChild = zenSys::GetTimeUSec();
	const zList2xNode* pInvalid = lstChild.GetInvalid();
	zList2xNode* childIt = lstChild.GetHead();
	while( childIt != pInvalid )
	{		
		val += ((TestList2x*)childIt)->miVal;
		childIt = childIt->LstNext();
	}
	uTraverseFwndTimeChild = zenSys::GetTimeUSec() - uTraverseFwndTimeChild;

	zU64 uTraverseBackTimeChild = zenSys::GetTimeUSec();	
	childIt = lstChild.GetTail();
	i=0;
	while( ++i < kuTestCount /*childIt != pInvalid*/ )
	{
		val += ((TestList2x*)childIt)->miVal;
		childIt = childIt->LstPrev();
	}
	uTraverseBackTimeChild = zenSys::GetTimeUSec() - uTraverseBackTimeChild;

	//----------------------------------------------------------------------------
	// Regular 2X list
	//----------------------------------------------------------------------------
	std::list<IntrusiveListItem*> lstStd;
	zU64 uAddingHeadTimeStd = zenSys::GetTimeUSec();
	for (int i = 0; i<kuTestCount; ++i)	
		lstStd.push_front(&aItemNode[i]);
	uAddingHeadTimeStd = zenSys::GetTimeUSec() - uAddingHeadTimeStd;

	zU64 uTraverseFwndTimeStd = zenSys::GetTimeUSec();
	auto stdIt = lstStd.begin();
	auto stdEnd = lstStd.end();
	while (stdIt != stdEnd)
	{
		val += (*stdIt)->mValue;
		++stdIt;
	}
	uTraverseFwndTimeStd = zenSys::GetTimeUSec() - uTraverseFwndTimeStd;


	intrIt = lstIntrusive.GetHeadIt();
	++intrIt;
	++intrIt;
	val += intrIt->mValue;

	childIt = lstChild.GetHead();
	childIt = childIt->LstNext();
	childIt = childIt->LstNext();
	val += ((TestList2x*)childIt)->miVal;


	zenIO::Log(zenConst::keLog_Game, "%s", zenConst::kzLineA70);
	zenIO::Log(zenConst::keLog_Game, " List Performances (%i elements)", kuTestCount, val);
	zenIO::Log(zenConst::keLog_Game, "%s", zenConst::kzLineA70);
	zenIO::Log(zenConst::keLog_Game, "               Add Head  | Add Tail  | Add Mid  | Seek Next | Seek Prev | Seek NoIt | Add NoIt");
	zenIO::Log(zenConst::keLog_Game, "%s", zenConst::kzLineB70);
	zenIO::Log(zenConst::keLog_Game, " Intrusive   : %9.02f  %9.02f  %9.02f  %9.02f  %9.02f  %9.02f  %9.02f", float(uAddingHeadTimeIntr) / 1000.0f, float(uAddingTailTimeIntr) / 1000.0f, float(uAddingMidTimeIntr) / 1000.0f, float(uTraverseFwndTimeIntr) / 1000.0f, float(uTraverseBackTimeIntr) / 1000.0f, float(uTraverseFwndNoItTimeIntr) / 1000.0f, float(uAddingMidTimeNoIt) / 1000.0f );
	zenIO::Log(zenConst::keLog_Game, " Child class : %9.02f  %9.02f  %9.02f  %9.02f  %9.02f", float(uAddingHeadTimeChild) / 1000.0f, float(uAddingTailTimeChild) / 1000.0f, float(uAddingMidTimeChild) / 1000.0f, float(uTraverseFwndTimeChild) / 1000.0f, float(uTraverseBackTimeChild) / 1000.0f);
	zenIO::Log(zenConst::keLog_Game, " Std         : %9.02f  %9.02f  %9.02f  %9.02f  %9.02f", float(uAddingHeadTimeStd) / 1000.0f, float(0) / 1000.0f, float(0) / 1000.0f, float(uTraverseFwndTimeStd) / 1000.0f, float(0) / 1000.0f);
	zenIO::Log(zenConst::keLog_Game, "");
	zenDelArray(aItemNode);
	zenDelArray(aItemChild);
}

}