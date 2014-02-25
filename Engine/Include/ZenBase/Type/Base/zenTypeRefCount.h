#pragma once
#ifndef __zenBase_Type_RefCount_h__
#define __zenBase_Type_RefCount_h__

namespace zen { namespace zenType {
	
	template<bool TAutoDelete>
	class zenRefCounted
	{
	AWClassDeclareNoParent(zenRefCounted)
	public:
							zenRefCounted();
		virtual				~zenRefCounted();
		inline void			ReferenceAdd();
		inline void			ReferenceRem();
		int					ReferenceGet();
	protected:
		zenInt				miRefCount;		
	};

	template<class TRefCountedType>
	class zenSharedPtr
	{
	AWClassDeclareNoParent(zenSharedPtr)
	public:
		inline 							zenSharedPtr();
		inline 							zenSharedPtr(TRefCountedType* _pReference);
		inline 							~zenSharedPtr();
		inline void						operator=(TRefCountedType* _pReference);
		inline bool						operator==(const zenSharedPtr& _Cmp);
		inline bool						operator!=(const zenSharedPtr& _Cmp);
		inline bool						IsValid();

		inline TRefCountedType*			operator->();		//!< Return a pointer to resource
		inline const TRefCountedType*	operator->()const;	//!< Return a const pointer to resource
	protected:
		TRefCountedType*				mpReference;
	};
	
}} // namespace zen, zenType

#include "zenTypeRefCount.inl"

#endif
