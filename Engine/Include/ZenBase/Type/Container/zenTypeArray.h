#pragma once
#ifndef __zenBase_Type_Array_h__
#define __zenBase_Type_Array_h__

// Forward declare
namespace zbMem { class zAllocator; }

// Array class
namespace zen { namespace zenType {

template<class TType>
class zArrayBase
{
//==================================================================================================
// Methods that need to be defined in child classes
//==================================================================================================
protected:
	virtual zUInt						SetCountNoConstructor(zUInt _uCount)=0;		//!< @brief Resize array without calling constructor (usefull for quick memcopy, use carefully)
public:		
	virtual zUInt						SetCount(zUInt _uCount)=0;						//!< @brief Resize array

//==================================================================================================
// Declarations
//==================================================================================================	
public:
										zArrayBase();
	void								Sort();											//!< @brief Sort every element in array
	int									Find(const TType& _Value);	
	void								SetAll(const TType& _Value);					//!< @brief Set every array element to specific value
	void								Clear();										//!< @brief Remove all element from array
	TType*								First();										//!< @brief void* to 1st element in array	
	TType*								Last();											//!< @brief void* to last element in array

	const TType*						Last()const;									//!< @brief void* to last element in array
	const TType*						First()const;									//!< @brief void* to 1st element in array
	zUInt								Count()const;									//!< @brief Number of elements
	zUInt								Size()const;									//!< @brief Size taken up by all elements
	zUInt								SizeElement()const;								//!< @brief Size of 1 element in array
	
	TType&								operator[](zUInt _uIndex);
	const TType&						operator[](zUInt _uIndex)const;
	zArrayBase<TType>&					operator=( const zArrayBase<TType>& _aCopy );
	bool								operator==(const zArrayBase& _Cmp)const;
	zUInt								Copy(const TType* _pCopy, zUInt _uCount);				//!< @brief Copy the value of a Memory area to this array
	template<class TTypeImport> zUInt	Copy(const TTypeImport* _ImportArray, zUInt _uCount );	//!< @brief Copy the value of another Array	
	template<class TTypeImport> zUInt	Copy(const zArrayBase<TTypeImport>& _ImportArray );	//!< @brief Copy the value of another Array	
	void								SetRange(const TType& _Value, zUInt _uFirst=0, zUInt _uLast=0xFFFFFFFF);	//!< @brief Set a range of array elements, to a specific value

protected:
	TType*								mpData;											//!< Data array
	zUInt								muCount;										//!< Current number of elements
};

} } //namespace zen, Type

#include "zenTypeArray.inl"

#endif