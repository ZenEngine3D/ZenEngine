namespace zen { namespace zenType {

//=================================================================================================
// NODE SUBCLASS
//=================================================================================================

//-------------------------------------------------------------------------------------------------
//! @brief Get count of valid Slots in this node
//-------------------------------------------------------------------------------------------------
template<class TKey, class TValue, class TIndex, int TIndexBits>
zU32 zHamt<TKey, TValue, TIndex, TIndexBits>::Node::GetSlotCount() const 
{	
	return zenMath::BitsCount( mIndexUsed ); 
}

//-------------------------------------------------------------------------------------------------
//! @brief Check if a Node Slot is a leaf (contains value) or point to child node
//-------------------------------------------------------------------------------------------------
template<class TKey, class TValue, class TIndex, int TIndexBits>
bool zHamt<TKey, TValue, TIndex, TIndexBits>::Node::IsLeafSlot( zU32 _uSlotID ) const 
{ 
	return (mSlotLeaf & (TIndex(1)<<_uSlotID)) != 0; 
}

//-------------------------------------------------------------------------------------------------
//! @brief Check if Node Index is being used in this Node 
//-------------------------------------------------------------------------------------------------
template<class TKey, class TValue, class TIndex, int TIndexBits>
bool zHamt<TKey, TValue, TIndex, TIndexBits>::Node::IsUsedIndex( zU32 _uNodeIndex ) const 
{ 
	return (mIndexUsed & (TIndex(1)<<_uNodeIndex)) != 0; 
}

//-------------------------------------------------------------------------------------------------
//! @brief Get in which slot a Node Index is stored
//-------------------------------------------------------------------------------------------------
template<class TKey, class TValue, class TIndex, int TIndexBits>
zU32 zHamt<TKey, TValue, TIndex, TIndexBits>::Node::GetSlotID( zU32 _uNodeIndex ) const 
{ 
	// SlotID is amount of Active Index up to this NodeIndex
	return zenMath::BitsCount( static_cast<TIndex>(mIndexUsed & ((TIndex(1)<<_uNodeIndex)-1)) ); 
}

//-------------------------------------------------------------------------------------------------
//! @brief Return first used slotID (-1 if none)
//-------------------------------------------------------------------------------------------------
template<class TKey, class TValue, class TIndex, int TIndexBits>
int zHamt<TKey, TValue, TIndex, TIndexBits>::Node::GetFirstUsedSlotID()const 
{ 
	return mIndexUsed ? 0 : -1; 
}

//-------------------------------------------------------------------------------------------------
//! @brief Return last used slotID (-1 if none)
//-------------------------------------------------------------------------------------------------
template<class TKey, class TValue, class TIndex, int TIndexBits>
int zHamt<TKey, TValue, TIndex, TIndexBits>::Node::GetLastUsedSlotID()const 
{ 
	return static_cast<int>(zenMath::BitsCount( mIndexUsed ))-1; 
}

//=================================================================================================
// ITERATOR SUBCLASS
//=================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
zHamt<TKey, TValue, TIndex, TIndexBits>::Iterator::Iterator() 
: msDepth(-1)				
{}

template<class TKey, class TValue, class TIndex, int TIndexBits>
zHamt<TKey, TValue, TIndex, TIndexBits>::Iterator::Iterator(const Iterator& _Copy)			
{ 
	zenMem::Copy( (zU8*)this, (zU8*)&_Copy, sizeof(Iterator) ); 
}

template<class TKey, class TValue, class TIndex, int TIndexBits>
zHamt<TKey, TValue, TIndex, TIndexBits>::Iterator::Iterator(const zHamt& _Parent )		
{ 
	_Parent.GetFirst(*this); 
}

template<class TKey, class TValue, class TIndex, int TIndexBits>
void zHamt<TKey, TValue, TIndex, TIndexBits>::Iterator::operator=(const Iterator& _Copy)	
{ 
	if( this != &_Copy )
	{
		zenMem::Copy( (zU8*)this, (const zU8*)&_Copy, sizeof(Iterator) );	
	}
}

template<class TKey, class TValue, class TIndex, int TIndexBits>
bool zHamt<TKey, TValue, TIndex, TIndexBits>::Iterator::IsValid()							
{ 
	return msDepth >= 0; 
}

template<class TKey, class TValue, class TIndex, int TIndexBits>
TKey zHamt<TKey, TValue, TIndex, TIndexBits>::Iterator::GetKey()							
{ 
	zenAssert( IsValid() ); 
	return mpNodeTree[msDepth]->mpSlots[mpSlotID[msDepth]].Key; 
}

template<class TKey, class TValue, class TIndex, int TIndexBits>
TValue& zHamt<TKey, TValue, TIndex, TIndexBits>::Iterator::GetValue()						
{ 
	zenAssert( IsValid() ); 
	return mpNodeTree[msDepth]->mpSlots[mpSlotID[msDepth]].Value(); 
}

template<class TKey, class TValue, class TIndex, int TIndexBits>
void zHamt<TKey, TValue, TIndex, TIndexBits>::Iterator::operator++()
{
	zenAssert( IsValid() );									
	// Go to next SlotID
	// And go up hierarchy while SlotID bigger than current Node slot count
	++mpSlotID[msDepth];		
	while( mpSlotID[msDepth] >= static_cast<int>(mpNodeTree[msDepth]->GetSlotCount()) )
	{
		--msDepth;		
		if( msDepth < 0 )
			return;
		++mpSlotID[msDepth];
	}

	// Find first leaf of next node
	Node* pNode	= mpNodeTree[msDepth];
	while( !pNode->IsLeafSlot(mpSlotID[msDepth]) )
	{
		pNode					= mpNodeTree[msDepth]->mpSlots[mpSlotID[msDepth]].pChildNode;
		mpNodeTree[++msDepth]	= pNode;
		mpSlotID[msDepth]		= 0;		
	}
}

template<class TKey, class TValue, class TIndex, int TIndexBits>
void zHamt<TKey, TValue, TIndex, TIndexBits>::Iterator::operator--()
{
	zenAssert( IsValid() );
	// Go to previous SlotID
	// And go up hierarchy while SlotID smaller than 0
	--mpSlotID[msDepth];
	while( mpSlotID[msDepth] < 0 )
	{
		--msDepth;		
		if( msDepth < 0 )
			return;
		--mpSlotID[msDepth];
	}

	// Find last leaf of previous node
	Node* pNode	= mpNodeTree[msDepth];
	while( !pNode->IsLeafSlot(mpSlotID[msDepth]) )
	{
		pNode					= mpNodeTree[msDepth]->mpSlots[mpSlotID[msDepth]].pChildNode;
		mpNodeTree[++msDepth]	= pNode;
		mpSlotID[msDepth]		= pNode->GetSlotCount()-1;		
	}
}


//=================================================================================================
// HAMT CLASS
//=================================================================================================

//==================================================================================================
//! @details	Constructor
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
zHamt< TKey, TValue, TIndex, TIndexBits>::zHamt()
: mpRootNode(nullptr)
, muCount(0)
, mpDeleteItemCB(nullptr)
{ 
	zenStaticAssertMsg( kuKeyBits >= kuSlotCount,"Size of Index not big enough to contain '1<<TIndexBits' Index, check template definition" ); 
}

//==================================================================================================
//! @details	Initialize the Hamt immediately so it can be used
//--------------------------------------------------------------------------------------------------
//! @param		_uReservePool	- Amount of memory item pre-allocated for this container
//! @param		_pAllocator		- Which allocator to use for memory allocations
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
zHamt< TKey, TValue, TIndex, TIndexBits>::zHamt( zU32 _uReservePool )
: mpRootNode(nullptr)
, muCount(0)
, mpDeleteItemCB(nullptr)
{
	zenStaticAssertMsg( kuKeyBits >= kuSlotCount,"Size of Index not big enough to contain '1<<TIndexBits' Index, check template definition" ); 
	Init(_uReservePool);
}

//==================================================================================================
//! @details	Destructor
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
zHamt< TKey, TValue, TIndex, TIndexBits>::~zHamt()
{
	if( mpRootNode )
	{
		Clear();
		zenDelnullptr(mpRootNode);
	}			
}

//==================================================================================================
//! @details	Tell us if the hashtable has been initialized
//! @return		True if already initialized
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
bool zHamt< TKey, TValue, TIndex, TIndexBits>::IsInit()const
{
	return mpRootNode!=nullptr;
}

//==================================================================================================
//! @brief		Remove every entry from the zHamt.
//! @details	The root node will be recreated empty	
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
void zHamt< TKey, TValue, TIndex, TIndexBits>::Clear()
{
	ClearNode(mpRootNode);
	for(zU32 uPoolIndex=0; uPoolIndex<kuPoolCount; ++uPoolIndex)
		mPools[uPoolIndex].Clear();
	mpRootNode	= CreateEmptyNode(0);
	muCount		= 0;
}

//==================================================================================================
//! @details	The root node will be created empty	
//--------------------------------------------------------------------------------------------------
//! @param		_uReservePool	- Amount of Item to pre-allocate for faster allocations
//! @param		_pAllocator		- Memory source used to initialize each Pool zAllocator
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
void zHamt< TKey, TValue, TIndex, TIndexBits>::Init( zU32 _uReservePool )
{	
	zenAssertMsg(!IsInit(),"zHamt already initialized");			
	zI32 iPoolItemLeft(static_cast<zI32>(_uReservePool)-1);
	zI32 iPoolItemMin	= zenMath::Max<zI32>((iPoolItemLeft / 3) / kuSlotCount, 1);	//< @Note: A 1/3 of PoolItemCount get split evenly between each Pool. Remaining assigned with 1/2 less every time. Need metric to adjust for optimal value
	iPoolItemLeft		= zenMath::Max<zI32>(iPoolItemLeft-iPoolItemMin*kuSlotCount, 0);
	mPools[0].Init("HamtSmallPool", sizeof(Node), 1, 1 );
	for(zU32 uPoolIndex=1; uPoolIndex<=kuSlotCount; ++uPoolIndex)
	{
		iPoolItemLeft = iPoolItemLeft / 2;
		mPools[uPoolIndex].Init("HamtSmallPool", sizeof(Node) + sizeof(Node::Slot)*uPoolIndex, iPoolItemLeft+iPoolItemMin, iPoolItemMin );
	}
	mpRootNode = CreateEmptyNode(0);
}

//==================================================================================================
//! @details	Find out if Key entry is in the hamt
//--------------------------------------------------------------------------------------------------
//! @param		_Key	- Key entry where to store the value
//! @return 			- True if we found the item
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
bool zHamt< TKey, TValue, TIndex, TIndexBits>::Exist(const TKey _Key)const
{
	const Node**	ppParentNode;
	zU32			uSlotID(0), uNodeIndex(0), uDepth(0);
	return GetNode( _Key, ppParentNode, uNodeIndex, uSlotID, uDepth );
}
		
//==================================================================================================
//! @details	Set value return when no value found
//--------------------------------------------------------------------------------------------------
//! @param		_Value		- Value used for default
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
void zHamt< TKey, TValue, TIndex, TIndexBits>::SetDefaultValue( const TValue& _Value )
{
	mDefault = _Value;
}

//==================================================================================================
//! @details	Set all value in table, to specified value
//--------------------------------------------------------------------------------------------------
//! @param		_Value		- Value to set all elements to
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
void zHamt< TKey, TValue, TIndex, TIndexBits>::SetAll( const TValue& _Value )
{
	Iterator it;
	for(GetFirst(it); it.IsValid(); ++it)
		it.GetValue() = _Value;
}

//==================================================================================================
//! @details	Assign value at a certain key entry
//--------------------------------------------------------------------------------------------------
//! @param		_Key	- Key entry where to store the value
//! @param		_Value	- Value to assign this key entry
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
void zHamt< TKey, TValue, TIndex, TIndexBits>::Set(const TKey _Key, const TValue& _Value)
{
	Node**	ppParentNode;
	zU32		uSlotID(0), uNodeIndex(0), uDepth(0);
	bool bFound = GetNode( _Key, ppParentNode, uNodeIndex, uSlotID, uDepth );
	muCount		+= bFound ? 0 : 1;
	SetSlotValue( _Key, _Value, ppParentNode, uNodeIndex, uSlotID, uDepth );
}
		
//==================================================================================================
//! @details	If key entry already exist, will replace the value and return us the previous value	
//--------------------------------------------------------------------------------------------------
//! @param		_Key			- Key entry where to store the value
//! @param		_Value			- Value to assign this key entry
//! @param		_OldValueOut	- Value to assign this key entry		
//! @param						- True if there was previously a value that we just replaced 
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
bool zHamt< TKey, TValue, TIndex, TIndexBits>::SetReplace(const TKey _Key, const TValue& _Value, TValue& _OldValueOut)
{
	Node**	ppParentNode;
	zU32		uSlotID(0), uNodeIndex(0), uDepth(0);
	bool bFound = GetNode( _Key, ppParentNode, uNodeIndex, uSlotID, uDepth );
	muCount		+= bFound ? 0 : 1;
	if( bFound )
	{
		_OldValueOut = (*ppParentNode)->mpSlots[uSlotID].Value();

	}
	SetSlotValue( _Key, _Value, ppParentNode, uNodeIndex, uSlotID, uDepth );
	return bFound;
}
		
//==================================================================================================
//! @details	Read only access.
//--------------------------------------------------------------------------------------------------
//! @param		_Key	- Key entry where to store the value
//! @return				- Value associated with the Key
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
const TValue& zHamt< TKey, TValue, TIndex, TIndexBits>::Get( const TKey _Key ) const
{
	zenAssertMsg(IsInit(),"zHamt isn't initialized");
	const Node** ppParentNode;
	zU32 uSlotID, uNodeIndex, uDepth;
	bool bFound = GetNode( _Key, ppParentNode, uNodeIndex, uSlotID, uDepth );
	if( !bFound )	return mDefault;									//Not found, return default value
	else			return (*ppParentNode)->mpSlots[uSlotID].Value();	//Return found entry
}
		
//==================================================================================================
//! @details	Do a Exist() and Get() in same call
//--------------------------------------------------------------------------------------------------
//! @param		_Key		- Key entry where to store the value
//! @param[out]	_ValueOut	- Value stored at that key
//! @return					- True if value found
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
bool zHamt< TKey, TValue, TIndex, TIndexBits>::Get(const TKey _Key, TValue& _ValueOut) const
{
	zenAssertMsg(IsInit(),"zHamt isn't initialized");
	const Node**	ppParentNode;
	zU32		uSlotID, uNodeIndex, uDepth;
	bool	bFound	= GetNode( _Key, ppParentNode, uNodeIndex, uSlotID, uDepth );
	_ValueOut		= bFound ? (*ppParentNode)->mpSlots[uSlotID].Value() : mDefault;
	return bFound;
}
/*
//==================================================================================================
//! @details	Do a Exist() and Get() in same call
//--------------------------------------------------------------------------------------------------
//! @param		_Key		- Key entry where to store the value
//! @param[out]	_pValueOut	- Value stored at that key
//! @return					- True if value found
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
bool zHamt< TKey, TValue, TIndex, TIndexBits>::Get(const TKey _Key, TValue*& _pValueOut) const
{
	zenAssertMsg(IsInit(),"zHamt isn't initialized");
	const Node**	ppParentNode;
	zU32		uSlotID, uNodeIndex, uDepth;
	bool	bFound	= GetNode( _Key, ppParentNode, uNodeIndex, uSlotID, uDepth );
	_pValueOut		= bFound ? &(*ppParentNode)->mpSlots[uSlotID].Value() : nullptr;
	return bFound;
}
*/
//==================================================================================================
//! @details	Read only access (won't add entry when none found).
//--------------------------------------------------------------------------------------------------
//! @param		_Key - Key reference used to find data
//! @return		Value stored at that key entry 
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
const TValue& zHamt< TKey, TValue, TIndex, TIndexBits>::operator[](const TKey _Key)const
{		
	return Get(_Key);
}

//==================================================================================================
//! @details	Used for entry assignments. If item doesn't already exist, insert a new one
//--------------------------------------------------------------------------------------------------
//! @param		_Key		- Key reference used to find data
//! @return		Value stored at that key entry
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
TValue& zHamt< TKey, TValue, TIndex, TIndexBits>::GetAdd(const TKey _Key)
{			
	Node**	ppParentNode;
	zU32	uSlotID, uNodeIndex, uDepth;
	bool	bFound = GetNode( _Key, ppParentNode, uNodeIndex, uSlotID, uDepth );
	muCount			+= bFound ? 0 : 1;
	if( !bFound )	return *SetSlotValue( _Key, mDefault, ppParentNode, uNodeIndex, uSlotID, uDepth );	//Not found, create new entry with default value
	else			return (*ppParentNode)->mpSlots[uSlotID].Value();									//Return found entry
}

//==================================================================================================
//! @details	Remove the value stored at that key entry
//--------------------------------------------------------------------------------------------------
//! @param		_Key - Key reference used to find data
//! @return		Data stored at that key entry
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
bool zHamt< TKey, TValue, TIndex, TIndexBits>::Unset(const TKey _Key)
{		
	zenAssertMsg(IsInit(),"zHamt isn't initialized");
	Node**		ppNodeTree[kuTreeMaxDepth];
	zU32		uSlotID[kuTreeMaxDepth];
	zU32		uNodeIndex[kuTreeMaxDepth];
	zU32		uDepth(0);			
	Node*		pNode = mpRootNode;
	//-------------------------------------------------------
	// Find hierarchy all the way down to Key entry
	//-------------------------------------------------------
	uNodeIndex[uDepth]	= GetNodeIndex( _Key, uDepth );
	uSlotID[uDepth]		= pNode->GetSlotID(uNodeIndex[uDepth]);
	ppNodeTree[uDepth]	= &mpRootNode;
	while( pNode->IsUsedIndex(uNodeIndex[uDepth]) && !pNode->IsLeafSlot(uSlotID[uDepth]) )
	{								
		ppNodeTree[uDepth+1]	= &pNode->mpSlots[uSlotID[uDepth]].pChildNode;
		pNode					= *ppNodeTree[++uDepth];				
		uNodeIndex[uDepth]		= GetNodeIndex( _Key, uDepth );
		uSlotID[uDepth]			= pNode->GetSlotID(uNodeIndex[uDepth]);				
	}
				
	//-------------------------------------------------------
	// Remove the entry associated with key, and all empty parent
	//-------------------------------------------------------
	bool bFound = pNode->mpSlots[uSlotID[uDepth]].Key == _Key;
	muCount		+= bFound ? -1 : 0;
	pNode		= bFound ? pNode : nullptr;
	if( bFound && mpDeleteItemCB)
		mpDeleteItemCB(*this, pNode->mpSlots[uSlotID[uDepth]].Value());

	while( pNode )
	{
		zU32 uNewSlotCount = pNode->GetSlotCount()-1;
		// No slot left in child node, after we remove the entry, update parent to refer to it
		if( uNewSlotCount == 0 && uDepth>0 )
		{
			zenDelnullptr(pNode);
			pNode = *ppNodeTree[--uDepth];
		}
		// Child node with 1 less slot, 
		else
		{
			Node* pNewNode			= CreateEmptyNode( uNewSlotCount );					
			pNewNode->mIndexUsed	= pNode->mIndexUsed & ~(TIndex(1)<<uNodeIndex[uDepth]);
			//Copy previous elements to new node
			for(zU32 i=0; i<uNewSlotCount; ++i)
			{
				zU32 uOldSlotID			= (i >= uSlotID[uDepth]) ? i+1 : i;
				pNewNode->mpSlots[i]	= pNode->mpSlots[uOldSlotID];
				pNewNode->mSlotLeaf		|= TIndex(pNode->IsLeafSlot(uOldSlotID)) << i;
			}
			*ppNodeTree[uDepth] = pNewNode;
			zenDelnullptr(pNode);
			pNode = nullptr; //Parent doesn't need updating
		}
	}
	return bFound;
}

//==================================================================================================
//! @details	Initialize iteator to first element of the hamt
//--------------------------------------------------------------------------------------------------
//! @param[out]	_It - Iterator pointing to 1st element
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
void zHamt< TKey, TValue, TIndex, TIndexBits>::GetFirst(Iterator& _It) const
{
	if( mpRootNode->GetSlotCount() == 0 )
	{
		_It.msDepth = -1;
		return;
	}
	Node* pNode(mpRootNode);	
	_It.mpNodeTree[0]	= pNode;
	_It.mpSlotID[0]		= pNode->GetFirstUsedSlotID();	
	_It.msDepth			= 0;
	while( !pNode->IsLeafSlot(0) )
	{
		pNode							= pNode->mpSlots[0].pChildNode;		
		_It.mpNodeTree[++_It.msDepth]	= pNode;
		_It.mpSlotID[_It.msDepth]		= pNode->GetFirstUsedSlotID();
	}
}

//==================================================================================================
//! @details	Initialize iterator to last element of the hamt
//--------------------------------------------------------------------------------------------------
//! @param[out]	_It - Iterator pointing to last element
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
void zHamt< TKey, TValue, TIndex, TIndexBits>::GetLast(Iterator& _It) const
{	
	if( mpRootNode->GetSlotCount() == 0 )
	{
		_It.msDepth = -1;
		return;
	}
	Node* pNode(mpRootNode);
	_It.mpNodeTree[0]	= pNode;
	_It.mpSlotID[0]		= pNode->GetLastUsedSlotID();	
	_It.msDepth			= 0;
	while( !pNode->IsLeafSlot( _It.mpSlotID[_It.msDepth] ) )
	{
		pNode							= pNode->mpSlots[ _It.mpSlotID[_It.msDepth] ].pChildNode;		
		_It.mpNodeTree[++_It.msDepth]	= pNode;
		_It.mpSlotID[_It.msDepth]		= pNode->GetLastUsedSlotID();
	}
}

//==================================================================================================
//! @details	
//--------------------------------------------------------------------------------------------------
//! @return First Unused key
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
TKey zHamt< TKey, TValue, TIndex, TIndexBits>::GetFirstUnusedKey()const
{
	Iterator It;
	GetFirst(It);
	TIndex currentKey(0);
	while( currentKey == It.GetKey() )
	{
		currentKey += 1;
		++It;
	}
	
	return currentKey;	
}

//==================================================================================================
//! @details	Print structure of the tree
//--------------------------------------------------------------------------------------------------
//! @param		_First	- First Key entry to print
//! @param		_Last	- Last Key entry to print
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
void zHamt< TKey, TValue, TIndex, TIndexBits>::DebugPrint(const TKey _First, TKey _Last) const
{
#if ZEN_DEBUGINFO_ON
#define	kuSpacePerLevel	14
	char		zSpaces[kuTreeMaxDepth*kuSpacePerLevel+1];
	zI8			iSlotIDPrev[ kuTreeMaxDepth ];
	zenMem::Set(zSpaces, ' ', sizeof(zSpaces));
	zenMem::Set(iSlotIDPrev, -1, sizeof(iSlotIDPrev) );
	Iterator it;
	for(GetFirst(it); it.IsValid() && it.GetKey() <= _Last;++it)
	{
		if( it.GetKey() >= _First )
		{
			zI8 sSharedParentDepth(0);
			zI8 sCurrentDepth(0);
			while( iSlotIDPrev[sSharedParentDepth] == it.mpSlotID[sSharedParentDepth] )
				++sSharedParentDepth;
			zenMem::Copy(iSlotIDPrev, it.mpSlotID, sizeof(iSlotIDPrev) );

			TKey Key = it.GetKey();
			zSpaces[kuSpacePerLevel*sSharedParentDepth] = 0; 
			zbLog::Printf(zbLog::keLog_DebugInfo, "\n%s", zSpaces);
			zSpaces[kuSpacePerLevel*sSharedParentDepth] = ' '; 
			sCurrentDepth = sSharedParentDepth;
			while( sCurrentDepth != it.msDepth )
				zbLog::Printf(zbLog::keLog_DebugInfo, "<%02i>----------", GetNodeIndex(Key, sCurrentDepth++));
			zbLog::Printf(zbLog::keLog_DebugInfo, "<%02i> %08X", GetNodeIndex(Key, sCurrentDepth), Key);
		}
	}
#endif
}

//==================================================================================================
//! @details	Return the amount of memory used by this structure		
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
size_t zHamt< TKey, TValue, TIndex, TIndexBits>::GetMemoryFootprint()
{
	size_t uSizeTotal(0);
	for(int i=0; i<kuPoolCount; ++i)
		uSizeTotal += mPools[i].GetTotalAllocSize();
	return uSizeTotal;
}

//==================================================================================================
//! @details	Copy the content of another Hamt
//--------------------------------------------------------------------------------------------------
//! @param		_Copy	- Hamt to copy
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
const zHamt< TKey, TValue, TIndex, TIndexBits>& zHamt< TKey, TValue, TIndex, TIndexBits>::operator=(const zHamt& _Copy)
{				
	if( IsInit() )
	{
		// Clear previous content
		Clear();
		zenDelnullptr(mpRootNode);
		// Size pools to have just enough space for all allocated items per pool
		for(zU32 poolIdx=0; poolIdx<kuPoolCount; ++poolIdx)
			mPools[poolIdx].MemoryIncrease( _Copy.mPools[poolIdx].GetTotalAllocCount() );
	}
	else
	{
		// Size pools to have just enough space for all allocated items per pool
		for(zU32 poolIdx=0; poolIdx<=kuSlotCount; ++poolIdx)
			mPools[poolIdx].Init("HamtSmallPool", _Copy.mPools[poolIdx].GetItemSize(), _Copy.mPools[poolIdx].GetTotalAllocCount(), _Copy.mPools[poolIdx].GetIncreaseCount() );
	}
			
	// Copy each nodes 			
	mpRootNode	= CreateNodeCopy( _Copy.mpRootNode );
	muCount		= _Copy.muCount;
	mDefault	= _Copy.mDefault;
	return *this;
}

//==================================================================================================
//! @details	Copy the content key/value of this hamt to carrays
//--------------------------------------------------------------------------------------------------
//! @param	_aKey	- Array to receive hashmap keys 
//! @param	_aValue	- Array to receive hashmap values 
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
void zHamt< TKey, TValue, TIndex, TIndexBits>::Export( zArrayBase<TKey>& _aKey, zArrayBase<TValue>& _aValue ) const
{
	_aKey.SetCount(muCount);
	_aValue.SetCount(muCount);
	TKey* pKeyCur(_aKey.First());
	TValue* pValCur(_aValue.First());
	Iterator it;
	for(GetFirst(it); it.IsValid(); ++it)
	{
		*pKeyCur++ = it.GetKey();
		*pValCur++ = it.GetValue();
	}
}
		
//==================================================================================================
//! @details	Copy the content key/value from arrays to this hamt
//--------------------------------------------------------------------------------------------------
//! @param	_aKey	- Array to retrieve hashmap keys 
//! @param	_aValue	- Array to retrieve hashmap values 
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
void zHamt< TKey, TValue, TIndex, TIndexBits>::Import( const zArrayBase<TKey>& _aKey, const zArrayBase<TValue>& _aValue ) 
{			
	zenAssertMsg( _aKey.Count() == _aValue.Count(), "Importing mismatching keys/values pair");
	const TValue* pValCur(_aValue.First());			
	const TKey* pKeyCur(_aKey.First());
	const TKey* pKeyLast(_aKey.Last());
	if( pKeyCur != nullptr )
	{
		while( pKeyCur <= pKeyLast )
			Set(*pKeyCur++, *pValCur++);
	}
}

//==================================================================================================
//! @details	Number of element (leaf node) stored in hashmap
//--------------------------------------------------------------------------------------------------
//! @return		- Item count
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
zU32 zHamt< TKey, TValue, TIndex, TIndexBits>::Count() const
{
	return muCount;
}

//==================================================================================================
//! @details	Create a copy of a node
//--------------------------------------------------------------------------------------------------
//! @param		_pNodeCopy	- Node to copy
//! @return		Created node copy
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
typename zHamt<TKey, TValue, TIndex, TIndexBits>::Node* zHamt<TKey, TValue, TIndex, TIndexBits>::CreateNodeCopy(typename const zHamt::Node* _pNodeCopy )
{
	zU32 uSlotCount			= _pNodeCopy->GetSlotCount();
	Node* pNewNode			= CreateEmptyNode( uSlotCount );
	pNewNode->mIndexUsed	= _pNodeCopy->mIndexUsed;
	pNewNode->mSlotLeaf		= _pNodeCopy->mSlotLeaf;
	for(zU32 uSlotIdx=0; uSlotIdx<uSlotCount; ++uSlotIdx)
	{
		if( _pNodeCopy->IsLeafSlot(uSlotIdx) )	pNewNode->mpSlots[uSlotIdx]				= _pNodeCopy->mpSlots[uSlotIdx];
		else									pNewNode->mpSlots[uSlotIdx].pChildNode	= CreateNodeCopy( _pNodeCopy->mpSlots[uSlotIdx].pChildNode );				
	}
	return pNewNode;
}

//==================================================================================================
//! @details	Each tree depth represent a sub section of 'TIndexBits' bits of the key
//--------------------------------------------------------------------------------------------------
//! @param		auKey	- Key we want to extract node index from
//! @param		auDepth - Depth at which the node is
//! @return		The NodeIndex of this key, at depth 'auDepth'
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
zU32 zHamt< TKey, TValue, TIndex, TIndexBits>::GetNodeIndex( const TKey _uKey, zU32 _uDepth ) const
{
	zU32 iShift	= (kuKeyBits-TIndexBits)-(_uDepth*TIndexBits);
	iShift		= iShift > 0 ? iShift : 0;
	return (_uKey>>iShift)&kuIndexMask;
}

//==================================================================================================
//! @details	Node(and slot they contain) are allocated from a preallocated 
//!				memory in variously sized pool(1 pool per Node Slot Count)
//--------------------------------------------------------------------------------------------------
//! @param		_uSlotCount - Number of slot wanted in the node
//! @return		Created node
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
typename zHamt<TKey, TValue, TIndex, TIndexBits>::Node* zHamt< TKey, TValue, TIndex, TIndexBits>::CreateEmptyNode(zU32 _uSlotCount)
{
	Node* pNewNode			= zenNew(&mPools[_uSlotCount])Node;
	pNewNode->mIndexUsed	= 0;
	pNewNode->mSlotLeaf		= 0;
	pNewNode->mpSlots		= (Node::Slot*)(((zU8*)pNewNode)+sizeof(Node));				
	zenMem::Set(pNewNode->mpSlots, 0, _uSlotCount*sizeof(Node::Slot));
	return pNewNode;
}

//==================================================================================================
//! @details	Used internally to find node containing wanted data. 
//!				Constant variation, used to read value without changing them.
//--------------------------------------------------------------------------------------------------
//! @param[in]	_Key			- Key entry wanted
//! @param[out]	_pParentSlot	- Return pointer to slot containing info on wanted node		
//! @param[out]	_uNodeIndex		- Return the Index of the key, at that node location
//! @param[out]	_uSlotID		- Return the SlotId in which entry is found in that node
//! @param[out]	_uDepth			- Return the depth of the found node
//! @return						- True if the slot contain the key entry
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
bool zHamt< TKey, TValue, TIndex, TIndexBits>::GetNode(TKey _Key, const Node**& _pParentSlot, zU32& _uNodeIndex, zU32& _uSlotID, zU32& _uDepth) const
{
	zenAssertMsg(mpRootNode!=nullptr,"zHamt isn't initialized");
	const Node* pNode	= mpRootNode;
	_pParentSlot		= (const Node**)&mpRootNode;
	_uDepth				= 0;
	_uNodeIndex			= GetNodeIndex(_Key, _uDepth);
	_uSlotID			= pNode->GetSlotID(_uNodeIndex);
	while( pNode->IsUsedIndex(_uNodeIndex) && !pNode->IsLeafSlot(_uSlotID) )
	{
		_pParentSlot	= (const Node**)&pNode->mpSlots[_uSlotID].pChildNode;
		pNode			= *_pParentSlot;
		_uNodeIndex		= GetNodeIndex(_Key, ++_uDepth );
		_uSlotID		= pNode->GetSlotID(_uNodeIndex);
	}		
	return pNode->IsUsedIndex(_uNodeIndex) && pNode->mpSlots[_uSlotID].Key==_Key;
}
		
//==================================================================================================
//! @details	Used internally to find node containing wanted data
//!				Non constant variation, used to insert new items.
//--------------------------------------------------------------------------------------------------
//! @param[in]	_Key			- Key entry wanted
//! @param[out]	_pParentSlot	- Return pointer to slot containing info on wanted node		
//! @param[out]	_uNodeIndex		- Return the Index of the key, at that node location
//! @param[out]	_uSlotID		- Return the SlotId in which entry is found in that node
//! @param[out]	_uDepth			- Return the depth of the found node
//! @return						- True if the slot contain the key entry
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
bool zHamt< TKey, TValue, TIndex, TIndexBits>::GetNode(TKey _Key, Node**& _pParentSlot, zU32& _uNodeIndex, zU32& _uSlotID, zU32& _uDepth)
{
	Node* pNode		= mpRootNode;
	_pParentSlot	= &mpRootNode;		
	_uDepth			= 0;
	_uNodeIndex		= GetNodeIndex(_Key, _uDepth);
	_uSlotID		= pNode->GetSlotID(_uNodeIndex);
	while( pNode->IsUsedIndex(_uNodeIndex) && !pNode->IsLeafSlot(_uSlotID) )
	{
		_pParentSlot	= (Node**)&pNode->mpSlots[_uSlotID].pChildNode;
		pNode			= *_pParentSlot;
		_uNodeIndex		= GetNodeIndex(_Key, ++_uDepth );
		_uSlotID		= pNode->GetSlotID(_uNodeIndex);
	}		
	return pNode->IsUsedIndex(_uNodeIndex) && pNode->mpSlots[_uSlotID].Key==_Key;
}

//==================================================================================================
//! @details	Remove every child node of this node
//--------------------------------------------------------------------------------------------------
//! @param[in]	_pNode- Node from which child nodes should be removed.
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
void zHamt< TKey, TValue, TIndex, TIndexBits>::ClearNode( Node* _pNode )
{	
	for(zU32 slot=0, count(_pNode->GetSlotCount()); slot<count; ++slot)
	{
		if( !_pNode->IsLeafSlot(slot) )
			ClearNode( _pNode->mpSlots[slot].pChildNode );
		else if(mpDeleteItemCB)
			mpDeleteItemCB(*this, _pNode->mpSlots[slot].Value());
	}
	zenDelnullptr( _pNode );
}

//==================================================================================================
//! @details	Will create new entry if needed
//--------------------------------------------------------------------------------------------------
//! @param		_Key	- Key entry where to store the value	
//==================================================================================================
template<class TKey, class TValue, class TIndex, int TIndexBits>
TValue* zHamt< TKey, TValue, TIndex, TIndexBits>::SetSlotValue(TKey _Key, const TValue& _Value, Node** _ppParentNode, zU32 _uNodeIndex, zU32 _uSlotID, zU32 _uDepth)
{			
	Node*	pNode(*_ppParentNode);
	Node*	pNewNode(nullptr);
	TValue*	pValue(nullptr);
						
	//------------------------------------------------------------------
	// Slot available in Node, resize array and add element
	if( pNode->IsUsedIndex(_uNodeIndex) == false )
	{
		zU32 uSlotCount						= pNode->GetSlotCount();
		pNewNode							= CreateEmptyNode( uSlotCount + 1 );
		pNewNode->mIndexUsed				= pNode->mIndexUsed | (TIndex(1)<<_uNodeIndex);				
		_uSlotID							= pNewNode->GetSlotID(_uNodeIndex);		//Must be updated with new array size
		pNewNode->mSlotLeaf					= TIndex(1)<<_uSlotID;				
		pNewNode->mpSlots[_uSlotID].Key		= _Key;
		pNewNode->mpSlots[_uSlotID].Value()	= _Value;				
		pValue								= &pNewNode->mpSlots[_uSlotID].Value();
		//Copy previous elements to new node
		for(zU32 i=0; i<uSlotCount; ++i)
		{
			zU32 uNewSlotID					= (i >= _uSlotID) ? i+1 : i;
			pNewNode->mpSlots[uNewSlotID]	= pNode->mpSlots[i];
			pNewNode->mSlotLeaf				|= TIndex(pNode->IsLeafSlot(i)) << uNewSlotID;
		}
		zenDelnullptr(pNode);
		*_ppParentNode	= pNewNode;				
	}
	//------------------------------------------------------------------
	// Slot already taken by same key, replace data
	else if( pNode->mpSlots[_uSlotID].Key==_Key )
	{
		if(mpDeleteItemCB)
			mpDeleteItemCB(*this, pNode->mpSlots[_uSlotID].Value());
		pNode->mpSlots[_uSlotID].Value() = _Value;
		pValue = &pNode->mpSlots[_uSlotID].Value();				
	}
	//------------------------------------------------------------------
	// Slot taken by another key, create child node to keep both
	else
	{
		Node::Slot prevSlot	= pNode->mpSlots[_uSlotID];
		zU32 uNodeIndexOld	= GetNodeIndex(prevSlot.Key, ++_uDepth);
		zU32 uNodeIndexNew	= GetNodeIndex(_Key, _uDepth);
		pNode->mSlotLeaf	= pNode->mSlotLeaf & ~(TIndex(1)<<_uSlotID);	//Slot is now pointing to child node, not a leaf anymore

		// While Node index of 2 node match, create child node
		while( uNodeIndexOld == uNodeIndexNew )
		{
			pNewNode								= CreateEmptyNode(1);			
			pNode->mpSlots[_uSlotID].pChildNode		= pNewNode;					
			pNode									= pNewNode;
			pNode->mIndexUsed						|= TIndex(1)<<uNodeIndexOld;
			_uSlotID								= 0;					
			uNodeIndexOld							= GetNodeIndex(prevSlot.Key, ++_uDepth);
			uNodeIndexNew							= GetNodeIndex(_Key, _uDepth);
		}
		
		// Store the value and displaced value into this node
		pNewNode													= CreateEmptyNode(2);
		pNewNode->mIndexUsed										= (TIndex(1)<<uNodeIndexOld) | (TIndex(1)<<uNodeIndexNew);
		pNewNode->mSlotLeaf											= (TIndex(1)<<0) | (TIndex(1)<<1);
		pNewNode->mpSlots[(uNodeIndexOld > uNodeIndexNew)].Key		= prevSlot.Key;
		pNewNode->mpSlots[(uNodeIndexOld > uNodeIndexNew)].Value()	= prevSlot.Value();
		pNewNode->mpSlots[(uNodeIndexNew > uNodeIndexOld)].Key		= _Key;
		pNewNode->mpSlots[(uNodeIndexNew > uNodeIndexOld)].Value()	= _Value;
		pValue														= &pNewNode->mpSlots[(uNodeIndexNew > uNodeIndexOld)].Value();
		pNode->mpSlots[_uSlotID].pChildNode							= pNewNode;
	}	
	return pValue;
}

} } //namespace zen, Type
