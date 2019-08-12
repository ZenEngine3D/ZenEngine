#pragma once

namespace zen { namespace zenType {

template<class TType, unsigned long TCount>
class zArrayFixed : public zArray<TType>
{
zenClassDeclare(zArrayFixed, zArray<TType>)
public:
	zArrayFixed();
	zArrayFixed(const zArray<TType>& inCopy);
	zArrayFixed(const zArrayFixed<TType,TCount>& inCopy); //! @note : Have to declare operator= with same class, to prevent wrong default one created by compiler	
	zArrayFixed(std::initializer_list<TType> inEntries);
	zArrayFixed& operator=(const zArray<TType>& inCopy);	
	template<unsigned long TCountCopy> zArrayFixed& operator=(const zArrayFixed<TType,TCountCopy>& inCopy); //! @note : Have to declare operator= with same class, to prevent wrong default one created by compiler
protected:
	TType mDataArray[TCount];
};


} } //namespace zen, Type

#include "zenBaseTypeArrayFixed.inl"