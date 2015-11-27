#pragma once
#ifndef __zenApi_Base_Type_Array_h__
#define __zenApi_Base_Type_Array_h__

// Forward declare
namespace zbMem { class zAllocator; }

// Array class
namespace zen { namespace zenType 
{

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
	int									Find(const TType& _Value);	
	void								SetAll(const TType& _Value);					//!< @brief Set every array element to specific value
	void								Clear();										//!< @brief Remove all element from array
	ZENInline TType*					First();										//!< @brief void* to 1st element in array	
	ZENInline TType*					Last();											//!< @brief void* to last element in array

	ZENInline const TType*				Last()const;									//!< @brief void* to last element in array
	ZENInline const TType*				First()const;									//!< @brief void* to 1st element in array
	ZENInline zUInt						Count()const;									//!< @brief Number of elements
	ZENInline bool						IsEmpty()const;									//!< @brief True if no element in array
	ZENInline zUInt						SizeMem()const;									//!< @brief Size taken up by all elements
	ZENInline zUInt						SizeItem()const;								//!< @brief Size of 1 element in array
	
	TType&								operator[](zInt _uIndex);
	const TType&						operator[](zInt _uIndex)const;
	zArrayBase<TType>&					operator=( const zArrayBase<TType>& _aCopy );
	bool								operator==(const zArrayBase& _Cmp)const;				//!< @brief	True if all element are the same
	ZENInline bool						operator!=(const zArrayBase& _Cmp)const;				//!< @brief	True if any element is different	
	ZENInline zUInt						Copy(const TType* _pCopy, zUInt _uCount);				//!< @brief Copy the value of a Memory area to this array
	template<class TTypeImport> zUInt	Copy(const TTypeImport* _ImportArray, zUInt _uCount );	//!< @brief Copy the value of another Array	
	template<class TTypeImport> zUInt	Copy(const zArrayBase<TTypeImport>& _ImportArray );		//!< @brief Copy the value of another Array	
	
	void								SetRange(const TType& _Value, zUInt _uFirst=0, zUInt _uLast=0xFFFFFFFF);	//!< @brief Set a range of array elements, to a specific value
	void								Sort();
protected:	
	TType*								mpData;											//!< Data array
	zUInt								muCount;										//!< Current number of elements
};

} } //namespace zen, Type

#include "zenBaseTypeArray.inl"

#endif