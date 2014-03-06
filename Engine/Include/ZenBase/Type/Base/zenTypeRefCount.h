#pragma once
#ifndef __zenBase_Type_RefCount_h__
#define __zenBase_Type_RefCount_h__

namespace zen { namespace zenType {
	
	template<bool TAutoDelete>
	class zenRefCounted
	{
	ZENClassDeclareNoParent(zenRefCounted)
	public:
							zenRefCounted();
		virtual				~zenRefCounted();
		ZENInline void			ReferenceAdd();
		ZENInline void			ReferenceRem();
		int					ReferenceGet();
	protected:
		zenInt				miRefCount;		
	};

	template<class TRefCountedType>
	class zenSharedPtr
	{
	ZENClassDeclareNoParent(zenSharedPtr)
	public:
		ZENInline 							zenSharedPtr();
		ZENInline 							zenSharedPtr(TRefCountedType* _pReference);
		ZENInline 							~zenSharedPtr();
		ZENInline void						operator=(TRefCountedType* _pReference);
		ZENInline bool						operator==(const zenSharedPtr& _Cmp);
		ZENInline bool						operator!=(const zenSharedPtr& _Cmp);
		ZENInline bool						IsValid();

		ZENInline TRefCountedType*			operator->();		//!< Return a pointer to resource
		ZENInline const TRefCountedType*	operator->()const;	//!< Return a const pointer to resource
	protected:
		TRefCountedType*				mpReference;
	};
	
}} // namespace zen, zenType

#include "zenTypeRefCount.inl"

#endif
