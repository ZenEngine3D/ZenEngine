#pragma once
#ifndef __LibCore_Api_Type_DynamicArray_h__
#define __LibCore_Api_Type_DynamicArray_h__

//! @todo : When adding element, use operator= instead of memcopy to prevent issues

namespace zen { namespace awtype {

	typedef awUInt(*GrowthPolicyFunction)(awUInt _uCurrentCount, awUInt _uNeededCount, awUInt _uItemSize); 
	
	//! @brief Default array size increase policy (1.5x needed size)
	awUInt GrowthPolicyOneandHalf( awUInt _uCurrentCount, awUInt _uNeededCount, awUInt _uItemSize);

	template<class TType, GrowthPolicyFunction TGrowthPolicy=GrowthPolicyOneandHalf>
	class awArrayDynamic : public awArrayBase<TType>
	{
	AWClassDeclare(awArrayDynamic, awArrayBase<TType>)
	public:
									awArrayDynamic(CMem::Allocator* _pAllocator=NULL);		
									awArrayDynamic(awU32 _uCount, CMem::Allocator* _pAllocator=NULL);
									awArrayDynamic(const TType* _pCopy, awUInt _uCount, awUInt _uExtraCount=0, CMem::Allocator* _pAllocator=NULL);
									awArrayDynamic(const awArrayDynamic& _Copy, awUInt _uExtraCount=0);
		virtual						~awArrayDynamic();
		virtual awUInt				SetCount(awUInt _uCount);
		
		void						operator+=( const awArrayBase<TType>& _ArrayAdd );
		void						Append(const TType& _Copy);
		void						Append(const TType* _Copy, awUInt _uCount);

		void						Reserve(awUInt _uCount);
		TType						Pop();
		awUInt						ReservedCount()const;
		awUInt						ReservedSize()const;
	protected:
		void						Shrink( );											//!< @brief	Reduce array size when needed
		void						Grow( awUInt _auCountNeeded );						//!< @brief Increase array size when needed
		void						GrowNoConstructor( awUInt _auCountNeeded );			//!< @brief Increase array size without calling TType's constructors
		virtual awUInt				SetCountNoConstructor(awUInt _uCount);				//!< @brief Resize array without calling constructor (usefull for quick memcopy, use carefully)
		awUInt						muCountReserved;									//!< Space currently allocated
		awUInt						muCountReservedMin;									//!< Minimum space we should always have allocated
	public:
		using awArrayBase<TType>::operator=;
	};

} } //namespace zen, Type


#include "zenTypeDynamicArray.inl"

#endif	
