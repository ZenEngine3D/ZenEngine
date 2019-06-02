#include "zenEngine.h"
#include <list>
//=================================================================================================
//! @example SampleList.cpp
//! Informations on the usage of various list functionalities
//=================================================================================================

namespace sample
{

class ListItemObjTest: public zListItem<3>
{
public:
	using ListA = zList<ListItemObjTest,0>;
	using ListB = zList<ListItemObjTest,1>;
	using ListC = zList<ListItemObjTest,2>;
	virtual ~ListItemObjTest()
	{
	}

	__declspec(noinline) void Test()
	{
		++value;
		printf("Value %i", value);
	}
	int value;
};

//=================================================================================================
// Testing auto removal of item from list when destroyed 
// and also testing class staying POD if auto-removal not supported
//=================================================================================================
struct ListItemTriv: zListItemTrivial<>
{
	ListItemTriv() = default;
	void Init(zU32 inValue) { mLinks[0].Reset(); mValue = inValue; }
	zU32 mValue;
};
using TypeListTriv = zList<ListItemTriv>;

struct ListItemPOD
{
	ListItemPOD() = default;
	void Init(zU32 inValue) { mLinks[0].Reset(); mValue = inValue; }
	zU32 mValue;
	zListLinkMember(1);	
};
using TypeListPOD = zList<ListItemPOD>;

struct ListItemAutoRem: public zListItem<> { ListItemAutoRem() = default;  zU32 mValue = 0; };
using TypeListAutoRem = zList<ListItemAutoRem>;

void SampleListIntrusive_POD()
{	
	//------------------------------------------------------------------------------------------------
	// Preserving 'trivialness' state of class
	//------------------------------------------------------------------------------------------------
	{
		zenAssert( std::is_trivial<ListItemTriv>() );	// Confirm that object is still a 'trivial' type (can be memcopied)		
		ListItemTriv Items[3];
		for( zU32 i(0); i<zenArrayCount(Items); i++)
			Items[i].Init(i);

		{			
			TypeListTriv ListTrivial;
			for( auto& Item : Items)
				ListTrivial.push_back(Item);

			Items[0].ListItemTriv::~ListItemTriv(); //Explicitly call destructor, making sure object *isn't* automatically removed from list
			zenAssert(ListTrivial.front().mValue == 0);
		}
		zenAssert( !TypeListTriv::IsInList(Items[0]) );	// Should have been removed because List has been destroyed
	}

	//------------------------------------------------------------------------------------------------
	// Preserving 'POD' state of class (Trivial + Standard layout)
	//------------------------------------------------------------------------------------------------
	{
		zenAssert(std::is_pod<ListItemPOD>());	// Confirm that object is still a 'trivial' type (can be memcopied)		
		ListItemPOD Items[3];
		for (zU32 i(0); i<zenArrayCount(Items); i++)
			Items[i].Init(i);

		{
			TypeListPOD ListPod;
			for (auto& Item : Items)
				ListPod.push_back(Item);

			Items[0].ListItemPOD::~ListItemPOD(); //Explicitly call destructor, making sure object *isn't* automatically removed from list
			zenAssert(ListPod.front().mValue == 0);
		}
		zenAssert(!TypeListPOD::IsInList(Items[0]));	// Should have been removed because List has been destroyed
	}

	//------------------------------------------------------------------------------------------------
	// Auto-removal support when item is destroyed
	//------------------------------------------------------------------------------------------------
	{		
		ListItemAutoRem Items[3];
		for (zU32 i(0); i<zenArrayCount(Items); i++)
			Items[i].mValue = i;

		{			
			TypeListAutoRem ListAutoRem;			
			zenAssert( std::is_trivial<TypeListAutoRem>::value == false);	// Confirm that object is not a POD anymore
							
			for (auto& Item : Items)
				ListAutoRem.push_back(Item);

			Items[0].ListItemAutoRem::~ListItemAutoRem(); //Explicitly call destructor, making sure object *is* automatically removed from list
			zenAssert(ListAutoRem.front().mValue != 0);			
		}
		zenAssert( !TypeListAutoRem::IsInList(Items[1]) );	// Should have been removed because List has been destroyed
	}
}

class ListItemRefCounted: public zRefCounted, public zListItem<1,1>
{
public:
	using List = zListRef<ListItemRefCounted, 0, zListItem<1,1> >;

	ListItemRefCounted(zU32 inVal):mValue(inVal){}
	virtual ~ListItemRefCounted()
	{
		printf("ListItemRefCounted::Destruct");
	}

protected:
	zU32 mValue;
};

void SampleListIntrusive_Ref()
{
	ListItemRefCounted::List ListItems;
	for(int i(0); i<3; ++i)
		ListItems.push_back( *(zenNew ListItemRefCounted(i)) );

	while( ListItems.empty() == false )
		ListItems.pop_back();
}

struct ParentListItem : public zListItem<1>
{
	zU32 mParentVal = 0;
	using List = zList<ParentListItem, 0>;
};

//! @Note	Parent class also inherit from zListItem, use index 1
//!			in zListItem<1,**1**> to prevent confusion.
//!			First 1 is just the number of list this item can be in
//!			(not counting the parent list support)
struct ChildListItem : public ParentListItem, public zListItem<1,1>
{
	zU32 mChildValue = 1;
	using List = zList<ChildListItem, 0, zListItem<1,1> >;
};

//! @Note	Parents classes also inherit from zListItem, use index 2
//!			in zListItem<1, **2**> to prevent confusion
//!			First 1 is just the number of list this item can be in
//!			(not counting the parenta list support)
struct ChildChildListItem: public ChildListItem,public zListItem<1,2>
{
	zU32 mChildChildValue = 2;
	using List = zList<ChildChildListItem,0,zListItem<1,2> >;
};

//==================================================================================================
//! @brief	Demonstrate how to fix issue when both parent and child class are zListItem
//! @detail When multiple inherited parent class are of type zListItem, this can cause
//!			issue, where compiler doesn't know which item to use to store list links. To get 
//!			around this, we can specify a zListIndex in declaration, telling us specifically
//!			which parent class to use.
//! @note	Not to be confused with zListItem that can be declared with multiple list support
//==================================================================================================
void SampleListMultiInheritance()
{
	ParentListItem::List		ListParent;
	ChildListItem::List			ListChild;
	ChildChildListItem::List	ListChildChild;
	ParentListItem				ObjectSimple;
	ChildListItem				ObjectMultiListItem;
	ChildChildListItem			ObjectMultiListItem2x;

	ListParent.push_front(ObjectSimple);
	ListParent.push_front(ObjectMultiListItem);
	ListParent.push_front(ObjectMultiListItem2x);
	ListChild.push_front(ObjectMultiListItem);
	ListChild.push_front(ObjectMultiListItem2x);
	ListChildChild.push_front(ObjectMultiListItem2x);
}

struct StructWithSort: public zListItem<>
{
	StructWithSort(zU32 _SortId): mSortId(_SortId) {};
	bool operator<(const StructWithSort& _Cmp) { return mSortId < _Cmp.mSortId; }
	zU32 mSortId;
	using List = zList<StructWithSort>;
};

void SampleListSort()
{
	StructWithSort Objects[] = {5, 4, 6, 5, 0, 3};
	StructWithSort::List ListObjs;
	for(auto& Item : Objects)
		ListObjs.push_sort(Item);	
}

//==================================================================================================
//! @brief		Test the zListIntrusive container
//!	@details	Test sorted add, removal of items, etc...
//==================================================================================================
void SampleListIntrusive()
{
	zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);
	zenIO::Log(zenConst::keLog_Game, " zListIntrusive");
	zenIO::Log(zenConst::keLog_Game, zenConst::kzLineA40);

	{
		ListItemObjTest Items[10];
		ListItemObjTest::ListA ListA;
		ListItemObjTest::ListB ListB;
		ListItemObjTest::ListC ListC;
		for(int i=0; i<zenArrayCount(Items); ++i)
		{
			Items[i].value = i;
			ListA.push_front(Items[i]);
			ListB.push_back(Items[i]);
			ListC.push_front(Items[i]);
		}
	
		int SumA(0);
		auto ItA(ListA.begin());		
		while( ItA )
		{
			SumA += (*ItA).value;
			++ItA;
		}

		int SumB(0);
		auto ItB(ListB.cbegin());		
		while( ItB )
		{
			SumB += ItB->value;
			++ItB;
		}

		int SumD(0);
		for( auto& Item : ListA )
			SumD += Item.value;	
	}

	SampleListIntrusive_POD();
	SampleListIntrusive_Ref();
	SampleListMultiInheritance();
	SampleListSort();

#if 0
	const zUInt						kuTestCount = 1024 * 512;
	IntrusiveListItem*				aItemNode = zenNew IntrusiveListItem[kuTestCount];
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
#endif
}


}