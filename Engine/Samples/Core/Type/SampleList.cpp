#include "libZenEngine.h"

//=================================================================================================
//! @example SampleList.cpp
//! Informations on the usage of various list functionalities
//=================================================================================================

namespace sample
{

//==================================================================================================
//! @brief		Test the awList1x container
//!	@details	Test sorted add, removal of items, etc...
//==================================================================================================
class TestList1x : public awList1xNode
{
public:
	int miVal;	
};

//==================================================================================================
//! @brief		Test the awList2x container
//!	@details	Test sorted add, removal of items, etc...
//==================================================================================================
class TestList2x : public awList2xNode
{
public:
	int miVal;	
};

//==================================================================================================
//! @brief		Test the awList1x container
//!	@details	Test sorted add, removal of items, etc...
//==================================================================================================
void SampleList1x()
{	
	CMgr::Log.Log(CLog::keLog_Game, awconst::kzLineA40);
	CMgr::Log.Log(CLog::keLog_Game, " awList1x (single linked list) Sorting");
	CMgr::Log.Log(CLog::keLog_Game, awconst::kzLineA40);

	//-----------------------------------------------------
	// Create 5 items with increasing value
	//-----------------------------------------------------
	#define kuItemCount 6
	TestList1x	oItems[kuItemCount];
	awU32			uInsertOrder[kuItemCount]={3,0,5,1,4,2};
	awU32			uRemoveOrder[kuItemCount]={3,0,5,1,4,2};

	//-----------------------------------------------------
	// Add the item in random order, insert sorted add
	//-----------------------------------------------------
	awList1x moList;
	CMgr::Log.Log(CLog::keLog_Game, "Added   :");
	for(int i=0; i<kuItemCount; ++i)
	{
		awU32 uIndex = uInsertOrder[i];
		oItems[uIndex].miVal = uIndex;
		moList.AddSort<int>(&oItems[uIndex], &oItems[uIndex].miVal);
		CMgr::Log.Printf(CLog::keLog_Game, " %02i", uIndex);
	}

	//-----------------------------------------------------
	// Output the result, making sure it is sorted
	//-----------------------------------------------------
	awList1xNode* pItem = moList.GetHead();	
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
		awU32 uIndex = uRemoveOrder[i];
		moList.Remove( &oItems[ uIndex ] );
		CMgr::Log.Log(CLog::keLog_Game, "Removed : (%02i) Left:", uIndex);
		awList1xNode* pItem = moList.GetHead();	
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
//! @brief		Test the awList2x container
//!	@details	Test sorted add, removal of items, etc...
//==================================================================================================
void SampleList2x()
{	
	CMgr::Log.Log(CLog::keLog_Game, awconst::kzLineA40);
	CMgr::Log.Log(CLog::keLog_Game, " awList2x (double linked list) Sorting");
	CMgr::Log.Log(CLog::keLog_Game, awconst::kzLineA40);

	//-----------------------------------------------------
	// Create 5 items with increasing value
	//-----------------------------------------------------
	#define kuItemCount 6
	TestList2x	oItems[kuItemCount];
	awU32			uInsertOrder[kuItemCount]={3,0,5,1,4,2};
	awU32			uRemoveOrder[kuItemCount]={3,0,5,1,4,2};

	//-----------------------------------------------------
	// Add the item in random order, insert sorted add
	//-----------------------------------------------------
	awList2x moList;
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
	awList2xNode* pItem = moList.GetHead();
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
		awU32 uIndex = uRemoveOrder[i];
		oItems[ uIndex ].LstRemove();
		CMgr::Log.Log(CLog::keLog_Game, "Removed : (%02i) Left:", uIndex);
		awList2xNode* pItem = moList.GetHead();	
		while( pItem != moList.GetInvalid() )
		{
			CMgr::Log.Printf(CLog::keLog_Game, " %02i", static_cast<TestList2x*>(pItem)->miVal);
			pItem = pItem->LstNext();
		}		
	}
	CMgr::Log.Printf(CLog::keLog_Game, "\n");

	
}

}