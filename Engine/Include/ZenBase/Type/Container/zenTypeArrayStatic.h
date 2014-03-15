#pragma once
#ifndef __zenBase_Type_ArrayStatic_h__
#define __zenBase_Type_ArrayStatic_h__

namespace zen { namespace zenType {

template<class TType>
class zArrayStatic : public zArrayBase<TType>
{
ZENClassDeclare(zArrayStatic, zArrayBase<TType>)	
public:
					zArrayStatic();
					zArrayStatic(zUInt _uCount);
					zArrayStatic(const TType* _pCopy, zUInt _uCount, zUInt _uExtraCount=0);
					zArrayStatic(const zArrayStatic& _Copy, zUInt _uExtraCount=0);
	virtual			~zArrayStatic();
				
	void			Swap(zArrayStatic<TType>& _Copy);		//!< @brief Exchange content between 2 arrays
	virtual zUInt	SetCount(zUInt _uCount);				//!< @brief Resize array
protected:
	virtual zUInt	SetCountNoConstructor(zUInt _uCount);	//!< @brief Resize array without calling constructor (usefull for quick memcopy, use carefully)
public:
	using zArrayBase<TType>::operator=;
};

} } //namespace zen, Type

#include "zenTypeArrayStatic.inl"

#endif	
