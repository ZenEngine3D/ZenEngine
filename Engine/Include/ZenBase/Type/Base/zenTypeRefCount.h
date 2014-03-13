#pragma once
#ifndef __zenBase_Type_RefCount_h__
#define __zenBase_Type_RefCount_h__

namespace zen { namespace zenType {
	
	template<bool TAutoDelete>
	class zRefCounted
	{
	ZENClassDeclareNoParent(zRefCounted)
	public:
							zRefCounted();
		virtual				~zRefCounted();
		ZENInline void		ReferenceAdd();
		ZENInline void		ReferenceRem();
		int					ReferenceGet();
	protected:
		zInt				miRefCount;		
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
		ZENInline bool						operator==(const zRefOwner& _Cmp);
		ZENInline bool						operator!=(const zRefOwner& _Cmp);
		ZENInline bool						IsValid();

	protected:
		TRefCountedType*					mpReference;
	};

	template<class TRefCountedType>
	class zSharedPtr : public zRefOwner<TRefCountedType>
	{
	ZENClassDeclare(zSharedPtr, zRefOwner<TRefCountedType>)
	public:
		ZENInline 							zSharedPtr();
		ZENInline 							zSharedPtr(TRefCountedType* _pReference);
		ZENInline 							zSharedPtr(const zRefOwner<TRefCountedType>& _Copy);
		/*
		ZENInline 							~zSharedPtr();
		ZENInline const zSharedPtr&			operator=(TRefCountedType* _pReference);
		ZENInline bool						operator==(const zSharedPtr& _Cmp);
		ZENInline bool						operator!=(const zSharedPtr& _Cmp);
		ZENInline bool						IsValid();
		*/
		ZENInline TRefCountedType*			Get();				//!< Return a pointer to resource
		ZENInline const TRefCountedType*	Get()const;			//!< Return a const pointer to resource
		ZENInline TRefCountedType*			operator->();		//!< Return a pointer to resource
		ZENInline const TRefCountedType*	operator->()const;	//!< Return a const pointer to resource
		
	protected:
		TRefCountedType*				mpReference;
	};
	
}} // namespace zen, zenType

#include "zenTypeRefCount.inl"

#endif
