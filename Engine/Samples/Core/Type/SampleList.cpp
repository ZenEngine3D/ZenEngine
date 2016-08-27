#include "zenEngine.h"
#include <list>
//=================================================================================================
//! @example SampleList.cpp
//! Informations on the usage of various list functionalities
//=================================================================================================

namespace sample
{

//==================================================================================================
//! @brief		Test the zListIntrusive container
//!	@details	Test sorted add, removal of items, etc...
//==================================================================================================
class IntrusiveListItem
{
public:
	IntrusiveListItem(){}
	IntrusiveListItem(int _uValue) : mValue(_uValue) {}
	int mValue = 0;
	zListLink mlnkList1;
	zListLink mlnkList2;
	typedef zList<IntrusiveListItem, &IntrusiveListItem::mlnkList1, false> TypeList1;
	typedef zList<IntrusiveListItem, &IntrusiveListItem::mlnkList2, false> TypeList2;
};

class ListItemRefCounted : public zRefCounted
{
public:
	ListItemRefCounted() {}
	
	ListItemRefCounted(int val) : mValue(val) {};

	virtual ~ListItemRefCounted()
	{
		printf("ListItemRefCounted::Destruct");
	}

	int mValue = 0;

protected:
	zListLink mlnkListRef;
	zListLink mlnkList;
public:
	typedef zListRef<zGameRef, ListItemRefCounted, &ListItemRefCounted::mlnkListRef> TypeList2;
	typedef zList<ListItemRefCounted, &ListItemRefCounted::mlnkList, false> TypeList3;
};

class ItemWithVirtual
{
public:
	virtual ~ItemWithVirtual(){};
	size_t mValueBefore;
	zListLink mlnkList;
	size_t mValueAfter;
	typedef zList<ItemWithVirtual, &ItemWithVirtual::mlnkList, true> TypeList;
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

	//============================================================================
	// Making sure compiler supports intrusive list vtable padding
	//============================================================================
	ItemWithVirtual				TestVirtualItem;
	ItemWithVirtual::TypeList	TestVirtualList;
	TestVirtualItem.mValueBefore	= 1;
	TestVirtualItem.mValueAfter		= 2;
	TestVirtualList.PushHead(TestVirtualItem);
	zenAssertMsg(TestVirtualItem.mValueBefore == 1, "zList doesn't support this compiler properly, adjust the TVirtualPad handling");
	zenAssertMsg(TestVirtualItem.mValueAfter == 2, "zList doesn't support this compiler properly, adjust the TVirtualPad handling");

	//============================================================================
	// Ref test Intrusive node
	//============================================================================
	ListItemRefCounted::TypeList2 listRef2Item;
	ListItemRefCounted::TypeList3 list3Item;
	ListItemRefCounted* pItem = new ListItemRefCounted;
	{
		zGameRef<ListItemRefCounted> rItemRef = pItem;
		listRef2Item.PushHead(rItemRef);
	}
	listRef2Item.PopHead();
	/*
	int valueInt								= 0;
	int* pValueInt								= &valueInt;
	zGameRef<TestReferenceValue> rValueSmart	= new TestReferenceValue;
	decltype(rValueSmart) rValue2				= rValueSmart;
	decltype(*rValueSmart) Value3				= *rValueSmart;
	decltype(*pValueInt) Value4					= *pValueInt;
	decltype((*pValueInt)) Value5				= *pValueInt;
	*/
	//============================================================================
	// Intrusive list usage
	//============================================================================
	const zUInt						kuTestCount = 1024 * 512;
	IntrusiveListItem*				aItemNode = zenNewDefault IntrusiveListItem[kuTestCount];
	IntrusiveListItem::TypeList1	lstIntrusive;

	for (int i = 0; i<kuTestCount; ++i)
		aItemNode[i].mValue = i;

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
		lstIntrusive.GetHeadIt().AddAfter(aItemNode[i]);
	uAddingMidTimeIntr = zenSys::GetTimeUSec() - uAddingMidTimeIntr;
	
	lstIntrusive.Empty();
	zU64 uAddingMidTimeNoIt = zenSys::GetTimeUSec();
	lstIntrusive.PushHead(aItemNode[0]);
	for (int i = 1; i<kuTestCount; ++i)
		lstIntrusive.AddAfter(aItemNode[0], aItemNode[i]);
	uAddingMidTimeNoIt = zenSys::GetTimeUSec() - uAddingMidTimeNoIt;

	zU64 uTraverseFwndTimeIntr = zenSys::GetTimeUSec();
	auto intrIt = lstIntrusive.GetHeadIt();
	int val = 0;
	int i = 0;
	while( intrIt )
	{
		val += intrIt->mValue;
		++intrIt;		
	}
	uTraverseFwndTimeIntr = zenSys::GetTimeUSec() - uTraverseFwndTimeIntr;
	
	zU64 uTraverseBackTimeIntr = zenSys::GetTimeUSec();
	intrIt = lstIntrusive.GetTailIt();
	while( intrIt )
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
		pCurrentItem = lstIntrusive.GetNext(*pCurrentItem);
	}
	uTraverseFwndNoItTimeIntr = zenSys::GetTimeUSec() - uTraverseFwndNoItTimeIntr;

	//============================================================================
	// Regular std list
	//============================================================================
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

	zenIO::Log(zenConst::keLog_Game, "%s", zenConst::kzLineA70);
	zenIO::Log(zenConst::keLog_Game, " List Performances (%i elements)", kuTestCount, val);
	zenIO::Log(zenConst::keLog_Game, "%s", zenConst::kzLineA70);
	zenIO::Log(zenConst::keLog_Game, "               Add Head  | Add Tail  | Add Mid  | Seek Next | Seek Prev | Seek NoIt | Add NoIt");
	zenIO::Log(zenConst::keLog_Game, "%s", zenConst::kzLineB70);
	zenIO::Log(zenConst::keLog_Game, " Intrusive   : %9.02f  %9.02f  %9.02f  %9.02f  %9.02f  %9.02f  %9.02f", float(uAddingHeadTimeIntr) / 1000.0f, float(uAddingTailTimeIntr) / 1000.0f, float(uAddingMidTimeIntr) / 1000.0f, float(uTraverseFwndTimeIntr) / 1000.0f, float(uTraverseBackTimeIntr) / 1000.0f, float(uTraverseFwndNoItTimeIntr) / 1000.0f, float(uAddingMidTimeNoIt) / 1000.0f );
	zenIO::Log(zenConst::keLog_Game, " Std         : %9.02f  %9.02f  %9.02f  %9.02f  %9.02f", float(uAddingHeadTimeStd) / 1000.0f, float(0) / 1000.0f, float(0) / 1000.0f, float(uTraverseFwndTimeStd) / 1000.0f, float(0) / 1000.0f);
	zenIO::Log(zenConst::keLog_Game, "");
	zenDelArray(aItemNode);
//	zenDelArray(aItemChild);
}

}