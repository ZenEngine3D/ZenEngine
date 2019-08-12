#pragma once

// Array class
namespace zen { namespace zenType 
{

template<class TType>
class zArray
{
zenClassDeclareNoParent(zArray)
public:
	class iterator
	{
	public:
							iterator(zArray& inArray, zInt inIndex=0)	: mArray(inArray), mIndex(inIndex){}
							iterator(const iterator& inCopy)			: mArray(inCopy.mArray), mIndex(inCopy.mIndex){}
		bool				IsValid()const							{return (mIndex >= 0 && mIndex < mArray.mCount) || (mIndex < 0 && -mIndex <= mArray.mCount);};
		bool				operator==(const iterator& inCmp)const	{return &mArray == &inCmp.mArray && mIndex == inCmp.mIndex;}
		bool				operator!=(const iterator& inCmp)const	{return (*this == inCmp) == false;}
		explicit			operator bool()const					{return IsValid();}
		TType*				Get()const {return &mArray[mIndex];}
		TType&				operator*()const { return *Get(); }
		TType*				operator->()const { return Get(); }
		iterator&			operator=(const iterator& _Copy);
		iterator&			operator++(){ mIndex += (zUInt)mIndex < mArray.mCount ? 1 : 0; }
		iterator&			operator--(){ mIndex -= mIndex > 0 ? 1 : 0; }
	protected:
		zArray&				mArray;
		zInt				mIndex;
	};
	class const_iterator
	{
	public:
							const_iterator(const zArray& inArray, zInt inIndex=0)	: mArray(inArray), mIndex(inIndex){}
							const_iterator(const const_iterator& inCopy)			: mArray(inCopy.mArray), mIndex(inCopy.mIndex){}
		bool				IsValid()const									{return (mIndex >= 0 && mIndex < mArray.mCount) || (mIndex < 0 && -mIndex <= mArray.mCount);};
		bool				operator==(const const_iterator& inCmp)const	{return &mArray == &inCmp.mArray && mIndex == inCmp.mIndex;}
		bool				operator!=(const const_iterator& inCmp)const	{return (*this == inCmp) == false;}
		explicit			operator bool()const							{return IsValid();}
		const TType*		Get()const {return &mArray[mIndex];}
		const TType&		operator*()const { return *Get(); }
		const TType*		operator->()const { return Get(); }
		const_iterator&		operator=(const const_iterator& _Copy);
		const_iterator&		operator++(){ mIndex += (zUInt)mIndex < mArray.mCount ? 1 : 0; }
		const_iterator&		operator--(){ mIndex -= mIndex > 0 ? 1 : 0; }
	protected:
		const zArray&		mArray;
		zInt				mIndex;
	};

	
	virtual						~zArray(){}

	//Using STL container naming convention
	TType&						at(zInt inIndex)
								{
									zenAssertMsg((inIndex >= 0 &&  (zUInt)inIndex >= mCount) == false, "Trying to access array past its last element");
									zenAssertMsg((inIndex <  0 &&  (zUInt)(-inIndex) >  mCount) == false, "Trying to access array past first element");
									return inIndex >= 0 ? mpData[inIndex] : mpData[mCount+inIndex];
								}
	const TType&				at(zInt inIndex)const
								{
									zenAssertMsg((inIndex >= 0 &&  (zUInt)inIndex >= mCount) == false,"Trying to access array past its last element");
									zenAssertMsg((inIndex <  0 &&  (zUInt)(-inIndex) >  mCount) == false,"Trying to access array past first element");
									return inIndex >= 0 ? mpData[inIndex] : mpData[mCount+inIndex];
								}
	TType&						operator[](zInt inIndex){return at(inIndex);}
	const TType&				operator[](zInt inIndex)const{return at(inIndex);}

	TType&						front(){return at(0);}	
	TType&						back(){return at(-1);}
	const TType&				front()const{return at(0);}
	const TType&				back()const{return at(-1);}
	
	iterator					begin(){return iterator(*this);}
	iterator					end(){return iterator(*this, mCount);}
	const_iterator				begin()const{return const_iterator(*this);}
	const_iterator				end()const{return const_iterator(*this, mCount);}
	const_iterator				cbegin()const{return const_iterator(*this);} 
	const_iterator				cend()const{return const_iterator(*this, mCount);}

	zenInline bool				empty()const{return mCount == 0;}
	zenInline zUInt				size()const{ return mCount; }
	
	zenInline bool				operator==(const zArray<TType>& inCmp)const;

	//Non-STL container naming convention
	zenInline zArray&				Copy(const TType* zenRestrict inpCopy, zUInt inCount);			//!< @brief Copy the value of a Memory area to this array
	template<class TImport> zArray&	Copy(const TImport* zenRestrict inpCopy, zUInt inCount);	//!< @brief Copy the value of another Array	
	template<class TImport> zArray&	Copy(const zArray<TImport>& zenRestrict inCopy);			//!< @brief Copy the value of another Array	

	zenInline size_t			SizeMem()const{ return mCount*sizeof(TType); }
	zenInline size_t			SizeItem()const{ return sizeof(TType); }

	const TType*				Data()const{ return mpData; }
	TType*						Data(){ return mpData; }	
	void						Sort(); //SF Remove me	
protected:
	TType* mpData				= nullptr;
	zUInt mCount				= 0;
};

// Array of no element
template<class TType>
using zArrayZero = zArray<TType>;


} } //namespace zen, Type

#include "zenBaseTypeArray.inl"
