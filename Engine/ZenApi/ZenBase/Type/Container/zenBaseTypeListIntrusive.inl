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
template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
TItem& zList<TItem, TLinkOffset, TVirtualPad>::GetItemFromLink(const zListLink& _ItemLink)
{
	const size_t uLinkAdr				= (size_t)&_ItemLink;
	const size_t uMemberOffset			= zenOffsetOf(TLinkOffset);
	const size_t uVirtualTableOffset	= TVirtualPad ? sizeof(size_t) : 0;
	return *reinterpret_cast<TItem*>(uLinkAdr - (uMemberOffset+uVirtualTableOffset) );
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
zListLink& zList<TItem, TLinkOffset, TVirtualPad>::GetLinkFromItem(const TItem& _Item)
{
	const size_t uItemAdr				= (size_t)&_Item;
	const size_t uMemberOffset			= zenOffsetOf(TLinkOffset);
	const size_t uVirtualTableOffset	= TVirtualPad ? sizeof(size_t) : 0;
	return *reinterpret_cast<zListLink*>(uItemAdr + (uMemberOffset + uVirtualTableOffset));
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
void zList<TItem, TLinkOffset, TVirtualPad>::Remove(TItem& _Item)
{
	zListLink& ItemLink		= GetLinkFromItem(_Item);
	return ItemLink.Remove();
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
bool zList<TItem, TLinkOffset, TVirtualPad>::IsInAList(TItem& _Item)
{
	zListLink& ItemLink		= GetLinkFromItem(_Item);
	return ItemLink.IsInList();
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
TItem* zList<TItem, TLinkOffset, TVirtualPad>::GetNext(TItem& _Item)
{
	zListLink& ItemLink		= GetLinkFromItem(_Item);
	zListLink* pLinkNext	= ItemLink.GetNext();
	return pLinkNext ? &GetItemFromLink(*pLinkNext) : nullptr;
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
TItem* zList<TItem, TLinkOffset, TVirtualPad>::GetPrev(TItem& _Item)
{
	zListLink& ItemLink		= GetLinkFromItem(_Item);
	zListLink* pLinkPrev	= ItemLink.GetPrev();
	return pLinkPrev ? &GetItemFromLink(*pLinkPrev) : nullptr;
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
void zList<TItem, TLinkOffset, TVirtualPad>::AddBefore(zListLink& _Link, zListLink& _NewLink)
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

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
void zList<TItem, TLinkOffset, TVirtualPad>::AddAfter(zListLink& _Link, zListLink& _NewLink)
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

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
void zList<TItem, TLinkOffset, TVirtualPad>::AddBefore(TItem& _Item, TItem& _NewItem)
{
	zListLink& ItemLink			= GetLinkFromItem(_Item);
	zListLink& NewItemLink		= GetLinkFromItem(_NewItem);
	AddBefore(ItemLink, NewItemLink);
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
void zList<TItem, TLinkOffset, TVirtualPad>::AddAfter(TItem& _Item, TItem& _NewItem)
{
	zListLink& ItemLink			= GetLinkFromItem(_Item);
	zListLink& NewItemLink		= GetLinkFromItem(_NewItem);
	AddAfter(ItemLink, NewItemLink);
}


//#################################################################################################
// LIST ITERATOR
//#################################################################################################
template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
zList<TItem, TLinkOffset, TVirtualPad>::Iterator::Iterator()
{
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
zList<TItem, TLinkOffset, TVirtualPad>::Iterator::Iterator(const Iterator& _Copy)
{
	operator=(_Copy);
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
zList<TItem, TLinkOffset, TVirtualPad>::Iterator::Iterator(zListLink* _pLink)
: mpLink(_pLink)
{
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
bool zList<TItem, TLinkOffset, TVirtualPad>::Iterator::IsValid() const
{
	return mpLink != nullptr;
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
zList<TItem, TLinkOffset, TVirtualPad>::Iterator::operator bool() const
{
	return IsValid();
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
TItem* zList<TItem, TLinkOffset, TVirtualPad>::Iterator::Get() const
{
	return IsValid() ? &zList::GetItemFromLink(*mpLink) : nullptr;
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
TItem& zList<TItem, TLinkOffset, TVirtualPad>::Iterator::operator*()const
{
	ZENAssert(mpLink);
	return *Get();
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
TItem* zList<TItem, TLinkOffset, TVirtualPad>::Iterator::operator->()const
{
	return Get();
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
void zList<TItem, TLinkOffset, TVirtualPad>::Iterator::operator++()
{
	ZENAssert(mpLink);
	mpLink = mpLink->GetNext();
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
void zList<TItem, TLinkOffset, TVirtualPad>::Iterator::operator--()
{
	ZENAssert(mpLink);
	mpLink = mpLink->GetPrev();
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
typename const zList<TItem, TLinkOffset, TVirtualPad>::Iterator& zList<TItem, TLinkOffset, TVirtualPad>::Iterator::operator=(const Iterator& _Copy)
{
	mpLink = _Copy.mpLink;
	return *this;
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
TItem* zList<TItem, TLinkOffset, TVirtualPad>::Iterator::Remove()
{
	ZENAssert(mpLink);
	TItem* pItem	= mpLink->Get();
	mpLink->Remove();
	mpLink			= nullptr;
	return pItem;
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
TItem* zList<TItem, TLinkOffset, TVirtualPad>::Iterator::RemoveGoNext()
{
	ZENAssert(mpLink);
	zListLink* pUnlink	= mpLink;
	TItem pItem			= GoNext();
	pUnlink->Remove();
	return pItem;
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
TItem* zList<TItem, TLinkOffset, TVirtualPad>::Iterator::RemoveGoPrev()
{
	ZENAssert(mpLink);
	zListLink* pUnlink	= mpLink;
	TItem pItem			= GoPrevious();
	pUnlink->Remove();
	return pItem;
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
void zList<TItem, TLinkOffset, TVirtualPad>::Iterator::AddBefore(TItem& _NewItem)
{
	ZENAssert(IsValid());
	zList::AddBefore(*mpLink, zList::GetLinkFromItem(_NewItem));
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
void zList<TItem, TLinkOffset, TVirtualPad>::Iterator::AddAfter(TItem& _NewItem)
{
	ZENAssert(IsValid());
	zList::AddAfter(*mpLink, zList::GetLinkFromItem(_NewItem));
}

//#################################################################################################
// LIST
//#################################################################################################
template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
zList<TItem, TLinkOffset, TVirtualPad>::zList()
: mpRootInvalid(reinterpret_cast<zListLink*>((size_t)&mRoot | 1))
{	
	mRoot.mpPrevLink = mRoot.mpNextLink = mpRootInvalid;
	Empty();
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
bool zList<TItem, TLinkOffset, TVirtualPad>::IsEmpty()const
{
	return mRoot.GetNext() == nullptr;
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
void zList<TItem, TLinkOffset, TVirtualPad>::PushHead(TItem& _Item)
{
	AddAfter(*mpRootInvalid, GetLinkFromItem(_Item) );
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
void zList<TItem, TLinkOffset, TVirtualPad>::PushTail(TItem& _Item)
{
	AddBefore(*mpRootInvalid, GetLinkFromItem(_Item));
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
TItem* zList<TItem, TLinkOffset, TVirtualPad>::PopHead()
{
	if (IsEmpty())
		return nullptr;

	zListLink* pHeadLink = mRoot.GetNext();
	pHeadLink->Remove();
	return &GetItemFromLink(*pHeadLink);
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
TItem* zList<TItem, TLinkOffset, TVirtualPad>::PopTail()
{
	if (IsEmpty())
		return nullptr;

	zListLink* pTailLink = mRoot.GetPrev();
	pTailLink->Remove();
	return &GetItemFromLink(*pTailLink);
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
TItem* zList<TItem, TLinkOffset, TVirtualPad>::GetHead()const
{
	return IsEmpty() ? nullptr : &GetItemFromLink(*mRoot.GetNext());
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
TItem* zList<TItem, TLinkOffset, TVirtualPad>::GetTail()const
{
	return IsEmpty() ? nullptr : &GetItemFromLink(*mRoot.GetPrev());
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
typename zList<TItem, TLinkOffset, TVirtualPad>::Iterator zList<TItem, TLinkOffset, TVirtualPad>::GetHeadIt()const
{
	return Iterator(mRoot.GetNext());
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
typename zList<TItem, TLinkOffset, TVirtualPad>::Iterator zList<TItem, TLinkOffset, TVirtualPad>::GetTailIt()const
{
	return Iterator(mRoot.GetPrev());
}

template<class TItem, zListLink TItem::* TLinkOffset, bool TVirtualPad>
void zList<TItem, TLinkOffset, TVirtualPad>::Empty()
{
	while( IsEmpty() == false )
		mRoot.mpPrevLink->Remove();
}

} } //namespace zen, Type
#endif	
