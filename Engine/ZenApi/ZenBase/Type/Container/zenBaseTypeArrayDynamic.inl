
namespace zen { namespace zenType {

//==================================================================================================
//! @details	Allow to copy from another array of different type, as long as the type support
//!				equal assignment
//--------------------------------------------------------------------------------------------------
//==================================================================================================
template<class TType> 
zArrayDyn<TType>& zArrayDyn<TType>::Copy(const TType* zenRestrict inpCopy, zUInt inCount)
{
	resize(inCount);
	if( mCount > 0 )
		Super::Copy<TType>(inpCopy, inCount);
	return *this;
}

//==================================================================================================
//! @details	Allow to copy from another array of different type, as long as the type support
//!				equal assignment
//--------------------------------------------------------------------------------------------------
//==================================================================================================
template<class TType> template<class TTypeImport>
zArrayDyn<TType>& zArrayDyn<TType>::Copy(const TTypeImport* zenRestrict inpCopy, zUInt inCount)
{
	resize(inCount);
	if( mCount > 0 )
		Super::Copy<TTypeImport>(inpCopy, inCount);
	return *this;
}

//==================================================================================================
//! @details	Allow to copy from another array of different type, 
//!				as long as the type support equal assignment
//--------------------------------------------------------------------------------------------------
//==================================================================================================
template<class TType> template<class TTypeImport>
zArrayDyn<TType>& zArrayDyn<TType>::Copy(const zArray<TTypeImport>& zenRestrict inCopy )
{
	resize(inCopy.size());
	if( mCount > 0 )
		Super::Copy<TTypeImport>(&inCopy.front(), inCopy.size());
	return *this;
}

//==================================================================================================
//! @details	Remove an element from the array by replacing it with last element.
//!				This avoids costly re-shuffling of entire array
//--------------------------------------------------------------------------------------------------
//==================================================================================================
template<class TType>
TType& zArrayDyn<TType>::Erase_swap(zUInt inIndex)
{
	zenAssert(inIndex < mCount);
	mpData[inIndex] = mpData[mCount-1];
	resize(mCount-1);
	return mpData[inIndex];
}

} } //namespace zen, Type

