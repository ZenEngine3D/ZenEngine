#pragma once
#ifndef __zenBase_Type_Array_h__
#define __zenBase_Type_Array_h__

// Forward declare
namespace zbMem { class Allocator; }

// Array class
namespace zen { namespace zenType {

template<class TType>
class zenArrayBase
{
//==================================================================================================
// Methods that need to be defined in child classes
//==================================================================================================
protected:
	virtual zenUInt						SetCountNoConstructor(zenUInt _uCount)=0;		//!< @brief Resize array without calling constructor (usefull for quick memcopy, use carefully)
public:		
	virtual zenUInt						SetCount(zenUInt _uCount)=0;						//!< @brief Resize array

//==================================================================================================
// Declarations
//==================================================================================================	
public:
										zenArrayBase();
	void								Sort();											//!< @brief Sort every element in array
	int									Find(const TType& _Value);	
	void								SetAll(const TType& _Value);					//!< @brief Set every array element to specific value
	void								Clear();										//!< @brief Remove all element from array
	TType*								First();										//!< @brief zenPointer to 1st element in array	
	TType*								Last();											//!< @brief zenPointer to last element in array

	const TType*						Last()const;									//!< @brief zenPointer to last element in array
	const TType*						First()const;									//!< @brief zenPointer to 1st element in array
	zenUInt								Count()const;									//!< @brief Number of elements
	zenUInt								Size()const;									//!< @brief Size taken up by all elements
	zenUInt								SizeElement()const;								//!< @brief Size of 1 element in array
	
	TType&								operator[](zenUInt _uIndex);
	const TType&						operator[](zenUInt _uIndex)const;
	zenArrayBase<TType>&				operator=( const zenArrayBase<TType>& _aCopy );	
	zenUInt								Copy(const TType* _pCopy, zenUInt _uCount);				//!< @brief Copy the value of a Memory area to this array
	template<class TTypeImport> zenUInt	Copy(const TTypeImport* _ImportArray, zenUInt _uCount );	//!< @brief Copy the value of another Array	
	template<class TTypeImport> zenUInt	Copy(const zenArrayBase<TTypeImport>& _ImportArray );	//!< @brief Copy the value of another Array	
	void								SetRange(const TType& _Value, zenUInt _uFirst=0, zenUInt _uLast=0xFFFFFFFF);	//!< @brief Set a range of array elements, to a specific value

protected:
	TType*								mpData;											//!< Data array
	zenUInt								muCount;										//!< Current number of elements
};

} } //namespace zen, Type

#include "zenTypeArray.inl"

#endif