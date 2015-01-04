#include "libZenEngine.h"

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
	zbMgr::Log.Log(zbLog::keLog_Game, zenConst::kzLineA40);
	zbMgr::Log.Log(zbLog::keLog_Game, " zList1x (single linked list) Sorting");
	zbMgr::Log.Log(zbLog::keLog_Game, zenConst::kzLineA40);

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
	zbMgr::Log.Log(zbLog::keLog_Game, "Added   :");
	for(int i=0; i<kuItemCount; ++i)
	{
		zU32 uIndex = uInsertOrder[i];
		oItems[uIndex].miVal = uIndex;
		moList.AddSort<int>(&oItems[uIndex], &oItems[uIndex].miVal);
		zbMgr::Log.Printf(zbLog::keLog_Game, " %02i", uIndex);
	}

	//-----------------------------------------------------
	// Output the result, making sure it is sorted
	//-----------------------------------------------------
	zList1xNode* pItem = moList.GetHead();	
	zbMgr::Log.Log(zbLog::keLog_Game, "Sorted  :");
	while( pItem != moList.GetInvalid() )
	{
		zbMgr::Log.Printf(zbLog::keLog_Game, " %02i", static_cast<TestList1x*>(pItem)->miVal);
		pItem = pItem->LstNext();
	}
	
	//-----------------------------------------------------
	// Remove each item
	//-----------------------------------------------------
	for(int i=0; i<kuItemCount; ++i)
	{
		zU32 uIndex = uRemoveOrder[i];
		moList.Remove( &oItems[ uIndex ] );
		zbMgr::Log.Log(zbLog::keLog_Game, "Removed : (%02i) Left:", uIndex);
		zList1xNode* pItem = moList.GetHead();	
		while( pItem != moList.GetInvalid() )
		{
			zbMgr::Log.Printf(zbLog::keLog_Game, " %02i", static_cast<TestList1x*>(pItem)->miVal);
			pItem = pItem->LstNext();
		}		
	}

	zbMgr::Log.Printf(zbLog::keLog_Game, "\n");

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
	zbMgr::Log.Log(zbLog::keLog_Game, zenConst::kzLineA40);
	zbMgr::Log.Log(zbLog::keLog_Game, " zList2x (double linked list) Sorting");
	zbMgr::Log.Log(zbLog::keLog_Game, zenConst::kzLineA40);

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
	zbMgr::Log.Log(zbLog::keLog_Game, "Added   :");
	for(int i=0; i<kuItemCount; ++i)
	{
		oItems[i].miVal = uInsertOrder[i];
		moList.AddSort<int>(&oItems[i], &oItems[i].miVal);
		zbMgr::Log.Printf(zbLog::keLog_Game, " %02i", uInsertOrder[i]);
	}

	//-----------------------------------------------------
	// Output the result, making sure it is sorted
	//-----------------------------------------------------
	zList2xNode* pItem = moList.GetHead();
	zbMgr::Log.Log(zbLog::keLog_Game, "Sorted  :");
	while( pItem != moList.GetInvalid() )
	{
		zbMgr::Log.Printf(zbLog::keLog_Game, " %02i", static_cast<TestList2x*>(pItem)->miVal);
		pItem = pItem->LstNext();
	}	

	//-----------------------------------------------------
	// Remove each item
	//-----------------------------------------------------
	for(int i=0; i<kuItemCount; ++i)
	{
		zU32 uIndex = uRemoveOrder[i];
		oItems[ uIndex ].LstRemove();
		zbMgr::Log.Log(zbLog::keLog_Game, "Removed : (%02i) Left:", uIndex);
		zList2xNode* pItem = moList.GetHead();	
		while( pItem != moList.GetInvalid() )
		{
			zbMgr::Log.Printf(zbLog::keLog_Game, " %02i", static_cast<TestList2x*>(pItem)->miVal);
			pItem = pItem->LstNext();
		}		
	}
	zbMgr::Log.Printf(zbLog::keLog_Game, "\n");

	
}

//==================================================================================================
//! @brief		Test the zListIntrusive container
//!	@details	Test sorted add, removal of items, etc...
//==================================================================================================

class IntrusiveListItem
{
public:
	IntrusiveListItem(int val):mValue(val){};
	zList<IntrusiveListItem>::Link mlnkList1;
	zList<IntrusiveListItem>::Link mlnkList2;
	int mValue;
};



void SampleListIntrusive()
{
	zbMgr::Log.Log(zbLog::keLog_Game, zenConst::kzLineA40);
	zbMgr::Log.Log(zbLog::keLog_Game, " zListIntrusive");
	zbMgr::Log.Log(zbLog::keLog_Game, zenConst::kzLineA40);

	//-----------------------------------------------------
	// Create 5 items with increasing value
	//-----------------------------------------------------
#define kuItemCount 6
	TestList2x	oItems[kuItemCount];
	zU32		uInsertOrder[kuItemCount] = { 3, 0, 5, 1, 4, 2 };
	zU32		uRemoveOrder[kuItemCount] = { 3, 0, 5, 1, 4, 2 };
	
	//zList<TestIntrusiveItem, offsetof(TestIntrusiveItem, mlnkList1) > lstOne;
	zListDeclare(IntrusiveListItem, mlnkList1) lstOne;
	IntrusiveListItem item1(1);
	IntrusiveListItem item2(2);
	IntrusiveListItem item3(3);
	IntrusiveListItem item4(4);

	//---	
	lstOne.UnlinkAll();
	lstOne.PushHead(item1); //Making sure re-inserting works
	lstOne.PushHead(item4);
	lstOne.PushHead(item3);
	lstOne.PushHead(item2);
	lstOne.PushHead(item1);
	IntrusiveListItem* pItem = lstOne.GetHead();
	zbMgr::Log.Log(zbLog::keLog_Game, "Add Head       : ");
	while (pItem)
	{
		printf("%02i ", pItem->mValue);
		pItem = pItem->mlnkList1.GetNext();
	}
	//---
	zbMgr::Log.Log(zbLog::keLog_Game, "Add Tail       : ");
	lstOne.UnlinkAll();
	lstOne.PushTail(item1);
	lstOne.PushTail(item2);
	lstOne.PushTail(item3);
	lstOne.PushTail(item4);
	pItem = lstOne.GetHead();
	while( pItem )
	{
		printf("%02i ", pItem->mValue );
		pItem = pItem->mlnkList1.GetNext();
	}
	//---
	zbMgr::Log.Log(zbLog::keLog_Game, "Inserting      : ");
	lstOne.UnlinkAll();
	lstOne.PushTail(item1);
	item1.mlnkList1.InsertAfter(item2);
	item2.mlnkList1.InsertAfter(item3);
	item3.mlnkList1.InsertAfter(item4);
	pItem = lstOne.GetHead();
	while( pItem )
	{
		printf("%02i ", pItem->mValue );
		pItem = pItem->mlnkList1.GetNext();
	}
	//---
	zbMgr::Log.Log(zbLog::keLog_Game, "Iterator back  : ");
	zList<IntrusiveListItem>::Iterator it = lstOne.GetTailIt();
	while( *it )
	{
		printf("%02i ", (*it)->mValue);
		--it;
	}

	zbMgr::Log.Log(zbLog::keLog_Game, " ");
#if 0
	//-----------------------------------------------------
	// Add the item in random order, insert sorted add
	//-----------------------------------------------------
	zList2x moList;
	zbMgr::Log.Log(zbLog::keLog_Game, "Added   :");
	for (int i = 0; i < kuItemCount; ++i)
	{
		oItems[i].miVal = uInsertOrder[i];
		moList.AddSort<int>(&oItems[i], &oItems[i].miVal);
		zbMgr::Log.Printf(zbLog::keLog_Game, " %02i", uInsertOrder[i]);
	}

	//-----------------------------------------------------
	// Output the result, making sure it is sorted
	//-----------------------------------------------------
	zList2xNode* pItem = moList.GetHead();
	zbMgr::Log.Log(zbLog::keLog_Game, "Sorted  :");
	while (pItem != moList.GetInvalid())
	{
		zbMgr::Log.Printf(zbLog::keLog_Game, " %02i", static_cast<TestList2x*>(pItem)->miVal);
		pItem = pItem->LstNext();
	}

	//-----------------------------------------------------
	// Remove each item
	//-----------------------------------------------------
	for (int i = 0; i < kuItemCount; ++i)
	{
		zU32 uIndex = uRemoveOrder[i];
		oItems[uIndex].LstRemove();
		zbMgr::Log.Log(zbLog::keLog_Game, "Removed : (%02i) Left:", uIndex);
		zList2xNode* pItem = moList.GetHead();
		while (pItem != moList.GetInvalid())
		{
			zbMgr::Log.Printf(zbLog::keLog_Game, " %02i", static_cast<TestList2x*>(pItem)->miVal);
			pItem = pItem->LstNext();
		}
	}
	zbMgr::Log.Printf(zbLog::keLog_Game, "\n");
#endif

}

}