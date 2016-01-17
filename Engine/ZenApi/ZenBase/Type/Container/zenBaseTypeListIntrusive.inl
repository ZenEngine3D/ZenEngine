#pragma once
#ifndef __zenBase_Type_ListIntrusive_inl__
#define __zenBase_Type_ListIntrusive_inl__

namespace zen {	namespace zenType 
{

template<class TItem>
TItem& zListGetItem(const zListLink& _ItemLink, size_t _uOffset)
{
	return *reinterpret_cast<TItem*>((size_t)&_ItemLink - _uOffset);
}

template<class TItem>
zListLink& zListGetLink(const TItem& _Item, size_t _uOffset)
{
	return *reinterpret_cast<zListLink*>((size_t)&_Item + _uOffset);
}

//#################################################################################################
// LINK
//#################################################################################################

zListLink::~zListLink()
{
	Remove();
}

bool zListLink::IsInList()const
{
	return mpNextLink != nullptr;
}

zListLink* zListLink::GetNext()const
{
	return ((size_t)mpNextLink & 0x1) ? nullptr : mpNextLink;
}

zListLink* zListLink::GetPrev()const
{
	return ((size_t)mpPrevLink & 0x1) ? nullptr : mpPrevLink;
}

zListLink* zListLink::GetNextValid()const
{
	return reinterpret_cast<zListLink*>((size_t)mpNextLink & ~0x1);
}

zListLink* zListLink::GetPrevValid()const
{
	return reinterpret_cast<zListLink*>((size_t)mpPrevLink & ~0x1);
}
void zListLink::Remove()
{
	if (IsInList())
	{
		GetNextValid()->mpPrevLink	= mpPrevLink;
		GetPrevValid()->mpNextLink	= mpNextLink;
		mpNextLink					= nullptr;
		mpPrevLink					= nullptr;
	}
}

void zListLink::InsertBefore(zListLink& _NewLink)
{	
	_NewLink.Remove();
	_NewLink.mpPrevLink				= mpPrevLink;
	_NewLink.mpNextLink				= this;
	GetPrevValid()->mpNextLink		= &_NewLink;
	mpPrevLink						= &_NewLink;
}

void zListLink::InsertAfter(zListLink& _NewLink)
{
	_NewLink.Remove();
	_NewLink.mpNextLink				= mpNextLink;
	_NewLink.mpPrevLink				= this;
	GetNextValid()->mpPrevLink		= &_NewLink;
	mpNextLink						= &_NewLink;
}

//#################################################################################################
// ITERATOR
//#################################################################################################
template<class TItem>
zListBase<TItem>::Iterator::Iterator()
{
}

template<class TItem>
zListBase<TItem>::Iterator::Iterator(const Iterator& _Copy)
{
	operator=(_Copy);
}

template<class TItem>
zListBase<TItem>::Iterator::Iterator(zListLink* _pLink, size_t _uOffset)
: mpLink(_pLink)
, muOffset(_uOffset)
{
}

template<class TItem>
bool zListBase<TItem>::Iterator::IsValid() const
{
	return mpLink;
}

template<class TItem>
TItem* zListBase<TItem>::Iterator::Get() const
{
	return IsValid() ?  &zListGetItem<TItem>(*mpLink, muOffset) : nullptr;
}

template<class TItem>
TItem* zListBase<TItem>::Iterator::operator*()const
{
	return Get();
}

template<class TItem>
TItem* zListBase<TItem>::Iterator::operator->()const
{
	return Get();
}

template<class TItem>
void zListBase<TItem>::Iterator::operator++()
{
	ZENAssert(mpLink);
	mpLink = mpLink->GetNext();
}

template<class TItem>
void zListBase<TItem>::Iterator::operator--()
{
	ZENAssert(mpLink);
	mpLink = mpLink->GetPrev();
}



template<class TItem>
typename const zListBase<TItem>::Iterator& zListBase<TItem>::Iterator::operator=(const Iterator& _Copy)
{
	mpLink		= _Copy.mpLink;
	muOffset	= _Copy.muOffset;
	return *this;
}

template<class TItem>
TItem* zListBase<TItem>::Iterator::Remove()
{
	ZENAssert(mpLink);
	TItem* pItem = mpLink->Get();
	mpLink->Unlink();	
	mpLink = nullptr;
	return pItem;
}

template<class TItem>
TItem* zListBase<TItem>::Iterator::RemoveGoNext()
{
	ZENAssert(mpLink);
	zListLink* pUnlink		= mpLink;
	TItem pItem	= GoNext();
	pUnlink->Unlink();	
	return pItem;
}

template<class TItem>
TItem* zListBase<TItem>::Iterator::RemoveGoPrev()
{
	ZENAssert(mpLink);
	zListLink* pUnlink	= mpLink;
	TItem pItem	= GoPrevious();
	pUnlink->Unlink();
	return pItem;
}

template<class TItem>
void zListBase<TItem>::Iterator::InsertBefore(TItem& _NewItem )
{
	ZENAssert(IsValid());
	mpLink->InsertBefore(zListGetLink<TItem>(_NewItem, muOffset));
}

template<class TItem>
void zListBase<TItem>::Iterator::InsertAfter(TItem& _NewItem)
{
	ZENAssert(IsValid());
	mpLink->InsertAfter(zListGetLink<TItem>(_NewItem, muOffset));
}

//#################################################################################################
// LIST
//#################################################################################################
template<class TItem>
zListBase<TItem>::zListBase(size_t _Offset)
: muOffset(_Offset)
{
	mRoot.mpNextLink = reinterpret_cast<zListLink*>((size_t)&mRoot | 1);	// Tell list link it's pointing to root node (invalid)
	mRoot.mpPrevLink = reinterpret_cast<zListLink*>((size_t)&mRoot | 1);	// Tell list link it's pointing to root node (invalid)
	Empty();
}

template<class TItem>
void zListBase<TItem>::PushHead(TItem& _Item)
{
	mRoot.InsertAfter( zListGetLink<TItem>(_Item, muOffset) );
	mRoot.mpNextLink->mpPrevLink = reinterpret_cast<zListLink*>((size_t)&mRoot | 1);	// Tell newly inserted item, previous link is invalid(root list link)
}

template<class TItem>
void zListBase<TItem>::PushTail(TItem& _Item)
{
	mRoot.InsertBefore( zListGetLink<TItem>(_Item, muOffset) );
	mRoot.mpPrevLink->mpNextLink  = reinterpret_cast<zListLink*>((size_t)&mRoot | 1);	// Tell newly inserted item, next link is invalid(root list link)
}

template<class TItem>
TItem* zListBase<TItem>::PopHead()
{
	if( IsEmpty() )
		return nullptr;

	zListLink* pHeadLink = mRoot.GetNext();
	pHeadLink->Remove();
	return &zListGetItem<TItem>(*pHeadLink, muOffset);
}

template<class TItem>
TItem* zListBase<TItem>::PopTail()
{
	if( IsEmpty() )
		return nullptr;

	zListLink* pTailLink = mRoot.GetPrev();
	pTailLink->Remove();
	return &zListGetItem<TItem>(*pTailLink, muOffset);
}

template<class TItem>
TItem* zListBase<TItem>::GetHead()const
{
	return &zListGetItem<TItem>(*mRoot.GetNext(), muOffset);
}

template<class TItem>
TItem* zListBase<TItem>::GetTail()const
{
	return &zListGetItem<TItem>(*mRoot.GetPrev(), muOffset);
}

template<class TItem>
typename zListBase<TItem>::Iterator zListBase<TItem>::GetHeadIt()const
{
	return Iterator(mRoot.GetNext(), muOffset);
}

template<class TItem>
typename zListBase<TItem>::Iterator zListBase<TItem>::GetTailIt()const
{
	return Iterator(mRoot.GetPrev(), muOffset);
}

template<class TItem>
void zListBase<TItem>::Empty()
{
	while( ((size_t) mRoot.mpPrevLink & 0x01) == 0 )
		mRoot.mpPrevLink->Remove();
}

template<class TItem>
bool zListBase<TItem>::IsEmpty()const
{
	return ((size_t)mRoot.mpPrevLink & 0x01) == 1;
}

//#################################################################################################
// LIST SPECIALIZED
//#################################################################################################

template<class TItem, zListLink TItem::* TLinkOffset>
zList<TItem, TLinkOffset>::zList()
: zListBase( zenOffsetOf(TLinkOffset) )
{
}

template<class TItem, zListLink TItem::* TLinkOffset>
void zList<TItem, TLinkOffset>::Remove(TItem& _Item)
{
	zListLink& ItemLink = zListGetLink<TItem>(_Item, zenOffsetOf(TLinkOffset));
	return ItemLink.Remove();
}

template<class TItem, zListLink TItem::* TLinkOffset>
bool zList<TItem, TLinkOffset>::IsInList(TItem& _Item)
{
	zListLink& ItemLink = zListGetLink<TItem>(_Item, zenOffsetOf(TLinkOffset));
	return ItemLink.IsInList();
}

template<class TItem, zListLink TItem::* TLinkOffset>
TItem* zList<TItem, TLinkOffset>::GetNext(TItem& _Item)
{
	zListLink& ItemLink		= zListGetLink<TItem>(_Item, zenOffsetOf(TLinkOffset));
	zListLink* pLinkNext	= ItemLink.GetNext();
	return pLinkNext ? &zListGetItem<TItem>(*pLinkNext, zenOffsetOf(TLinkOffset)) : nullptr;
}

template<class TItem, zListLink TItem::* TLinkOffset>
TItem* zList<TItem, TLinkOffset>::GetPrev(TItem& _Item)
{
	zListLink& ItemLink		= zListGetLink<TItem>(_Item, zenOffsetOf(TLinkOffset));
	zListLink* pLinkPrev	= ItemLink.GetPrev();
	return pLinkPrev ? &zListGetItem<TItem>(*pLinkPrev, zenOffsetOf(TLinkOffset)) : nullptr;
}

template<class TItem, zListLink TItem::* TLinkOffset>
void zList<TItem, TLinkOffset>::InsertBefore(TItem& _Item, TItem& _NewItem)
{
	zListLink& ItemLink = zListGetLink<TItem>(_Item, zenOffsetOf(TLinkOffset));
	zListLink& NewItemLink = zListGetLink<TItem>(_NewItem, zenOffsetOf(TLinkOffset));
	ItemLink.InsertBefore(NewItemLink);
}

template<class TItem, zListLink TItem::* TLinkOffset>
void zList<TItem, TLinkOffset>::InsertAfter(TItem& _Item, TItem& _NewItem)
{
	zListLink& ItemLink = zListGetLink<TItem>(_Item, zenOffsetOf(TLinkOffset));
	zListLink& NewItemLink = zListGetLink<TItem>(_NewItem, zenOffsetOf(TLinkOffset));
	ItemLink.InsertAfter(NewItemLink);
}

} } //namespace zen, Type

#endif	
