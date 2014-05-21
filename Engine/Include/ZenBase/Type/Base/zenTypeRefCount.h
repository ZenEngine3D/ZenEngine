#pragma once
#ifndef __zenBase_Type_RefCount_h__
#define __zenBase_Type_RefCount_h__

namespace zen { namespace zenType {
	
	class zRefCounted
	{
	ZENClassDeclareNoParent(zRefCounted)
	public:
		ZENInline			zRefCounted();
		virtual				~zRefCounted();
		ZENInline void		ReferenceAdd();
		ZENInline void		ReferenceRem();
		ZENInline zInt		ReferenceCount();
	protected:
		virtual	 void		ReferenceNoneCB();	//!< Called when no reference are left on object
		zI32				miRefCount;		
	};
	
	class zRefCountedAutoDel : public zRefCounted
	{
	ZENClassDeclare(zRefCountedAutoDel, zRefCounted);
	protected:
		virtual	 void		ReferenceNoneCB();
	};

	template<class TRefCountedType>
	class zRefOwner
	{
	ZENClassDeclareNoParent(zRefOwner)
	public:
		ZENInline 							zRefOwner();
		ZENInline 							zRefOwner(TRefCountedType* _pReference);
		ZENInline 							zRefOwner(const zRefOwner& _Copy);
		ZENInline 							~zRefOwner();
		ZENInline const zRefOwner&			operator=(TRefCountedType* _pReference);
		ZENInline const zRefOwner&			operator=(const zRefOwner& _Copy);
		ZENInline bool						operator==(const zRefOwner& _Cmp);
		ZENInline bool						operator!=(const zRefOwner& _Cmp);
		ZENInline bool						IsValid()const;
	#if ZEN_ENGINELIB
		ZENInline TRefCountedType*			Get();				//!< @note Only use on engine side, else won't compile
		ZENInline const TRefCountedType*	Get()const;			//!< @note Only use on engine side, else won't compile
		ZENInline TRefCountedType*			operator->();		//!< @note Only use on engine side, else won't compile
		ZENInline const TRefCountedType*	operator->()const;	//!< @note Only use on engine side, else won't compile
	#endif
	protected:
		TRefCountedType*					mpReference;
	};

	template<class TRefCountedType>
	class zSharedPtr : public zRefOwner<TRefCountedType>
	{
	ZENClassDeclare(zSharedPtr, zRefOwner )
	public:
		ZENInline 							zSharedPtr();
		ZENInline 							zSharedPtr(TRefCountedType* _pReference);
		ZENInline 							zSharedPtr(const zRefOwner<TRefCountedType>& _Copy);

		ZENInline TRefCountedType*			Get();				//!< Return a pointer to resource
		ZENInline const TRefCountedType*	Get()const;			//!< Return a const pointer to resource
		ZENInline TRefCountedType*			operator->();		//!< Return a pointer to resource
		ZENInline const TRefCountedType*	operator->()const;	//!< Return a const pointer to resource
	};
	
}} // namespace zen, zenType

#include "zenTypeRefCount.inl"

#endif
