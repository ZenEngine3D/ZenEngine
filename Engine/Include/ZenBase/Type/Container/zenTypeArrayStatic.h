#pragma once
#ifndef __zenBase_Type_ArrayStatic_h__
#define __zenBase_Type_ArrayStatic_h__

namespace zen { namespace zenType {

template<class TType>
class zenArrayStatic : public zenArrayBase<TType>
{
ZENClassDeclare(zenArrayStatic, zenArrayBase<TType>)	
public:
					zenArrayStatic();
					zenArrayStatic(zenUInt _uCount);
					zenArrayStatic(const TType* _pCopy, zenUInt _uCount, zenUInt _uExtraCount=0);
					zenArrayStatic(const zenArrayStatic& _Copy, zenUInt _uExtraCount=0);
	virtual			~zenArrayStatic();
				
	void			Swap(zenArrayStatic<TType>& _Copy);		//!< @brief Exchange content between 2 arrays
	virtual zenUInt	SetCount(zenUInt _uCount);				//!< @brief Resize array
protected:
	virtual zenUInt	SetCountNoConstructor(zenUInt _uCount);	//!< @brief Resize array without calling constructor (usefull for quick memcopy, use carefully)
public:
	using zenArrayBase<TType>::operator=;
};

} } //namespace zen, Type

#include "zenTypeArrayStatic.inl"

#endif	
