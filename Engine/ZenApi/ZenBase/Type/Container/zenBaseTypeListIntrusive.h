#pragma once
#ifndef __zenApi_Base_Type_ListIntrusive_h__
#define __zenApi_Base_Type_ListIntrusive_h__

namespace zen { namespace zenType 
{

//=================================================================================================
//! @class	zList
//! @brief	Double 'intrusive' linked list.
//! @detail Intrusive list do not require additional allocation to manage, their list infos is 
//!			directly included in the object, making it more cache friendly and less heap intensive.
//!
//!			Example : look at SampleListIntrusive() for more infos
//!
//! @Note	This implementation supports adding an object to multiple list
//!			A class can't contain a list of itself
//=================================================================================================
template<class TListItem>
class zList
{
public:
	//=============================================================================================
	//! @class	Link
	//! @brief	Contain infos for accessing previous or next item in the list
	//=============================================================================================
	class Link
	{
	public:
		Link();
		~Link();

		TListItem*			Get();								//!< @brief Get current item tied to this list Link
		TListItem*			GetNext();							//!< @brief Get next item tied to this list Link
		TListItem*			GetPrev();							//!< @brief Get previous item tied to this list Link
		const TListItem*	Get()const;							//!< @brief Get current item tied to this list Link
		const TListItem*	GetNext()const;						//!< @brief Get next item tied to this list Link
		const TListItem*	GetPrev()const;						//!< @brief Get previous item tied to this list Link
		void				Unlink();							//!< @brief Remove item from the list this Link belongs to
		void				InsertBefore(TListItem& _NewItem);	//!< @brief Insert a new item before this Link
		void				InsertAfter(TListItem& _NewItem);	//!< @brief Insert a new item after this Link
		bool				IsLinked()const;					//!< @brief True if this link has been added to a list

	protected:
		Link*				GetNextItemLink()const;				//!< @brief Get the next item Link object
		size_t				GetOffset()const;					//!< @brief Get the offset between object and its link

		Link*				mpPrevLink;
		TListItem*			mpNextItem;

		template<class>
		friend class zList;
	};
	//=============================================================================================
	//! @class	Iterator
	//! @brief	Allow easy traversal of a list
	//=============================================================================================
	class Iterator
	{
	public:
							Iterator();
							Iterator(Iterator& _Copy);
							Iterator(Link* _Current);
		
		bool				IsValid();							//!< @brief Return true if pointing to valid data
		TListItem*			Get();								//!< @brief Get current item (NULL if none)
		const TListItem*	Get()const;							//!< @brief Get current item (NULL if none)
		TListItem*			operator*();						//!< @brief Get current item (NULL if none)
		const TListItem*	operator*()const;					//!< @brief Get current item (NULL if none)
		TListItem*			operator->();						//!< @brief Get current item (NULL if none)
		const TListItem*	operator->()const;					//!< @brief Get current item (NULL if none)
		void				operator++();						//!< @brief Go to next item
		void				operator--();						//!< @brief Go to previous item
		const Iterator&		operator=(Iterator& _Copy);			//!< @brief Assign iterator current location
		const Iterator&		operator=(Link* _Current);			//!< @brief Assign iterator current location
		TListItem*			GoNext();							//!< @brief Move iterator forward and return value found
		TListItem*			GoPrevious();						//!< @brief Move iterator backward and return value found
		void				InsertBefore(TListItem& _NewItem);	//!< @brief Insert a new item before this Link
		void				InsertAfter(TListItem& _NewItem);	//!< @brief Insert a new item after this Link
		TListItem*			Unlink();							//!< @brief Unlink current item and return value (iterator invalid afterward)
		TListItem*			UnlinkGoNext();						//!< @brief Unlink current item and go to next value
		TListItem*			UnlinkGoPrev();						//!< @brief Unlink current item and go to previous value
		
	protected:
		Link*				mpLink;
	};
	//=============================================================================================
	
	void					PushHead(TListItem& _Item);			//!< @brief Add an item to the begining of this list
	void					PushTail(TListItem& _Item);			//!< @brief Add an item to the end of this list
	TListItem*				PopHead();							//!< @brief Get first item in the list and remove it (NULL of none)
	TListItem*				PopTail();							//!< @brief Get last item in the list and remove it (NULL of none)
	
	TListItem*				GetHead();							//!< @brief Get first item in the list (NULL of none)
	TListItem*				GetTail();							//!< @brief Get last item in the list (NULL of none)
	const TListItem*		GetHead()const;						//!< @brief Get first item in the list (NULL of none)
	const TListItem*		GetTail()const;						//!< @brief Get last item in the list (NULL of none)

	Iterator				GetHeadIt()const;					//!< @brief Get iterator to first item
	Iterator				GetTailIt()const;					//!< @brief Get iterator to last item
	void					UnlinkAll();						//!< @brief Unlink all element, one by one
	void					RemoveAll();						//!< @brief Detach list from current elements (leaving them still chained together)

	bool					IsEmpty()const;						//!< @brief True if it contains no element
protected:
							zList(size_t _Offset);				//!< @brief Use zListDeclare to declare new list
	Link					mRoot;								//!< Pointer to first and last element of the list
	size_t					mOffset;							//!< Offset between object and its link
};

//=================================================================================================
//! @class	zListSpecialized
//! @brief	Transmit offset infos to base class
//=================================================================================================
template<class TListItem, typename zList<TListItem>::Link TListItem::* TLinkOffset>
class zListLinked : public zList<TListItem>
{
public:
	zListLinked();
};

#define zListDeclare(_Class_, _Link_) zListLinked<_Class_, &_Class_::_Link_>

} } //namespace zen, Type



#endif	
