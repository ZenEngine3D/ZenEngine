#pragma once
#ifndef __zenBase_Type_ListIntrusive_inl__
#define __zenBase_Type_ListIntrusive_inl__

namespace zen {	namespace zenType 
{

//#################################################################################################
// LIST LINK
//#################################################################################################
zListLink::~zListLink()
{
	// If item wasn't removed from list prior to destruction, unlink it 
	// (but unable to call the zList::Remove(), so child class of zList won't be able 
	//  to do special destruction operations)
	Remove();
}

bool zListLink::IsInList()const
{
	return mpNextLink != nullptr;
}

zListLink* zListLink::GetNext()const
{
	return ((size_t)mpNextLink & 0x1) ? nullptr : mpNextLink; //If link pointer contains 0x01, we know it points to list root, and is invalid
}

zListLink* zListLink::GetPrev()const
{
	return ((size_t)mpPrevLink & 0x1) ? nullptr : mpPrevLink; //If link pointer contains 0x01, we know it points to list root, and is invalid
}

void zListLink::SetNull()
{
	mpNextLink = mpPrevLink = nullptr;
}

void zListLink::Remove()
{
	if (IsInList())
	{
		zListLink* pValidNext = reinterpret_cast<zListLink*>((size_t)mpNextLink & ~0x1);
		zListLink* pValidPrev = reinterpret_cast<zListLink*>((size_t)mpPrevLink & ~0x1);
		pValidNext->mpPrevLink = mpPrevLink;
		pValidPrev->mpNextLink = mpNextLink;
		mpNextLink = nullptr;
		mpPrevLink = nullptr;
	}
}

//#################################################################################################
// LIST (Static methods)
//#################################################################################################
template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
TItem& zList<TItem, TLinkOffset, TVirtual1stTime>::GetItemFromLink(const zListLink& _ItemLink)
{
	const size_t uLinkAdr				= (size_t)&_ItemLink;
	const size_t uMemberOffset			= zenOffsetOf(TLinkOffset);
	const size_t uVirtualTableOffset	= TVirtual1stTime ? sizeof(size_t) : 0;
	return *reinterpret_cast<TItem*>(uLinkAdr - (uMemberOffset+uVirtualTableOffset) );
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
zListLink& zList<TItem, TLinkOffset, TVirtual1stTime>::GetLinkFromItem(const TItem& _Item)
{
	const size_t uItemAdr				= (size_t)&_Item;
	const size_t uMemberOffset			= zenOffsetOf(TLinkOffset);
	const size_t uVirtualTableOffset	= TVirtual1stTime ? sizeof(size_t) : 0;
	return *reinterpret_cast<zListLink*>(uItemAdr + (uMemberOffset + uVirtualTableOffset));
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
void zList<TItem, TLinkOffset, TVirtual1stTime>::Remove(TItem& _Item)
{
	zListLink& ItemLink		= GetLinkFromItem(_Item);
	return ItemLink.Remove();
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
bool zList<TItem, TLinkOffset, TVirtual1stTime>::IsInAList(TItem& _Item)
{
	zListLink& ItemLink		= GetLinkFromItem(_Item);
	return ItemLink.IsInList();
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
TItem* zList<TItem, TLinkOffset, TVirtual1stTime>::GetNext(TItem& _Item)
{
	zListLink& ItemLink		= GetLinkFromItem(_Item);
	zListLink* pLinkNext	= ItemLink.GetNext();
	return pLinkNext ? &GetItemFromLink(*pLinkNext) : nullptr;
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
TItem* zList<TItem, TLinkOffset, TVirtual1stTime>::GetPrev(TItem& _Item)
{
	zListLink& ItemLink		= GetLinkFromItem(_Item);
	zListLink* pLinkPrev	= ItemLink.GetPrev();
	return pLinkPrev ? &GetItemFromLink(*pLinkPrev) : nullptr;
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
void zList<TItem, TLinkOffset, TVirtual1stTime>::AddBefore(zListLink& _Link, zListLink& _NewLink)
{	
	//Pointers to root link adds 0x01 address, mask it out when accessing memory in case link is root node
	_NewLink.Remove();
	 zListLink& LinkSafe		= *reinterpret_cast<zListLink*>((size_t)&_Link & ~0x1);
	 zListLink& LinkSafePrev	= *reinterpret_cast<zListLink*>((size_t)LinkSafe.mpPrevLink & ~0x1);	
	_NewLink.mpPrevLink			= LinkSafe.mpPrevLink;
	_NewLink.mpNextLink			= &_Link;
	LinkSafePrev.mpNextLink		= &_NewLink;
	LinkSafe.mpPrevLink			= &_NewLink;
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
void zList<TItem, TLinkOffset, TVirtual1stTime>::AddAfter(zListLink& _Link, zListLink& _NewLink)
{
	//Pointers to root link adds 0x01 address, mask it out when accessing memory in case link is root node
	_NewLink.Remove();
	 zListLink& LinkSafe		= *reinterpret_cast<zListLink*>((size_t)&_Link & ~0x1); 
	 zListLink& LinkSafeNext	= *reinterpret_cast<zListLink*>((size_t)LinkSafe.mpNextLink & ~0x1);	
	_NewLink.mpNextLink			= LinkSafe.mpNextLink;
	_NewLink.mpPrevLink			= &_Link;
	LinkSafeNext.mpPrevLink		= &_NewLink;
	LinkSafe.mpNextLink			= &_NewLink;
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
void zList<TItem, TLinkOffset, TVirtual1stTime>::AddBefore(TItem& _Item, TItem& _NewItem)
{
	zListLink& ItemLink			= GetLinkFromItem(_Item);
	zListLink& NewItemLink		= GetLinkFromItem(_NewItem);
	AddBefore(ItemLink, NewItemLink);
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
void zList<TItem, TLinkOffset, TVirtual1stTime>::AddAfter(TItem& _Item, TItem& _NewItem)
{
	zListLink& ItemLink			= GetLinkFromItem(_Item);
	zListLink& NewItemLink		= GetLinkFromItem(_NewItem);
	AddAfter(ItemLink, NewItemLink);
}


//#################################################################################################
// LIST ITERATOR
//#################################################################################################
template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
zList<TItem, TLinkOffset, TVirtual1stTime>::Iterator::Iterator()
{
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
zList<TItem, TLinkOffset, TVirtual1stTime>::Iterator::Iterator(const Iterator& _Copy)
{
	operator=(_Copy);
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
zList<TItem, TLinkOffset, TVirtual1stTime>::Iterator::Iterator(zListLink* _pLink)
: mpLink(_pLink)
{
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
bool zList<TItem, TLinkOffset, TVirtual1stTime>::Iterator::IsValid() const
{
	return mpLink != nullptr;
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
zList<TItem, TLinkOffset, TVirtual1stTime>::Iterator::operator bool() const
{
	return IsValid();
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
TItem* zList<TItem, TLinkOffset, TVirtual1stTime>::Iterator::Get() const
{
	return IsValid() ? &zList::GetItemFromLink(*mpLink) : nullptr;
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
TItem& zList<TItem, TLinkOffset, TVirtual1stTime>::Iterator::operator*()const
{
	zenAssert(mpLink);
	return *Get();
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
TItem* zList<TItem, TLinkOffset, TVirtual1stTime>::Iterator::operator->()const
{
	return Get();
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
void zList<TItem, TLinkOffset, TVirtual1stTime>::Iterator::operator++()
{
	zenAssert(mpLink);
	mpLink = mpLink->GetNext();
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
void zList<TItem, TLinkOffset, TVirtual1stTime>::Iterator::operator--()
{
	zenAssert(mpLink);
	mpLink = mpLink->GetPrev();
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
typename const zList<TItem, TLinkOffset, TVirtual1stTime>::Iterator& zList<TItem, TLinkOffset, TVirtual1stTime>::Iterator::operator=(const Iterator& _Copy)
{
	mpLink = _Copy.mpLink;
	return *this;
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
TItem* zList<TItem, TLinkOffset, TVirtual1stTime>::Iterator::Remove()
{
	zenAssert(mpLink);
	TItem* pItem	= mpLink->Get();
	mpLink->Remove();
	mpLink			= nullptr;
	return pItem;
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
TItem* zList<TItem, TLinkOffset, TVirtual1stTime>::Iterator::RemoveGoNext()
{
	zenAssert(mpLink);
	zListLink* pUnlink	= mpLink;
	TItem pItem			= GoNext();
	pUnlink->Remove();
	return pItem;
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
TItem* zList<TItem, TLinkOffset, TVirtual1stTime>::Iterator::RemoveGoPrev()
{
	zenAssert(mpLink);
	zListLink* pUnlink	= mpLink;
	TItem pItem			= GoPrevious();
	pUnlink->Remove();
	return pItem;
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
void zList<TItem, TLinkOffset, TVirtual1stTime>::Iterator::AddBefore(TItem& _NewItem)
{
	zenAssert(IsValid());
	zList::AddBefore(*mpLink, zList::GetLinkFromItem(_NewItem));
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
void zList<TItem, TLinkOffset, TVirtual1stTime>::Iterator::AddAfter(TItem& _NewItem)
{
	zenAssert(IsValid());
	zList::AddAfter(*mpLink, zList::GetLinkFromItem(_NewItem));
}

//#################################################################################################
// LIST
//#################################################################################################
template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
zList<TItem, TLinkOffset, TVirtual1stTime>::zList()
: mpRootInvalid(reinterpret_cast<zListLink*>((size_t)&mRoot | 1))
{	
	mRoot.mpPrevLink = mRoot.mpNextLink = mpRootInvalid;
	Empty();
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
bool zList<TItem, TLinkOffset, TVirtual1stTime>::IsEmpty()const
{
	return mRoot.GetNext() == nullptr;
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
void zList<TItem, TLinkOffset, TVirtual1stTime>::PushHead(TItem& _Item)
{
	AddAfter(*mpRootInvalid, GetLinkFromItem(_Item) );
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
void zList<TItem, TLinkOffset, TVirtual1stTime>::PushTail(TItem& _Item)
{
	AddBefore(*mpRootInvalid, GetLinkFromItem(_Item));
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
TItem* zList<TItem, TLinkOffset, TVirtual1stTime>::PopHead()
{
	if (IsEmpty())
		return nullptr;

	zListLink* pHeadLink = mRoot.GetNext();
	pHeadLink->Remove();
	return &GetItemFromLink(*pHeadLink);
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
TItem* zList<TItem, TLinkOffset, TVirtual1stTime>::PopTail()
{
	if (IsEmpty())
		return nullptr;

	zListLink* pTailLink = mRoot.GetPrev();
	pTailLink->Remove();
	return &GetItemFromLink(*pTailLink);
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
TItem* zList<TItem, TLinkOffset, TVirtual1stTime>::GetHead()const
{
	return IsEmpty() ? nullptr : &GetItemFromLink(*mRoot.GetNext());
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
TItem* zList<TItem, TLinkOffset, TVirtual1stTime>::GetTail()const
{
	return IsEmpty() ? nullptr : &GetItemFromLink(*mRoot.GetPrev());
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
typename zList<TItem, TLinkOffset, TVirtual1stTime>::Iterator zList<TItem, TLinkOffset, TVirtual1stTime>::GetHeadIt()const
{
	return Iterator(mRoot.GetNext());
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
typename zList<TItem, TLinkOffset, TVirtual1stTime>::Iterator zList<TItem, TLinkOffset, TVirtual1stTime>::GetTailIt()const
{
	return Iterator(mRoot.GetPrev());
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtual1stTime>
void zList<TItem, TLinkOffset, TVirtual1stTime>::Empty()
{
	while( IsEmpty() == false )
		mRoot.mpPrevLink->Remove();
}

} } //namespace zen, Type
#endif	
