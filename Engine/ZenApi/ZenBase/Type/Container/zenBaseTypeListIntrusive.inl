#pragma once

namespace zen {	namespace zenType 
{

//=================================================================================================
// zListLink
//=================================================================================================
bool zListLink::IsInList()const 
{ 
	return mpPrev != nullptr && mpNext != nullptr; 
}

bool zListLink::IsItem()const 
{ 
	return (reinterpret_cast<size_t>(mpNext) & 0x01) == 0; 
}

void zListLink::Reset() 
{ 
	mpNext = nullptr; 
	mpPrev = nullptr; 
}

zListLink* zListLink::Next()const 
{ 
	zenAssert(mpNext); 
	return reinterpret_cast<zListLink*>(reinterpret_cast<size_t>(mpNext) & ~0x01); 
}

zListLink* zListLink::Prev()const 
{ 
	zenAssert(mpPrev);
	return reinterpret_cast<zListLink*>(reinterpret_cast<size_t>(mpPrev) & ~0x01); 
}

void zListLink::SetAsRoot() 
{ 
	mpPrev = mpNext = reinterpret_cast<zListLink*>(reinterpret_cast<size_t>(this) | 0x01); 
}

void zListLink::SetNext(zListLink* inItem) 
{ 	
	mpNext = reinterpret_cast<zListLink*>((reinterpret_cast<size_t>(mpNext) & 0x01) | (reinterpret_cast<size_t>(inItem) & ~0x01)); 
}

void zListLink::SetPrev(zListLink* inItem) 
{ 
	mpPrev = reinterpret_cast<zListLink*>((reinterpret_cast<size_t>(mpPrev) & 0x01) | (reinterpret_cast<size_t>(inItem) & ~0x01)); 
}

void zListLink::Remove()
{
	if (IsInList())
	{
		zenAssert(IsItem());
		Next()->SetPrev(mpPrev);
		Prev()->SetNext(mpNext);
		Reset();
	}
}

void zListLink::InsertBefore(zListLink& zenRestrict _NewLink)
{
	zenAssert(this != &_NewLink);
	_NewLink.Remove();
	_NewLink.SetPrev(mpPrev);
	_NewLink.SetNext(this);
	Prev()->SetNext(&_NewLink);
	SetPrev(&_NewLink);
}

void zListLink::InsertAfter(zListLink& zenRestrict _NewLink)
{
	zenAssert(this != &_NewLink);
	_NewLink.Remove();
	_NewLink.SetNext(mpNext);
	_NewLink.SetPrev(this);
	Next()->SetPrev(&_NewLink);
	SetNext(&_NewLink);
}

//=================================================================================================
// zListItem
//=================================================================================================
template <unsigned char TListCount,unsigned char TMultiIndex>
zListItem<TListCount, TMultiIndex>::zListItem()
{
	for (auto& Link : mLinks)
		Link.Reset();
}

template <unsigned char TListCount,unsigned char TMultiIndex>
zListItem<TListCount, TMultiIndex>::~zListItem()
{
	for (auto& Link : mLinks)
		Link.Remove();
}

//=================================================================================================
// zListBase
//=================================================================================================

zListBase::zListBase()
{
	mRoot.SetAsRoot();
}

zListBase::~zListBase()
{
	clear();
}

bool zListBase::empty()const
{
	return mRoot.Next()->IsItem() == false;
}

void zListBase::clear()
{
	while (!empty())
		mRoot.Next()->Remove();
}


zListBase::IteratorConst::IteratorConst(const zListLink* _pCurrent)
: mpCurrent(_pCurrent)
{
}

zListBase::IteratorConst::IteratorConst(const IteratorConst& _Copy)
: mpCurrent(_Copy.mpCurrent)
{
}

bool zListBase::IteratorConst::IsValid()const
{
	return (reinterpret_cast<zUInt>(mpCurrent) & 0x01) != 0;
}
zListBase::IteratorConst::operator bool()const
{
	return IsValid();
}

//=============================================================================

zListBase::Iterator::Iterator(zListLink* _pCurrent)
: mpCurrent(_pCurrent)
{
}

zListBase::Iterator::Iterator(const Iterator& _Copy)
: mpCurrent(_Copy.mpCurrent)
{
}

bool zListBase::Iterator::IsValid()const
{
	return mpCurrent != nullptr && mpCurrent->IsItem();
}
zListBase::Iterator::operator bool()const
{
	return IsValid();
}

bool zListBase::Iterator::operator==(const Iterator& _Cmp)const
{
	return mpCurrent == _Cmp.mpCurrent;
}

bool zListBase::Iterator::operator!=(const Iterator& _Cmp)const
{
	return !(*this == _Cmp);
}

bool zListBase::IteratorConst::operator==(const IteratorConst& _Cmp)const
{
	return mpCurrent == _Cmp.mpCurrent;
}

bool zListBase::IteratorConst::operator!=(const IteratorConst& _Cmp)const
{
	return !(*this == _Cmp);
}

//=================================================================================================
// zList
//=================================================================================================
template<class TItem, unsigned char TListIdx, class TLinkAccess>
zList<TItem,TListIdx,TLinkAccess>& zList<TItem,TListIdx,TLinkAccess>::push_front(TItem& _ItemAdd) 
{ 
	mRoot.InsertAfter(_ItemAdd.TLinkAccess::mLinks[TListIdx]); 
	return *this; 
} 

template<class TItem, unsigned char TListIdx, class TLinkAccess>
zList<TItem,TListIdx,TLinkAccess>& zList<TItem,TListIdx,TLinkAccess>::push_back(TItem& _ItemAdd) 
{ 
	mRoot.InsertBefore(_ItemAdd.TLinkAccess::mLinks[TListIdx]); 
	return *this; 
}

template<class TItem,unsigned char TListIdx,class TLinkAccess>
zList<TItem,TListIdx,TLinkAccess>& zList<TItem,TListIdx,TLinkAccess>::push_sort(TItem& _ItemAdd)
{
	Iterator it	= (empty() || (back() < _ItemAdd)) ? end() : begin();	
	while( it.IsValid() && *it < _ItemAdd)
		++it;
			
	if( it.IsValid() )
		insert_before(it, _ItemAdd);
	else
		push_back(_ItemAdd);

	return *this;	
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
zList<TItem,TListIdx,TLinkAccess>& zList<TItem,TListIdx,TLinkAccess>::insert(Iterator& _Pos,TItem& _ItemAdd) 
{ 
	zenAssert(_Pos.IsValid()); 
	zListBase::InsertAfter(*_Pos.mpCurrent,_ItemAdd.TLinkAccess::mLinks[TListIdx]); 
	return *this; 
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
zList<TItem,TListIdx,TLinkAccess>& zList<TItem,TListIdx,TLinkAccess>::insert(TItem& _ItemPos,TItem& _ItemAdd) 
{ 
	zenAssert(_ItemPos.TLinkAccess::mLinks[TListIdx].IsValid()); 
	zListBase::InsertAfter(_ItemPos.TLinkAccess::mLinks[TListIdx],_ItemAdd.TLinkAccess::mLinks[TListIdx]); 
	return *this; 
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
TItem& zList<TItem,TListIdx,TLinkAccess>::front()const 
{ 
	zenAssert(!empty()); 
	return *GetItemFromLink(*mRoot.Next()); 
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
TItem& zList<TItem,TListIdx,TLinkAccess>::back()const 
{ 
	zenAssert(!empty()); 
	return *GetItemFromLink(*mRoot.Prev()); 
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
void zList<TItem,TListIdx,TLinkAccess>::remove(TItem& _ItemRem)
{ 
	_ItemRem.TLinkAccess::mLinks[TListIdx].Remove(); 
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
typename zList<TItem,TListIdx,TLinkAccess>::Iterator zList<TItem,TListIdx,TLinkAccess>::begin() 
{ 
	return Iterator(mRoot.Next()); 
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
typename zList<TItem,TListIdx,TLinkAccess>::Iterator zList<TItem,TListIdx,TLinkAccess>::end() 
{ 
	return Iterator(&mRoot); 
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
typename zList<TItem,TListIdx,TLinkAccess>::IteratorConst zList<TItem,TListIdx,TLinkAccess>::cbegin()const 
{ 
	return IteratorConst(mRoot.Next()); 
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
typename zList<TItem,TListIdx,TLinkAccess>::IteratorConst zList<TItem,TListIdx,TLinkAccess>::cend()const 
{ 
	return IteratorConst(&mRoot); 
}

template<class TItem,unsigned char TListIdx,class TLinkAccess>
zList<TItem,TListIdx,TLinkAccess>& zList<TItem,TListIdx,TLinkAccess>::insert_after(Iterator& _Pos,TItem& _ItemAdd)
{
	return insert(pos, _ItemAdd);
}

template<class TItem,unsigned char TListIdx,class TLinkAccess>
zList<TItem,TListIdx,TLinkAccess>& zList<TItem,TListIdx,TLinkAccess>::insert_after(TItem& _ItemPos,TItem& _ItemAdd)
{
	return insert(_ItemPos, _ItemAdd);
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
zList<TItem,TListIdx,TLinkAccess>& zList<TItem,TListIdx,TLinkAccess>::insert_before(Iterator& _Pos,TItem& _ItemAdd) 
{ 
	zenAssert(_Pos.IsValid()); 
	_Pos.mpCurrent->InsertBefore(_ItemAdd.TLinkAccess::mLinks[TListIdx]); 
	return *this; 
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
zList<TItem,TListIdx,TLinkAccess>& zList<TItem,TListIdx,TLinkAccess>::insert_before(TItem& _ItemPos,TItem& _ItemAdd) 
{ 
	zenAssert(_ItemPos.TLinkAccess::mLinks[TListIdx].IsValid()); 
	zListBase::InsertBefore(_ItemPos.TLinkAccess::mLinks[TListIdx],_ItemAdd.TLinkAccess::mLinks[TListIdx]); 
	return *this; 
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
TItem* zList<TItem,TListIdx,TLinkAccess>::front_check()const 
{ 
	return empty() ? nullptr : GetItemFromLink(*mRoot.Next()); 
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
TItem* zList<TItem,TListIdx,TLinkAccess>::back_check()const 
{ 
	return empty() ? nullptr : GetItemFromLink(*mRoot.Prev()); 
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
TItem* zList<TItem,TListIdx,TLinkAccess>::GetNext(const TItem& Item)
{
	return Item.TLinkAccess::mLinks[TListIdx].IsInList() && Item.TLinkAccess::mLinks[TListIdx].Next()->IsItem() ? GetItemFromLink(*Item.TLinkAccess::mLinks[TListIdx].Next()) : nullptr;
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
TItem* zList<TItem,TListIdx,TLinkAccess>::GetPrev(const TItem& Item)
{
	return Item.TLinkAccess::mLinks[TListIdx].IsInList() && Item.TLinkAccess::mLinks[TListIdx].Prev()->IsItem() ? GetItemFromLink(*Item.TLinkAccess::mLinks[TListIdx].Prev()) : nullptr;
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
TItem* zList<TItem,TListIdx,TLinkAccess>::GetItemFromLink(const zListLink& _Link)
{
	const size_t LinkOffset = reinterpret_cast<size_t>(&(((TItem*)nullptr)->TLinkAccess::mLinks[TListIdx])); //! @todo 2 fix OffsetOf
	//const size_t LinkOffset	= zenOffsetOf(&TItem::mLinks) + sizeof(zListLinkTest)*TListIdx;
	size_t ItemAdr			= _Link.IsInList() ? (reinterpret_cast<size_t>(&_Link) - LinkOffset) : 0;
	return reinterpret_cast<TItem*>(ItemAdr);
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
bool zList<TItem,TListIdx,TLinkAccess>::IsInList(const TItem& Item)
{
	return Item.TLinkAccess::mLinks[TListIdx].IsInList();
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
TItem* zList<TItem,TListIdx,TLinkAccess>::pop_front() 
{ 
	TItem* pItem(nullptr);
	if( empty() == false )
	{
		pItem = GetItemFromLink(*mRoot.Next());
		mRoot.Next()->Remove();
	}
	return pItem;
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
TItem* zList<TItem,TListIdx,TLinkAccess>::pop_back() 
{ 
	TItem* pItem(nullptr);
	if (empty() == false)
	{
		pItem = GetItemFromLink(*mRoot.Prev());
		mRoot.Prev()->Remove();
	}
	return pItem;
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
zList<TItem,TListIdx,TLinkAccess>::Iterator::Iterator(const Iterator& _Copy)
: zListBase::Iterator(_Copy)
{
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
zList<TItem,TListIdx,TLinkAccess>::Iterator::Iterator(zListLink* _pCurrent)
: zListBase::Iterator(_pCurrent)
{
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
typename zList<TItem,TListIdx,TLinkAccess>::Iterator& zList<TItem,TListIdx,TLinkAccess>::Iterator::operator=(const Iterator& _Copy)
{
	mpCurrent	= _Copy.mpCurrent;
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
TItem* zList<TItem,TListIdx,TLinkAccess>::Iterator::Get()const
{
	return zList::GetItemFromLink( *mpCurrent );
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
typename zList<TItem,TListIdx,TLinkAccess>::Iterator& zList<TItem,TListIdx,TLinkAccess>::Iterator::operator++()
{
	mpCurrent = mpCurrent->Next();
	return *this;
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
typename zList<TItem,TListIdx,TLinkAccess>::Iterator& zList<TItem,TListIdx,TLinkAccess>::Iterator::operator--()
{
	mpCurrent = mpCurrent->Prev();
	return *this;
}

//=================================================================================================

template<class TItem, unsigned char TListIdx, class TLinkAccess>
zList<TItem,TListIdx,TLinkAccess>::IteratorConst::IteratorConst(const IteratorConst& _Copy)
: Iterator(_Copy)
{
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
zList<TItem,TListIdx,TLinkAccess>::IteratorConst::IteratorConst(const zListLink* _pCurrent)
: zListBase::IteratorConst(_pCurrent)
{
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
typename zList<TItem,TListIdx,TLinkAccess>::IteratorConst&	zList<TItem,TListIdx,TLinkAccess>::IteratorConst::operator=(const IteratorConst& _Copy)
{
	mpCurrent	= _Copy.mpCurrent;
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
const TItem* zList<TItem,TListIdx,TLinkAccess>::IteratorConst::Get()const
{	
	return zList::GetItemFromLink( *mpCurrent );
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
typename zList<TItem,TListIdx,TLinkAccess>::IteratorConst& zList<TItem,TListIdx,TLinkAccess>::IteratorConst::operator++()
{	
	mpCurrent = mpCurrent->Next();
	return *this;
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
typename zList<TItem,TListIdx,TLinkAccess>::IteratorConst& zList<TItem,TListIdx,TLinkAccess>::IteratorConst::operator--()
{
	mpCurrent = mpCurrent->Prev();
	return *this;
}

//=================================================================================================
template<class TItem, unsigned char TListIdx, class TLinkAccess>
zListRef<TItem,TListIdx,TLinkAccess>::~zListRef()
{
	clear();
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
void zListRef<TItem,TListIdx,TLinkAccess>::clear()
{	
	while (!empty())
	{
		auto& Item = front();
		Item.ReferenceRem();
		Super::remove(Item);
	}
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
zListRef<TItem,TListIdx,TLinkAccess>& zListRef<TItem,TListIdx,TLinkAccess>::push_front(TItem& inItem) 
{ 
	if (!IsInList(inItem))	
		inItem.ReferenceAdd(); 
	return Super::push_front(inItem); 
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
zListRef<TItem,TListIdx,TLinkAccess>& zListRef<TItem,TListIdx,TLinkAccess>::push_back(TItem& inItem) 
{ 
	if (!IsInList(inItem))	
		inItem.ReferenceAdd(); 
	Super::push_back(inItem); 
	return *this;
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
zListRef<TItem,TListIdx,TLinkAccess>& zListRef<TItem,TListIdx,TLinkAccess>::insert(Iterator& _Pos,TItem& _ItemAdd) 
{ 
	if(!IsInList(_ItemAdd))	
		inItem.ReferenceAdd(); 
	Super::insert(_Pos, _ItemAdd); 
	return *this;
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
zListRef<TItem,TListIdx,TLinkAccess>& zListRef<TItem,TListIdx,TLinkAccess>::insert(TItem& _ItemPos,TItem& _ItemAdd)
{
	if (!IsInList(_ItemAdd))
		inItem.ReferenceAdd();
	Super::insert(_ItemPos,_ItemAdd);
	return *this;
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
void zListRef<TItem,TListIdx,TLinkAccess>::remove(TItem& _ItemRem) 
{ 
	if (IsInList(inItem))
		inItem.ReferenceRem(); 
	Super::remove(_ItemRem); 
}
							 
template<class TItem, unsigned char TListIdx, class TLinkAccess>
zListRef<TItem,TListIdx,TLinkAccess>& zListRef<TItem,TListIdx,TLinkAccess>::insert_before(TItem& _Item) 
{
	if (!IsInList(inItem)) 
		inItem.ReferenceAdd(); 
	Super::push_front(inItem); 
	return *this;
}

template<class TItem, unsigned char TListIdx, class TLinkAccess>
TItem* zListRef<TItem,TListIdx,TLinkAccess>::pop_front() 
{	
	TItem* pItem=Super::pop_front(); 
	if (pItem) 
		pItem->ReferenceRem(); 
	return pItem; 
}

 template<class TItem, unsigned char TListIdx, class TLinkAccess>
TItem* zListRef<TItem,TListIdx,TLinkAccess>::pop_back() 
{ 
	TItem* pItem=Super::pop_back(); 
	if (pItem)
		pItem->ReferenceRem(); 
	return pItem; 
}

} } //namespace zen, Type
