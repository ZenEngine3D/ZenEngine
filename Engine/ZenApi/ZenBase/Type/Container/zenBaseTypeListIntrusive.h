#pragma once
#ifndef __zenApi_Base_Type_ListIntrusive_h__
#define __zenApi_Base_Type_ListIntrusive_h__

namespace zen { namespace zenType 
{

//=============================================================================================
//! @class	zListLink
//! @brief	Contain infos to reach previous and next item in a list
//! @detail For each list a class can be a member of, add 1 zListLink member. This allow
//!			having intrusive list support (much faster and safer than external container list)
//!			that support multiple lists instead of being limited to one through inheritance.
//=============================================================================================
class zListLink
{
public:
	ZENInline							~zListLink();		
	
protected:	
	ZENInline void						Remove();							//!< @brief Remove item from the list this zListLink belongs to	
	ZENInline void						InsertBefore(zListLink& _NewLink);	//!< @brief Insert a new item before this zListLink
	ZENInline void						InsertAfter(zListLink& _NewLink);	//!< @brief Insert a new item after this zListLink	
	ZENInline zListLink*				GetNext()const;						//!< @brief Get the next item zListLink object
	ZENInline zListLink*				GetPrev()const;						//!< @brief Get the previous item zListLink object
	ZENInline zListLink*				GetNextValid()const;				//!< @brief For internal use, valid even if pointing to list root
	ZENInline zListLink*				GetPrevValid()const;				//!< @brief For internal use, valid even if pointing to list root
	ZENInline bool						IsInList()const;					//!< @brief True if this link has been added to a list	
	zListLink*							mpPrevLink = nullptr;				//!< Pointer to next link (lower bit set to 1 if pointing to list root, invalid)
	zListLink*							mpNextLink = nullptr;				//!< Pointer to previous link (lower bit set to 1 if pointing to list root, invalid)
	template<class>						friend class zListBase;
	template<class T, zListLink T::*>	friend class zList;
};

//=================================================================================================
//! @class	zListBase
//! @brief	Double 'intrusive' linked list
//! @detail Intrusive list do not require additional allocation to manage, their list infos is 
//!			directly included in the object, making it more cache friendly and less heap intensive.
//!			This class contain all the implementation details, use zList for declarating the list.
//!			Example : look at SampleListIntrusive() for more infos
//!
//! @Note	This implementation supports adding an object to multiple list
//=================================================================================================
template<class TItem>
class zListBase
{
public:
	//=============================================================================================
	//! @class	Iterator
	//! @brief	Allow easy traversal of a list
	//=============================================================================================
	class Iterator
	{
	public:
									Iterator();
									Iterator(const Iterator& _Copy);									
		
		bool						IsValid()const;						//!< @brief Return true if pointing to valid data
		TItem*						Get()const;							//!< @brief Get current item (nullptr if none)
		TItem*						operator*()const;					//!< @brief Get current item (nullptr if none)
		TItem*						operator->()const;					//!< @brief Get current item (nullptr if none)
		void						operator++();						//!< @brief Go to next item
		void						operator--();						//!< @brief Go to previous item
		const Iterator&				operator=(const Iterator& _Copy);	//!< @brief Assign iterator current location
		void						InsertBefore(TItem& _NewItem);		//!< @brief Insert a new item before this zListLink
		void						InsertAfter(TItem& _NewItem);		//!< @brief Insert a new item after this zListLink
		TItem*						Remove();							//!< @brief Unlink current item and return value (iterator invalid afterward)
		TItem*						RemoveGoNext();						//!< @brief Unlink current item and go to next value
		TItem*						RemoveGoPrev();						//!< @brief Unlink current item and go to previous value
		
	protected:
									Iterator(zListLink* _pLink, size_t _uOffset);
		zListLink*					mpLink			= nullptr;			//!< Current link node pointed by iterator
		size_t						muOffset		= 0;				//!< Offset between Object and the Link data member
		template<class>				friend class zListBase;
	};
	//=============================================================================================
	
	void							PushHead(TItem& _Item);				//!< @brief Add an item to the beginning of this list
	void							PushTail(TItem& _Item);				//!< @brief Add an item to the end of this list
	TItem*							PopHead();							//!< @brief Get first item in the list and remove it (nullptr of none)
	TItem*							PopTail();							//!< @brief Get last item in the list and remove it (nullptr of none)
	
	TItem*							GetHead()const;						//!< @brief Get first item in the list (nullptr of none)
	TItem*							GetTail()const;						//!< @brief Get last item in the list (nullptr of none)

	Iterator						GetHeadIt()const;					//!< @brief Get iterator to first item
	Iterator						GetTailIt()const;					//!< @brief Get iterator to last item
	void							Empty();							//!< @brief Unlink all element, one by one
	bool							IsEmpty()const;						//!< @brief True if it contains no element

protected:
									zListBase(size_t _Offset);			//!< @brief Use zList to declare new list
	zListLink						mRoot;								//!< Pointer to first and last element of the list
	size_t							muOffset = 0;						//!< Offset between object and its link
};

//=================================================================================================
//! @class	zList
//! @brief	Class used to declare a new list type
//! @detail	The template expect the type of the object stored in the list, and a member pointer to
//!			object listlink used to store this list info.
//!			Exemple: zList<ExempleClass, &ExempleClass::myLink> myList;
//!			Example: look at SampleListIntrusive() for more infos
//=================================================================================================
template<class TItem, zListLink TItem::* TLinkOffset>
class zList : public zListBase<TItem>
{
public:
							zList();
	static void				Remove(TItem& _Item);
	static bool				IsInList(TItem& _Item);
	static TItem*			GetNext(TItem& _Item);							//!< @brief Get the next item (from current item) in the list
	static TItem*			GetPrev(TItem& _Item);							//!< @brief Get the previous item (from current item) in the list
	static void				InsertBefore(TItem& _Item, TItem& _NewItem);	//!< @brief Insert a new item before an item
	static void				InsertAfter(TItem& _Item, TItem& _NewItem);		//!< @brief Insert a new item after an item
};


} } //namespace zen, Type



#endif	
