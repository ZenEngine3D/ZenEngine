#pragma once
#ifndef __zenApi_Base_Type_HamtSmall_h__
#define __zenApi_Base_Type_HamtSmall_h__

namespace zen { namespace zenType 
{
	//=================================================================================================
	//! @class		zHamt
	//-------------------------------------------------------------------------------------------------
	//! @brief		Classes for managing a Hash Array Map Trie.
	//! @details	Inspired by article here : http://altdevblogaday.com/2011/03/22/ideal-hash-tries-an-implementation-in-c/ \n
	//!				and wiki here : http://en.wikipedia.org/wiki/Hash_array_mapped_trie \n
	//!				This class put emphasis on being space efficiency. Each node are of exactly the right size to contain
	//!				the amount of slots it has. The slowdown occurs when adding/removing an entry, we need to recopy each slot 
	//!				into a new node. The copy overhead shouldn't be critical though.
	//!				-# Node : Contains 0 to 'kuSlotCount' slots
	//!				-# Slot : Key + Value of element or Key + Pointer to child Node
	//!				-# Pool : Each node is created from a memory pool. There's 'kuSlotCount+1' memory pool (1 per possible slot count in a node)
	//! @todo Optim:		Specify pool size increase	
	//! @todo Clean:		Support object better, not just pointer. At the moment, need to set a callback when removing object, to call custom destructor, not very programmer error safe
	//! @tparam TKey		Datatype of Keys used to find/retrieve entries
	//! @tparam TValue		Datatype of Values stored
	//! @tparam TIndex		Datatype bitfield used to store info of each slot in a node. Must be big enough to contain 1<<TIndexBits bits.
	//! @tparam TIndexBits	Each node level refer to subsection of TIndexBits bits of the Key entry. More bits means more slot per node, and less tree depth. 
	//!						For exemple TIndexBits=5 means 32 slot available per node
	//=================================================================================================
	template<class TKey, class TValue, class TIndex, int TIndexBits>
	class zHamt
	{
	ZENClassDeclareNoParent(zHamt)
	protected:
		//=================================================================================================
		//! @brief		Internal Hamt constants
		//=================================================================================================
		enum eConstant
		{
			kuKeyBits		= sizeof(TKey)*8,						//!< Bits count of the key used to store entries	
			kuTreeMaxDepth	= (kuKeyBits+TIndexBits-1)/TIndexBits,	//!< Maximum depth of tree (based on KeyBits/IndexBits) rounded up
			kuSlotCount		= 1<<TIndexBits,
			kuPoolCount		= kuSlotCount+1,
			kuIndexMask		= kuSlotCount-1,
		};
		
		//=================================================================================================
		//! @class		Node
		//-------------------------------------------------------------------------------------------------
		//! @brief		Contain subset if data stored in the Tree. 
		//! @detail		Used internally bu Hamt
		//=================================================================================================
		struct Node
		{
			struct Slot	{	TKey Key; union { Node* pChildNode; zU8 aValue[sizeof(TValue)]; }; 
							ZENInline TValue& Value(){return *(TValue*)aValue;} };

			zUInt				GetSlotCount() const;						//! @brief Get count of valid Slots in this node
			bool				IsLeafSlot( zUInt _uSlotID ) const;			//! @brief Check if a Node Slot is a leaf (contains value) or point to child node
			bool				IsUsedIndex( zUInt _uNodeIndex ) const;		//! @brief Check if Node Index is being used in this Node 
			zUInt				GetSlotID( zUInt _uNodeIndex ) const;		//! @brief Get in which slot a Node Index is stored
			int					GetFirstUsedSlotID()const;					//! @brief Return first used slotID (-1 if none)			
			int					GetLastUsedSlotID()const;					//! @brief Return last used slotID (-1 if none)

			Slot*				mpSlots;									//!< Array of slots
			TIndex				mIndexUsed;									//!< Keep track of Node Index with a valid slot allocated
			TIndex				mSlotLeaf;									//!< Keep track of Slot that are a leaf (otherwise is link to child node)			
		};	  
		
	public:	
		typedef void (*DeleteItemCB)(zHamt& Hamt, TValue& ItemDel);

		//=================================================================================================
		//! @class		Iterator
		//-------------------------------------------------------------------------------------------------
		//! @brief		Allow tree traversal, element by element
		//=================================================================================================
		class Iterator
		{
		public:
								Iterator();
								Iterator(const Iterator& _Copy);
								Iterator(const zHamt& _Parent );
			
			bool				IsValid();
			TKey				GetKey();
			TValue&				GetValue();
			void				operator++();			
			void				operator--();
			void				operator=(const Iterator& _Copy);
		protected:
			Node*				mpNodeTree[kuTreeMaxDepth];	//!< void* to current node parsed in each tree depth 
			zI8					mpSlotID[kuTreeMaxDepth];	//!< Current slot in each node being parsed
			zI8					msDepth;					//!< Current leaf depth level
			friend class		zHamt;
		};
		
		//=================================================================================================
		// Main Class
		//=================================================================================================
								zHamt();
								zHamt( zUInt _uReservePool);
								~zHamt();
		
		void					Init( zUInt _uReservePool);													//!< @brief Initialize the HashTable
		ZENInline bool			Exist(const TKey _Key)const;															    
		ZENInline bool			IsInit()const;																			    
		ZENInline zUInt			Count() const;																	//!< @brief Number of element (leaf node) stored in hashmap		
																														    
		void					Clear();																				    
		void					SetDefaultValue( const TValue& _Value );										//!< @brief Set value return when no value found

		const TValue&			Get( const TKey _Key ) const;													//!< @brief Return value stored at a key entry
		bool					Get(const TKey _Key, TValue& _ValueOut) const;									//!< @brief Return value stored at a key entry
		//bool					Get(const TKey _Key, TValue*& _pValueOut) const;								//!< @brief Return value stored at a key entry
		ZENInline const TValue&	operator[](const TKey _Key)const;												//!< @brief Return value stored at that key entry																											    
		TValue&					GetAdd(const TKey _Key);														//!< @brief Return value stored at that key entry
		bool					Unset(const TKey _Key);															//!< @brief Remove the value stored at that key entry
																														    																														    
		void					SetAll( const TValue& _Value );													//!< @brief Set all value in table, to specified value
		void					Set(const TKey _Key, const TValue& _Value);										//!< @brief Assign value at a certain key entry
		bool					SetReplace(const TKey _Key, const TValue& _Value, TValue& _OldValueOut);		//!< @brief Assign value at a certain key entry		
		void					Export( zArrayBase<TKey>& _aKey, zArrayBase<TValue>& _aValue ) const;			//!< @brief Copy the content key/value of this hamt to carrays
		void					Import( const zArrayBase<TKey>& _aKey, const zArrayBase<TValue>& _aValue );	//!< @brief Copy the content key/value from arrays to this hamt
		const zHamt&			operator=(const zHamt& _Copy);													//!< @brief Copy the content of another Hamt
																														    																														    
		void					GetFirst(Iterator& _It) const;													//!< @brief Initialize iteator to first element of the hamt
		void					GetLast(Iterator& _It) const;													//!< @brief Initialize iterator to last element of the hamt
		TKey					GetFirstUnusedKey()const;														//!< @brief Find the first available key in the hashmap
		void					DebugPrint(const TKey _First, TKey _Last) const;								//!< @brief Print structure of the tree
		size_t					GetMemoryFootprint();															//!< @brief Return the amount of memory used by this structure		
		void					SetDeleteItemCB(DeleteItemCB _pCallback){mpDeleteItemCB = _pCallback;}
	protected:
		
		Node*					CreateNodeCopy( const Node* _pNodeCopy );										//!< @brief	Create a copy of a node
		zUInt					GetNodeIndex( const TKey _uKey, zUInt _uDepth ) const;							//!< @brief	Return the key NodeIndex at a certain tree depth
		Node*					CreateEmptyNode(zU32 _uSlotCount);												//!< @brief	Return a new empty node with requested number of slots
		void					ClearNode( Node* _pNode );														//!< @brief	Remove every child node of this node
		TValue*					SetSlotValue(TKey _Key, const TValue& _Value, Node** _ppParentNode, zUInt _uNodeIndex, zUInt _uSlotID, zUInt _uDepth);	//!< @brief		Get the slot in a node associated to a Key. 
		bool					GetNode(TKey _Key, const Node**& _pParentSlot, zU32& _uNodeIndex, zU32& _uSlotID, zU32& _uDepth) const;	//!< @brief Find the Node(and relevant infos) associated to a Key entry
		bool					GetNode(TKey _Key, Node**& _pParentSlot, zU32& _uNodeIndex, zU32& _uSlotID, zU32& _uDepth);	//!< @brief	Find the Node(and relevant infos) associated to a Key entry		
				
		Node*					mpRootNode;				//!< First accessible node
		zU32					muCount;				//!< Keep track of element count in the table, for debug purposes
		TValue					mDefault;				//!< Default value to assign when accessing a non-existing entry
		zenMem::zAllocatorPool	mPools[kuPoolCount];	//!< PreAllocated memory pools, to contain our nodes	
		DeleteItemCB			mpDeleteItemCB;
		friend class Iterator;
	};


	//=================================================================================================
	//! @class		zMap
	//-------------------------------------------------------------------------------------------------
	//! @brief				PreConfigured Hamt for use as a zMap
	//! @tparam _Data_		Datatype of Values stored
	//! @tparam _Null_		Value to return when a Key entry isn't found
	//=================================================================================================
	template<class _Value_>
	struct zMap
	{
		typedef zHamt<zU16, _Value_, zU16, 4>		Key16;	//!< zMap with 16bits Keys with node of 16 entries each (maximum tree depth of 4)
		typedef zHamt<zHash32, _Value_, zU32, 5>	Key32;	//!< Size optimized hamt, 32bits Keys with node of 32 entries each (maximum tree depth of 7)
		typedef zHamt<zHash64, _Value_, zU64, 6>	Key64;	//!< Size optimized hamt, 64bits Keys with node of 64 entries each (maximum tree depth of 11)
	};

	//=================================================================================================
	//! @class		SparseArray
	//-------------------------------------------------------------------------------------------------
	//! @brief				PreConfigured Hamt for use as a zMap
	//! @tparam _Data_		Datatype of Values stored
	//! @tparam _Null_		Value to return when a Key entry isn't found
	//=================================================================================================
	template<class _Value_>
	struct zArraySparse
	{
		typedef zHamt<zU16, _Value_, zU16, 4 >Key16;		//!< Size optimized hamt, 16bits Keys with node of 16 entries each (maximum tree depth of 4)
		typedef zHamt<zU32, _Value_, zU32, 5> Key32;		//!< Size optimized hamt, 32bits Keys with node of 32 entries each (maximum tree depth of 7)
		typedef zHamt<zU64, _Value_, zU64, 6> Key64;		//!< Size optimized hamt, 64bits Keys with node of 64 entries each (maximum tree depth of 11)
	};

} } //namespace zen, Type

#include "zenBaseTypeHamtSmall.inl"

#endif	