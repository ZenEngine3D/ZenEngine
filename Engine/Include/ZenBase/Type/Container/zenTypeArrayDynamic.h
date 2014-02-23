#pragma once
#ifndef __zenBase_Type_ArrayDynamic_h__
#define __zenBase_Type_ArrayDynamic_h__

namespace zen { namespace zenType {

	//! @brief Default array size increase policy (1.5x needed size)
	typedef zenUInt(*GrowthPolicyFunction)(zenUInt _uCurrentCount, zenUInt _uNeededCount, zenUInt _uItemSize); 
	zenUInt GrowthPolicyOneandHalf( zenUInt _uCurrentCount, zenUInt _uNeededCount, zenUInt _uItemSize);

	template<class TType, GrowthPolicyFunction TGrowthPolicy=GrowthPolicyOneandHalf>
	class zenArrayDynamic : public zenArrayBase<TType>
	{
	AWClassDeclare(zenArrayDynamic, zenArrayBase<TType>)
	public:
									zenArrayDynamic(CMem::Allocator* _pAllocator=NULL);		
									zenArrayDynamic(zenU32 _uCount, CMem::Allocator* _pAllocator=NULL);
									zenArrayDynamic(const TType* _pCopy, zenUInt _uCount, zenUInt _uExtraCount=0, CMem::Allocator* _pAllocator=NULL);
									zenArrayDynamic(const zenArrayDynamic& _Copy, zenUInt _uExtraCount=0);
		virtual						~zenArrayDynamic();
		virtual zenUInt				SetCount(zenUInt _uCount);
		
		void						operator+=( const zenArrayBase<TType>& _ArrayAdd );
		void						Append(const TType& _Copy);
		void						Append(const TType* _Copy, zenUInt _uCount);

		void						Reserve(zenUInt _uCount);
		TType						Pop();
		zenUInt						ReservedCount()const;
		zenUInt						ReservedSize()const;
	protected:
		void						Shrink( );											//!< @brief	Reduce array size when needed
		void						Grow( zenUInt _auCountNeeded );						//!< @brief Increase array size when needed
		void						GrowNoConstructor( zenUInt _auCountNeeded );			//!< @brief Increase array size without calling TType's constructors
		virtual zenUInt				SetCountNoConstructor(zenUInt _uCount);				//!< @brief Resize array without calling constructor (usefull for quick memcopy, use carefully)
		zenUInt						muCountReserved;									//!< Space currently allocated
		zenUInt						muCountReservedMin;									//!< Minimum space we should always have allocated
	public:
		using zenArrayBase<TType>::operator=;
	};

} } //namespace zen, Type


#include "zenTypeArrayDynamic.inl"

#endif	
