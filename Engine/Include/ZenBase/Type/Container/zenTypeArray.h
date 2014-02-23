#pragma once
#ifndef __LibCore_Api_Type_Array_h__
#define __LibCore_Api_Type_Array_h__

// Forward declare
namespace CMem { class Allocator; }

// Array class
namespace zen { namespace awtype {

template<class TType>
class awArrayBase
{
//==================================================================================================
// Methods that need to be defined in child classes
//==================================================================================================
protected:
	virtual awUInt						SetCountNoConstructor(awUInt _uCount)=0;		//!< @brief Resize array without calling constructor (usefull for quick memcopy, use carefully)
public:		
	virtual awUInt						SetCount(awUInt _uCount)=0;						//!< @brief Resize array

//==================================================================================================
// Declarations
//==================================================================================================	
public:
	awArrayBase(CMem::Allocator* _pAllocator=NULL);
	void								Sort();											//!< @brief Sort every element in array
	int									Find(const TType& _Value);	
	void								SetAll(const TType& _Value);					//!< @brief Set every array element to specific value
	void								Clear();										//!< @brief Remove all element from array
	TType*								First();										//!< @brief awPointer to 1st element in array	
	TType*								Last();											//!< @brief awPointer to last element in array

	const TType*						Last()const;									//!< @brief awPointer to last element in array
	const TType*						First()const;									//!< @brief awPointer to 1st element in array
	awUInt								Count()const;									//!< @brief Number of elements
	awUInt								Size()const;									//!< @brief Size taken up by all elements
	awUInt								SizeElement()const;								//!< @brief Size of 1 element in array
	
	TType&								operator[](awUInt _uIndex);
	const TType&						operator[](awUInt _uIndex)const;
	awArrayBase<TType>&					operator=( const awArrayBase<TType>& _aCopy );	
	awUInt								Copy(const TType* _pCopy, awUInt _uCount);				//!< @brief Copy the value of a Memory area to this array
	template<class TTypeImport> awUInt	Copy(const TTypeImport* _ImportArray, awUInt _uCount );	//!< @brief Copy the value of another Array	
	template<class TTypeImport> awUInt	Copy(const awArrayBase<TTypeImport>& _ImportArray );	//!< @brief Copy the value of another Array	
	void								SetRange(const TType& _Value, awUInt _uFirst=0, awUInt _uLast=0xFFFFFFFF);	//!< @brief Set a range of array elements, to a specific value

protected:
	CMem::Allocator*					mpAllocator;									//!< Memory allocated used to allocate elements
	TType*								mpData;											//!< Data array
	awUInt								muCount;										//!< Current number of elements
};

} } //namespace zen, Type

#include "zenTypeArray.inl"

#endif