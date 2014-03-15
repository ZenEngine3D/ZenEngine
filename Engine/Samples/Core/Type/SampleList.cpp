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
	CMgr::Log.Log(CLog::keLog_Game, zenConst::kzLineA40);
	CMgr::Log.Log(CLog::keLog_Game, " zList1x (single linked list) Sorting");
	CMgr::Log.Log(CLog::keLog_Game, zenConst::kzLineA40);

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
	CMgr::Log.Log(CLog::keLog_Game, "Added   :");
	for(int i=0; i<kuItemCount; ++i)
	{
		zU32 uIndex = uInsertOrder[i];
		oItems[uIndex].miVal = uIndex;
		moList.AddSort<int>(&oItems[uIndex], &oItems[uIndex].miVal);
		CMgr::Log.Printf(CLog::keLog_Game, " %02i", uIndex);
	}

	//-----------------------------------------------------
	// Output the result, making sure it is sorted
	//-----------------------------------------------------
	zList1xNode* pItem = moList.GetHead();	
	CMgr::Log.Log(CLog::keLog_Game, "Sorted  :");
	while( pItem != moList.GetInvalid() )
	{
		CMgr::Log.Printf(CLog::keLog_Game, " %02i", static_cast<TestList1x*>(pItem)->miVal);
		pItem = pItem->LstNext();
	}
	
	//-----------------------------------------------------
	// Remove each item
	//-----------------------------------------------------
	for(int i=0; i<kuItemCount; ++i)
	{
		zU32 uIndex = uRemoveOrder[i];
		moList.Remove( &oItems[ uIndex ] );
		CMgr::Log.Log(CLog::keLog_Game, "Removed : (%02i) Left:", uIndex);
		zList1xNode* pItem = moList.GetHead();	
		while( pItem != moList.GetInvalid() )
		{
			CMgr::Log.Printf(CLog::keLog_Game, " %02i", static_cast<TestList1x*>(pItem)->miVal);
			pItem = pItem->LstNext();
		}		
	}

	CMgr::Log.Printf(CLog::keLog_Game, "\n");

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
	CMgr::Log.Log(CLog::keLog_Game, zenConst::kzLineA40);
	CMgr::Log.Log(CLog::keLog_Game, " zList2x (double linked list) Sorting");
	CMgr::Log.Log(CLog::keLog_Game, zenConst::kzLineA40);

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
	CMgr::Log.Log(CLog::keLog_Game, "Added   :");
	for(int i=0; i<kuItemCount; ++i)
	{
		oItems[i].miVal = uInsertOrder[i];
		moList.AddSort<int>(&oItems[i], &oItems[i].miVal);
		CMgr::Log.Printf(CLog::keLog_Game, " %02i", uInsertOrder[i]);
	}

	//-----------------------------------------------------
	// Output the result, making sure it is sorted
	//-----------------------------------------------------
	zList2xNode* pItem = moList.GetHead();
	CMgr::Log.Log(CLog::keLog_Game, "Sorted  :");
	while( pItem != moList.GetInvalid() )
	{
		CMgr::Log.Printf(CLog::keLog_Game, " %02i", static_cast<TestList2x*>(pItem)->miVal);
		pItem = pItem->LstNext();
	}	

	//-----------------------------------------------------
	// Remove each item
	//-----------------------------------------------------
	for(int i=0; i<kuItemCount; ++i)
	{
		zU32 uIndex = uRemoveOrder[i];
		oItems[ uIndex ].LstRemove();
		CMgr::Log.Log(CLog::keLog_Game, "Removed : (%02i) Left:", uIndex);
		zList2xNode* pItem = moList.GetHead();	
		while( pItem != moList.GetInvalid() )
		{
			CMgr::Log.Printf(CLog::keLog_Game, " %02i", static_cast<TestList2x*>(pItem)->miVal);
			pItem = pItem->LstNext();
		}		
	}
	CMgr::Log.Printf(CLog::keLog_Game, "\n");

	
}

}