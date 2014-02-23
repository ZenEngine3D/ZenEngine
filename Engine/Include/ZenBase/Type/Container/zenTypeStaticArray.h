#pragma once
#ifndef __LibCore_Type_StaticArray_h__
#define __LibCore_Type_StaticArray_h__

namespace zen { namespace awtype {

template<class TType>
class awArrayStatic : public awArrayBase<TType>
{
AWClassDeclare(awArrayStatic, awArrayBase<TType>)	
public:
					awArrayStatic(CMem::Allocator* _pAllocator=NULL);
					awArrayStatic(awUInt _uCount, CMem::Allocator* _pAllocator=NULL);
					awArrayStatic(const TType* _pCopy, awUInt _uCount, awUInt _uExtraCount=0, CMem::Allocator* _pAllocator=NULL);
					awArrayStatic(const awArrayStatic& _Copy, awUInt _uExtraCount=0);
	virtual			~awArrayStatic();
				
	void			Swap(awArrayStatic<TType>& _Copy);		//!< @brief Exchange content between 2 arrays
	virtual awUInt	SetCount(awUInt _uCount);				//!< @brief Resize array
protected:
	virtual awUInt	SetCountNoConstructor(awUInt _uCount);	//!< @brief Resize array without calling constructor (usefull for quick memcopy, use carefully)
public:
	using awArrayBase<TType>::operator=;
};

} } //namespace zen, Type

#include "zenTypeStaticArray.inl"

#endif	
