#pragma once

namespace zen { namespace zenType 
{

//! @todo 2 Move constructor
template<class TType>
class zArrayDyn: public zArray<TType>
{
zenClassDeclare(zArrayDyn, zArray<TType>)
public:
	zArrayDyn()
	{
	}
	zArrayDyn(zUInt inCount)
	{
		resize(inCount);
	}

	zArrayDyn(const zArray<TType>& inCopy)
	{ 
		Copy<TType>(inCopy); 
	}

	//! @note : Have to declare operator= with same class, to prevent wrong default one created by compiler
	zArrayDyn(const zArrayDyn<TType>& inCopy)
	{ 
		Copy<TType>(inCopy); 
	}

	zArrayDyn(const TType* InpCopy, zUInt inCount)
	{
		Copy(InpCopy, inCount);
	}

	zArrayDyn(std::initializer_list<TType> inEntries)
	{
		Copy(inEntries.begin(), inEntries.size());
	}

	virtual ~zArrayDyn()
	{
		zenMem::DelSafe(mpData);
	}
	
	void resize(zUInt inCount, const TType& inDefault)
	{
		mCount = inCount;
		if( inCount == 0 )
			zenMem::DelSafe(mpData);
		else
			mpData = (mpData == nullptr) ? zenMem::NewResizeable<TType>(inCount, inDefault) : zenMem::Resize<TType>(mpData, inCount, inDefault);		
	}

	//Using STL container naming convention
	zUInt resize(zUInt inCount)
	{
		mCount = inCount;
		if( inCount == 0 )
			zenMem::DelSafe(mpData);
		else
			mpData = (mpData == nullptr) ? zenMem::NewResizeable<TType>(inCount) : zenMem::Resize<TType>(mpData, inCount);		
		return mCount;
	}

	zArrayDyn& push_back(const TType& inItemAdd)
	{
		resize(mCount + 1, inItemAdd);
		return *this;
	}

	TType pop_back()
	{
		zenAssert(empty() == false);
		TType value = back();
		resize(mCount - 1);
		return value;
	}

	void clear()
	{
		resize(0);
	}

	zenInline zArrayDyn& operator=(const zArray<TType>& inCopy)
	{ 
		return Copy<TType>(inCopy); 
	}
	//! @note : Have to declare operator= with same class, to prevent wrong default one created by compiler
	zenInline zArrayDyn& operator=(const zArrayDyn<TType>& inCopy)
	{ 
		return Copy<TType>(inCopy); 
	}

	//Non-STL container naming convention
	zArrayDyn&								Copy(const TType* zenRestrict inpCopy, zUInt inCount);			//!< @brief Copy the value of a Memory area to this array
	template<class TTypeImport> zArrayDyn&	Copy(const TTypeImport* zenRestrict inpCopy, zUInt inCount);	//!< @brief Copy the value of another Array	
	template<class TTypeImport> zArrayDyn&	Copy(const zArray<TTypeImport>& zenRestrict inCopy);			//!< @brief Copy the value of another Array	
	TType&									Erase_swap(zUInt inIndex);
	void									Reserve(zUInt inCount){}	//SF Remove me or implement properly
};

} } //namespace zen, Type

#include "zenBaseTypeArrayDynamic.inl"
