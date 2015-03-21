#pragma once
#ifndef __zenBase_Type_ListIntrusive_inl__
#define __zenBase_Type_ListIntrusive_inl__

namespace zen {	namespace zenType 
{

//#################################################################################################
// LINK
//#################################################################################################
template<class TListItem>
zList<TListItem>::Link::Link()
: mpPrevLink(NULL)
, mpNextItem(NULL)
{}

template<class TListItem>
zList<TListItem>::Link::~Link()
{
	Unlink();
}

template<class TListItem>
TListItem* zList<TListItem>::Link::Get()
{
	ZENAssert(IsLinked());
	return mpPrevLink->GetNext();
}

template<class TListItem>
TListItem* zList<TListItem>::Link::GetNext()
{
	ZENAssert(IsLinked());
	return ((size_t)mpNextItem & 0x01) ? NULL : mpNextItem;
}

template<class TListItem>
TListItem* zList<TListItem>::Link::GetPrev()
{
	ZENAssert(IsLinked());
	return mpPrevLink->mpPrevLink->GetNext();
}

template<class TListItem>
const TListItem* zList<TListItem>::Link::Get() const
{
	ZENAssert(IsLinked());
	return mpPrevLink->GetNext();
}

template<class TListItem>
const TListItem* zList<TListItem>::Link::GetNext()const
{
	ZENAssert(IsLinked());
	return ((size_t)mpNextItem & 0x01) ? NULL : mpNextItem;
}

template<class TListItem>
const TListItem* zList<TListItem>::Link::GetPrev()const
{
	ZENAssert(IsLinked());
	return mpPrevLink->mpPrevLink->GetNext();
}


template<class TListItem>
void zList<TListItem>::Link::Unlink()
{
	if (IsLinked())
	{
		GetNextItemLink()->mpPrevLink = mpPrevLink;
		mpPrevLink->mpNextItem = mpNextItem;
		mpNextItem = NULL;
		mpPrevLink = NULL;
	}
}

template<class TListItem>
void zList<TListItem>::Link::InsertBefore(TListItem& _Item)
{
	Link* pNewLink = reinterpret_cast<Link*>((size_t)&_Item + GetOffset());
	pNewLink->Unlink();
	pNewLink->mpPrevLink = mpPrevLink;
	pNewLink->mpNextItem = mpPrevLink->mpNextItem;
	mpPrevLink->mpNextItem = &_Item;
	mpPrevLink = pNewLink;
}

template<class TListItem>
void zList<TListItem>::Link::InsertAfter(TListItem& _Item)
{
	GetNextItemLink()->InsertBefore(_Item);
}

template<class TListItem>
bool zList<TListItem>::Link::IsLinked()const
{
	return mpPrevLink != NULL;
}


template<class TListItem>
typename zList<TListItem>::Link* zList<TListItem>::Link::GetNextItemLink()const
{
	ZENAssert(IsLinked());
	return reinterpret_cast<Link*>((((size_t)mpNextItem) & ~0x1) + GetOffset());
}

template<class TListItem>
size_t zList<TListItem>::Link::GetOffset()const
{
	ZENAssert(IsLinked());
	return (size_t)this - (((size_t)mpPrevLink->mpNextItem) & ~0x1);
}

//#################################################################################################
// ITERATOR
//#################################################################################################
template<class TListItem>
zList<TListItem>::Iterator::Iterator()
: mpLink(NULL)
{
}

template<class TListItem>
zList<TListItem>::Iterator::Iterator(Iterator& _Copy)
{
	operator=(_Copy);
}

template<class TListItem>
zList<TListItem>::Iterator::Iterator(Link* _Current)
: mpLink(_Current)
{
}

template<class TListItem>
TListItem* zList<TListItem>::Iterator::Get()
{
	return mpLink ? mpLink->Get() : NULL;
}

template<class TListItem>
const TListItem* zList<TListItem>::Iterator::Get()const
{
	return mpLink ? mpLink->Get() : NULL;
}

template<class TListItem>
TListItem* zList<TListItem>::Iterator::operator*()
{
	return Get();
}

template<class TListItem>
const TListItem* zList<TListItem>::Iterator::operator*()const
{
	return Get();
}

template<class TListItem>
TListItem* zList<TListItem>::Iterator::GoNext()
{
	ZENAssert(mpLink);
	mpLink = mpLink->GetNextItemLink();
	return mpLink->Get();
}

template<class TListItem>
TListItem* zList<TListItem>::Iterator::GoPrevious()
{
	ZENAssert(mpLink);
	mpLink = mpLink->mpPrevLink;
	return mpLink->Get();
}

template<class TListItem>
void zList<TListItem>::Iterator::operator++()
{
	GoNext();	
}

template<class TListItem>
void zList<TListItem>::Iterator::operator--()
{
	GoPrevious();
}

template<class TListItem>
TListItem* zList<TListItem>::Iterator::operator->()
{
	return Get();
}

template<class TListItem>
const TListItem* zList<TListItem>::Iterator::operator->()const
{
	return Get();
}

template<class TListItem>
typename const zList<TListItem>::Iterator& zList<TListItem>::Iterator::operator=(Iterator& _Copy)
{
	mpLink = _Copy.mpLink;
	return *this;
}

template<class TListItem>
typename const zList<TListItem>::Iterator& zList<TListItem>::Iterator::operator=(Link* _Current)
{
	mpLink = _Current;
	return *this;
}

template<class TListItem>
TListItem* zList<TListItem>::Iterator::Unlink()
{
	ZENAssert(mpLink);
	TListItem* pItem = mpLink->Get();
	mpLink->Unlink();	
	mpLink = NULL;
	return pItem;
}

template<class TListItem>
TListItem* zList<TListItem>::Iterator::UnlinkGoNext()
{
	ZENAssert(mpLink);
	Link* pUnlink		= mpLink;
	TListItem* pItem	= GoNext();
	pUnlink->Unlink();	
	return pItem;
}

template<class TListItem>
TListItem* zList<TListItem>::Iterator::UnlinkGoPrev()
{
	ZENAssert(mpLink);
	Link* pUnlink		= mpLink;
	TListItem* pItem	= GoPrevious();
	pUnlink->Unlink();
	return pItem;
}

//#################################################################################################
// LIST
//#################################################################################################
template<class TListItem>
zList<TListItem>::zList(size_t _Offset)
: mOffset(_Offset)
{
	RemoveAll();
}

template<class TListItem>			
void zList<TListItem>::PushHead(TListItem& _Item)
{
	mRoot.InsertAfter(_Item);
}

template<class TListItem>
void zList<TListItem>::PushTail(TListItem& _Item)
{
	mRoot.InsertBefore(_Item);
}

template<class TListItem>
TListItem* zList<TListItem>::PopHead()
{
	if( IsEmpty() )
		return NULL;
	TListItem* pHead = mRoot.GetNext();
	mRoot.GetNextItemLink()->Unlink();
	return pHead;
}

template<class TListItem>
TListItem* zList<TListItem>::PopTail()
{
	if( IsEmpty() )
		return NULL;
	TListItem* pTail = mRoot.GetPrev();
	mRoot.mpPrevLink->Unlink();
	return pTail;
}

template<class TListItem>
TListItem* zList<TListItem>::GetHead()
{
	return mRoot.GetNext();
}

template<class TListItem>
TListItem* zList<TListItem>::GetTail()
{
	return mRoot.GetPrev();
}

template<class TListItem>
const TListItem* zList<TListItem>::GetHead()const
{
	return mRoot.GetNext();
}

template<class TListItem>
const TListItem* zList<TListItem>::GetTail()const
{
	return mRoot.GetPrev();
}

template<class TListItem>
typename zList<TListItem>::Iterator zList<TListItem>::GetHeadIt()const
{
	return Iterator(mRoot.GetNextItemLink());
}

template<class TListItem>
typename zList<TListItem>::Iterator zList<TListItem>::GetTailIt()const
{
	return Iterator(mRoot.mpPrevLink);
}

template<class TListItem>
void zList<TListItem>::UnlinkAll()
{
	const Link* root = &mRoot;
	while (mRoot.mpPrevLink != root)
		mRoot.mpPrevLink->Unlink();
}

template<class TListItem>
void zList<TListItem>::RemoveAll()
{
	mRoot.Unlink();
	mRoot.mpPrevLink = reinterpret_cast<Link*>((size_t)&mRoot);
	mRoot.mpNextItem = reinterpret_cast<TListItem*>((size_t)&mRoot - mOffset + 1);
}

template<class TListItem>
bool zList<TListItem>::IsEmpty()const
{
	return mRoot.GetNext() == NULL;
}

//#################################################################################################
// LIST SPECIALIZED
//#################################################################################################
template<class TListItem, typename zList<TListItem>::Link TListItem::* TLinkOffset>
zListLinked<TListItem, TLinkOffset>::zListLinked()
: zList( zenOffsetOf(TLinkOffset) )
{
}

} } //namespace zen, Type

#endif	
