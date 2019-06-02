#pragma once

namespace zen { namespace zenType 
{
//=================================================================================================
//! @brief	Macro used to add a ListLink to a class.
//! @note	Unless wanting to preserve POD of a class, does not need to be used directly creating
//!			a class with intrusive list support.
//=================================================================================================
#define zListLinkMember(ListCount)	zListLink	mLinks[ListCount];

//=============================================================================================
//! @class	zListLink
//! @brief	Contain infos to reach previous and next item in a list.
//! @detail The templated list object is able to find the object the link belongs too, 
//!			by knowing the offset between base object address and link member address.
//! @note	Bit 0 turned on prev/next pointer when this link object belongs to list root node.
//=============================================================================================
class zListLink
{
public:	
							zListLink() = default;
	zenInline bool			IsInList()const;
	zenInline bool			IsItem()const;
	zenInline void			Reset();
	zenInline zListLink*	Next()const;
	zenInline zListLink*	Prev()const;
	
protected:
	zenInline void			SetAsRoot();
	zenInline void			SetNext(zListLink* inItem);
	zenInline void			SetPrev(zListLink* inItem);
	zenInline void			Remove();
	zenInline void			InsertBefore(zListLink& zenRestrict _NewLink);
	zenInline void			InsertAfter(zListLink& zenRestrict _NewLink);
	zListLink*				mpPrev;		//!< Pointer to next link
	zListLink*				mpNext;		//!< Pointer to previous link
	friend class zListBase;
	template<class, unsigned char, class> friend class zList;
	template<unsigned char, unsigned char> friend class zListItem;
};

//=================================================================================================
//! @brief		Used to add IntrusiveList support in a class. 
//! @detail		This class preserve the 'trivialness' of a class, meaning that objects using it can 
//!				still be memcopied without issue.
//!	@example	Creating a class that can live in 2 different list :
//!				class MyClass : public zListItemTrivial<2> {}
//!				zList<MyClass, 0> listA;
//!				zList<MyClass, 1> listB;
//! @note		In the case of a parent class and child class both wanting to be in different list
//!				we can use TMultiIndex with a different value to avoid compiler confusion when 
//!				knowing which mLink member to use. Look at SampleListMultiInheritance() example.
//=================================================================================================
template <unsigned char TListCount=1, unsigned char TMultiIndex=0>
class zListItemTrivial
{
public: 
	zListItemTrivial() = default;
protected:
	zListLinkMember(TListCount);
	template<class,unsigned char,class> friend class zList;
};

//=================================================================================================
//! @brief	Add IntrusiveList support in a child class through inheritance.
//! @detail	Unlike zListItemTrivial, objects using this class are not trivial anymore, because
//!			it use a constructor to initialize data to null, and destructor to auto remove items
//!			from list it belongs to, when destroyed. 
//! @note	Should be the main class used for adding list support to a class.
//=================================================================================================
template <unsigned char TListCount=1, unsigned char TMultiIndex=0>
class zListItem : public zListItemTrivial<TListCount, TMultiIndex>
{
public:
	zenInline zListItem();
	zenInline ~zListItem();
};

//=================================================================================================
//! @class	zListBase
//! @brief	Double 'intrusive' linked list base class
//! @detail	Intrusive list do not require using additional allocation, their list infos is 
//!			directly included in the object, making it more cache friendly and less heap intensive.
//!			Example : look at SampleListIntrusive() for more infos
//=================================================================================================
class zListBase
{
public:
	zenInline				zListBase();
	zenInline virtual		~zListBase();

	// Using STL standard naming convention (http://www.cplusplus.com/reference/stl/)
	zenInline bool			empty()const;
	zenInline virtual void	clear();

protected:
	class Iterator
	{
	public:
		zenInline			Iterator(zListLink* _pCurrent);
		zenInline			Iterator(const Iterator& _Copy);
		zenInline bool		IsValid()const;
		zenInline bool		operator==(const Iterator& _Cmp)const;
		zenInline bool		operator!=(const Iterator& _Cmp)const;
		zenInline explicit	operator bool()const;
	protected:
		zListLink*			mpCurrent = nullptr;
	};

	class IteratorConst
	{
	public:
		zenInline			IteratorConst(const zListLink* _pCurrent);
		zenInline			IteratorConst(const IteratorConst& _Copy);
		zenInline bool		IsValid()const;		
		zenInline bool		operator==(const IteratorConst& _Cmp)const;	
		zenInline bool		operator!=(const IteratorConst& _Cmp)const;
		zenInline explicit	operator bool()const;
	protected:
		const zListLink*	mpCurrent = nullptr;
	};

	zListLink mRoot;
};

//=================================================================================================
//! @class	zList
//! @brief	Double 'intrusive' linked list
//! @detail	Made class compatible with STL containers, so std coding technique can be used with it
//!	@param TItem -		Class of object stored in the list
//! @param TListIdx -	Object support being stored in multiple list at the same time, 
//!						specify which ListIndex this list should use. (Only needed when not 0)
//! @param TLinkAccess -Through consecutive inheritance, it's possible for class to inherit from  
//!						multiple zListItem, prevent problem by making which one to use explicit.
//!						(Needed when ambiguous compile error from multiple zListItem child class)
//!						Example here : SampleListMultiInheritance
//=================================================================================================
template<class TItem, unsigned char TListIdx=0, class TLinkAccess=TItem>
class zList : public zListBase
{
public:
	class Iterator : public zListBase::Iterator
	{
	public:				
									Iterator(){}
									Iterator(const Iterator& _Copy);
		TItem*						Get()const;
		TItem&						operator*()const { return *Get(); }
		TItem*						operator->()const {return Get(); }
		zenInline Iterator&			operator=(const Iterator& _Copy);
		zenInline Iterator&			operator++();
		zenInline Iterator&			operator--();
	protected:
									Iterator(zListLink* _pCurrent);
		friend class zList;
	};
	class IteratorConst: public zListBase::IteratorConst
	{
	public:
									IteratorConst() {}
									IteratorConst(const IteratorConst& _Copy);
		const TItem*				Get()const;
		const TItem&				operator*()const { return *Get(); }
		const TItem*				operator->()const { return Get(); }
		zenInline IteratorConst&	operator=(const IteratorConst& _Copy);
		zenInline IteratorConst&	operator++();
		zenInline IteratorConst&	operator--();
	protected:
									IteratorConst(const zListLink* _pCurrent);
		friend class zList;
	};

	//Using STL container naming convention	
	zenInline zList&		push_front(TItem& _ItemAdd);
	zenInline zList&		push_back(TItem& _ItemAdd);	
	zenInline zList&		insert(Iterator& _Pos, TItem& _ItemAdd);
	zenInline zList&		insert(TItem& _ItemPos, TItem& _ItemAdd);
	zenInline TItem&		front()const;
	zenInline TItem&		back()const;
	zenInline TItem*		pop_front();
	zenInline TItem*		pop_back();
	zenInline static void	remove(TItem& _ItemRem);

	zenInline Iterator		begin();
	zenInline Iterator		end();
	zenInline IteratorConst	cbegin()const;
	zenInline IteratorConst	cend()const;
	
	//Non standard STL
	zenInline zList&		insert_after(Iterator& _Pos,TItem& _ItemAdd);
	zenInline zList&		insert_after(TItem& _ItemPos,TItem& _ItemAdd);
	zenInline zList&		insert_before(Iterator& _Pos, TItem& _ItemAdd);
	zenInline zList&		insert_before(TItem& _ItemPos, TItem& _ItemAdd);
	zenInline zList&		push_sort(TItem& _ItemAdd);	
	zenInline TItem*		front_check()const;
	zenInline TItem*		back_check()const;

	zenInline static bool	IsInList(const TItem& Item);
	zenInline static TItem*	GetNext(const TItem& Item);
	zenInline static TItem*	GetPrev(const TItem& Item);

protected:
	static TItem*			GetItemFromLink(const zListLink& _Link);
};

//=================================================================================================
//! @class	zListRefTest
//! @brief	Double 'intrusive' linked list with Reference Count awareness
//! @detail	Similar to zList, but will increment refcount when item added in list, 
//!			and decrement when removed
//! @note	Only need to override methods than can add or remove an item
//=================================================================================================
template<class TItem,unsigned char TListIdx=0, class TLinkAccess=TItem>
class zListRef: public zList<TItem, TListIdx, TLinkAccess>
{
zenClassDeclare(zListRef, zList)
public:
	zenInline virtual		~zListRef();
	zenInline virtual void	clear();

	//Standard STL
	zenInline zListRef&		push_front(TItem& inItem);
	zenInline zListRef&		push_back(TItem& inItem);
	zenInline zListRef&		insert(Iterator& _Pos, TItem& _ItemAdd);
	zenInline zListRef&		insert(TItem& _ItemPos, TItem& _ItemAdd);
	zenInline TItem*		pop_front();
	zenInline TItem*		pop_back();
	zenInline static void	remove(TItem& _ItemRem);

	//Non standard STL
	zenInline zListRef&		insert_before(TItem& _Item);

};

} } //namespace zen, Type