#pragma once
#ifndef __zenApi_Base_Type_ArrayStatic_h__
#define __zenApi_Base_Type_ArrayStatic_h__

namespace zen { namespace zenType {

template<class TType>
class zArrayStatic : public zArrayBase<TType>
{
zenClassDeclare(zArrayStatic, zArrayBase<TType>)	
public:
					zArrayStatic();
					zArrayStatic(zUInt _uCount);
					zArrayStatic(std::initializer_list<TType> _Entries);
					zArrayStatic(const zArrayStatic& _Copy, zUInt _uExtraCount=0);
					zArrayStatic(const TType* _pCopy, zUInt _uCount, zUInt _uExtraCount=0);
					
	virtual			~zArrayStatic();
				
	void			Swap(zArrayStatic<TType>& _Copy);		//!< @brief Exchange content between 2 arrays
	virtual zUInt	SetCount(zUInt _uCount);				//!< @brief Resize array
protected:
	virtual zUInt	SetCountNoConstructor(zUInt _uCount);	//!< @brief Resize array without calling constructor (usefull for quick memcopy, use carefully)
public:
	using zArrayBase<TType>::operator=;
};

//! @todo Optim : Create a zArrayStackStatic that doesn't use heap, to wrap std::array

} } //namespace zen, Type

#include "zenBaseTypeArrayStatic.inl"

#endif	