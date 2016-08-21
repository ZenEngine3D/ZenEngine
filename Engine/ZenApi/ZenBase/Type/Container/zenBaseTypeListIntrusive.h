#pragma once
#ifndef __zenApi_Base_Type_ListIntrusive_h__
#define __zenApi_Base_Type_ListIntrusive_h__

namespace zen { namespace zenType 
{

//=============================================================================================
//! @class	zListLink
//! @brief	Contain infos to reach previous and next item in a list
//! @detail Add one zListLink member per list a class can be a member of. This allow
//!			having intrusive list support (much faster and safer than external container list)
//!			that support multiple lists instead of being limited to one through inheritance.
//=============================================================================================
class zListLink
{
public:
    ZENInline				~zListLink();		
	ZENInline void			SetNull();							//!< Use to set pointer to null. Doing so will break the list if element was in one
	ZENInline bool			IsInList()const;					//!< @brief True if this link has been added to a list

protected:
	ZENInline void			Remove();							//!< @brief Remove item from the list this zListLink belongs to	
	ZENInline zListLink*	GetNext()const;						//!< @brief Get the next item zListLink object
	ZENInline zListLink*	GetPrev()const;						//!< @brief Get the previous item zListLink object	
	zListLink*				mpPrevLink = nullptr;				//!< Pointer to next link (lower bit set to 1 if pointing to list root, invalid)
	zListLink*				mpNextLink = nullptr;				//!< Pointer to previous link (lower bit set to 1 if pointing to list root, invalid)
	template<class T, zListLink T::*, bool> friend class zList;
};

//=================================================================================================
//! @class	zList
//! @brief	Double 'intrusive' linked list
//! @detail	Intrusive list do not require using additional allocation, their list infos is 
//!			directly included in the object, making it more cache friendly and less heap intensive.
//!			Example : look at SampleListIntrusive() for more infos
//!			Exemple: zList<ExempleClass, &ExempleClass::myLink> myList;
//! 
//!			TItem			: The item class
//!			TLinkOffset		: The class member pointer (used to find offset to data in object)
//!			TVirtual1stTime : Needed to tell compiler there's a virtual table at object beginning
//!
//! @note	IMPORTANT: When declaring a zList<myClass, &myClass::listLink> directly in myClass, 
//!			and myClass has a virtual method for the first time, set TVirtualPad to true. 
//!			Without this, compiler give us the wrong offset to member listLink.
//!			(Doesn't realize class has a vtable before data, until class declaration is finished)
//=================================================================================================
template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
class zList
{
public:
	//---------------------------------------------------------------------------------------------
	//! @class	Iterator
	//! @brief	Allow easy traversal of a list
	//---------------------------------------------------------------------------------------------
	class Iterator
	{
	public:
							Iterator();
							Iterator(const Iterator& _Copy);
		bool				IsValid()const;						//!< @brief Return true if pointing to valid data
		TItem*				Get()const;							//!< @brief Get current item (nullptr if none)
		TItem&				operator*()const;					//!< @brief Get current item
		TItem*				operator->()const;					//!< @brief Get current item (nullptr if none)
		explicit 			operator bool()const;				//!< @brief Return true if iterator is still valid
		void				operator++();						//!< @brief Go to next item
		void				operator--();						//!< @brief Go to previous item
		const Iterator&		operator=(const Iterator& _Copy);	//!< @brief Assign iterator current location
		void				AddBefore(TItem& _NewItem);			//!< @brief Insert a new item before this zListLink
		void				AddAfter(TItem& _NewItem);			//!< @brief Insert a new item after this zListLink
		TItem*				Remove();							//!< @brief Unlink current item and return value (iterator invalid afterward)
		TItem*				RemoveGoNext();						//!< @brief Unlink current item and go to next value
		TItem*				RemoveGoPrev();						//!< @brief Unlink current item and go to previous value

	protected:
							Iterator(zListLink* _pLink);
		zListLink*			mpLink = nullptr;					//!< Current link node pointed by iterator
		template<class T, zListLink T::*, bool> friend class zList;
	};

	//---------------------------------------------------------------------------------------------
	// Static methods
	//---------------------------------------------------------------------------------------------
public:	
	static bool				IsInAList(TItem& _Item);
	static TItem*			GetNext(TItem& _Item);								//!< @brief Get the next item (from current item) in the list
	static TItem*			GetPrev(TItem& _Item);								//!< @brief Get the previous item (from current item) in the list
	static void				Remove(TItem& _Item);								//!< @brief Remove the element from the list it's in
	static void				AddBefore(TItem& _Item, TItem& _NewItem);			//!< @brief Insert a new item before an item
	static void				AddAfter(TItem& _Item, TItem& _NewItem);			//!< @brief Insert a new item after an item

protected:	
	static TItem&			GetItemFromLink(const zListLink&);					//!< @brief Retrieve the item associated with a link object
	static zListLink&		GetLinkFromItem(const TItem& _Item);				//!< @brief Retrieve the link for this list type, from an object
	static void				AddBefore(zListLink& _Link, zListLink& _NewLink);	//!< @brief Internal use only, making sure we have the right link data from an item object
	static void				AddAfter(zListLink& _Link, zListLink& _NewLink);	//!< @brief Internal use only, making sure we have the right link data from an item object
	
	//---------------------------------------------------------------------------------------------
	// List method
	//---------------------------------------------------------------------------------------------
public:
							zList();
	void					PushHead(TItem& _Item);								//!< @brief Add an item to the beginning of this list
	void					PushTail(TItem& _Item);								//!< @brief Add an item to the end of this list
	TItem*					PopHead();											//!< @brief Get first item in the list and remove it (nullptr of none)
	TItem*					PopTail();											//!< @brief Get last item in the list and remove it (nullptr of none)

	TItem*					GetHead()const;										//!< @brief Get first item in the list (nullptr of none)
	TItem*					GetTail()const;										//!< @brief Get last item in the list (nullptr of none)

	Iterator				GetHeadIt()const;									//!< @brief Get iterator to first item
	Iterator				GetTailIt()const;									//!< @brief Get iterator to last item
	void					Empty();											//!< @brief Unlink all element, one by one
	bool					IsEmpty()const;										//!< @brief True if it contains no element

protected:
	zListLink				mRoot;												//!< Keep Head and Tail of list inside a regular zListLink
	zListLink* const		mpRootInvalid;										//!< Pointer to root link have 0x01 added to their address, to know when it is the root, and thus invalid
};


} } //namespace zen, Type

template< template<class> class TRefPtr, class TItem, zListLink TItem::* TLinkOffset>
class zListRef : public zList<TItem, TLinkOffset, false>
{
ZENClassDeclare(zListRef, zList)
public:
	//Missing iterator delete
	static void	Remove(TRefPtr<TItem>& _rItem)
	{
		Super::Remove(*_rItem.GetSafe());
		_rItem->ReferenceRem();
	}

	static void	AddBefore(TRefPtr<TItem>& _rItem, TRefPtr<TItem>& _rNewItem)
	{
		_rNewItem->ReferenceAdd();
		Super::AddBefore(*_rNewItem.GetSafe());
	}

	static void	AddAfter(TRefPtr<TItem>& _Item, TRefPtr<TItem>& _rNewItem)
	{
		_rNewItem->ReferenceAdd();
		Super::AddAfter(*_rNewItem.GetSafe());
	}

	void PushHead(TRefPtr<TItem>& _rNewItem)
	{
		_rNewItem->ReferenceAdd();
		Super::PushHead(*_rNewItem.GetSafe());
	}

	void PushTail(TRefPtr<TItem>& _rNewItem)
	{
		_rNewItem->ReferenceAdd();
		Super::PushHead(*_rNewItem.GetSafe());
	}

	TRefPtr<TItem> PopHead()
	{
		TRefPtr<TItem> rItem = Super::PopHead();
		if (rItem.IsValid())
			rItem->ReferenceRem();
		return rItem;
	}

	TRefPtr<TItem> PopTail()
	{
		TRefPtr<TItem> rItem = Super::PopTail();
		if (rItem.IsValid())
			rItem->ReferenceRem();
		return rItem;
	}
};

#endif	
