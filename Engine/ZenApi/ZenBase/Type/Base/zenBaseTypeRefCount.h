#pragma once
#ifndef __zenApi_Base_Type_RefCount_h__
#define __zenApi_Base_Type_RefCount_h__

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
	
	class zReference
	{
	ZENClassDeclareNoParent(zReference)
	public:
		 									zReference();
		 									zReference(zRefCounted* _pReference);
		 									zReference(const zReference& _Copy);
		 									~zReference();
		 const zReference&					operator=(zRefCounted* _pReference);
		 const zReference&					operator=(const zReference& _Copy);
		ZENInline bool						operator==(const zReference& _Cmp)const;
		ZENInline bool						operator!=(const zReference& _Cmp)const;
		ZENInline bool						operator==(const zRefCounted* _pCmp)const;
		ZENInline bool						operator!=(const zRefCounted* _pCmp)const;
		ZENInline bool						IsValid()const;
	protected:
		zRefCounted*						mpReference;
	};

	template<class TRefCountedType>
	class zEngineRef : public zReference
	{
	ZENClassDeclare(zEngineRef, zReference)
	public:
		ZENInline 							zEngineRef();
		ZENInline 							zEngineRef(TRefCountedType* _pReference);
		ZENInline 							zEngineRef(const zEngineRef& _Copy);		
		ZENInline const zEngineRef&			operator=(TRefCountedType* _pReference);
		ZENInline const zEngineRef&			operator=(const zEngineRef& _Copy);
	#if ZEN_ENGINELIB
		ZENInline TRefCountedType*			Get()const;				//!< @note Only use on engine side, else won't compile
		ZENInline TRefCountedType*			GetSafe()const;			//!< @note Only use on engine side, else won't compile
		ZENInline TRefCountedType*			operator->()const;		//!< @note Only use on engine side, else won't compile				
	#endif
		using Super::operator==;
		using Super::operator!=;
	};
	
	template<class TRefCountedType>
	class zEngineConstRef : public zReference
	{
	ZENClassDeclare(zEngineRef, zReference)
	public:
		ZENInline 							zEngineConstRef();
		ZENInline 							zEngineConstRef(const TRefCountedType* _pReference);
		ZENInline 							zEngineConstRef(const zEngineConstRef& _Copy);
		ZENInline 							zEngineConstRef(const zEngineRef<TRefCountedType>& _Copy);
		ZENInline const zEngineConstRef&	operator=(const TRefCountedType* _pReference);
		ZENInline const zEngineConstRef&	operator=(const zEngineConstRef& _Copy);
		ZENInline const zEngineConstRef&	operator=(const zEngineRef<TRefCountedType>& _Copy);
	#if ZEN_ENGINELIB
		ZENInline const TRefCountedType*	Get()const;			//!< @note Only use on engine side, else won't compile
		ZENInline const TRefCountedType*	GetSafe()const;		//!< @note Only use on engine side, else won't compile
		ZENInline const TRefCountedType*	operator->()const;	//!< @note Only use on engine side, else won't compile
	#endif
		using Super::operator==;
		using Super::operator!=;
	};
	
	template<class TRefCountedType>
	class zGameRef : public zReference
	{
	ZENClassDeclare(zGameRef, zReference)
	public:
		ZENInline 							zGameRef();
		ZENInline 							zGameRef(TRefCountedType* _pReference);
		ZENInline 							zGameRef(const zEngineRef<TRefCountedType>& _Copy);
		ZENInline const zGameRef&			operator=(TRefCountedType* _pReference);
		ZENInline const zGameRef&			operator=(const zGameRef& _Copy);

		ZENInline TRefCountedType&			operator*()const{return *Get();}		//!< Return a pointer to resource

		ZENInline TRefCountedType*			Get()const;				//!< Return a pointer to resource
		ZENInline TRefCountedType*			GetSafe()const;			//!< Return a pointer to resource and do nullptr check first
		ZENInline TRefCountedType*			operator->()const;		//!< Return a pointer to resource
		using Super::operator==;
		using Super::operator!=;
	};
	
	template<class TRefCountedType>
	class zGameConstRef : public zReference
	{
	ZENClassDeclare(zGameConstRef, zReference)
	public:
		ZENInline 							zGameConstRef();
		ZENInline 							zGameConstRef(const TRefCountedType* _pReference);
		ZENInline 							zGameConstRef(const zGameConstRef& _Copy);
		ZENInline 							zGameConstRef(const zGameRef<TRefCountedType>& _Copy);
		ZENInline const zGameConstRef&		operator=(const TRefCountedType* _pReference);
		ZENInline const zGameConstRef&		operator=(const zGameConstRef& _Copy);
		ZENInline const zGameConstRef&		operator=(const zGameRef<TRefCountedType>& _Copy);

		ZENInline const TRefCountedType*	Get()const;			//!< Return a const pointer to resource
		ZENInline const TRefCountedType*	GetSafe()const;		//!< Return a const pointer to resource and do nullptr check first
		ZENInline const TRefCountedType*	operator->()const;	//!< Return a const pointer to resource
		using Super::operator==;
		using Super::operator!=;
	};

}} // namespace zen, zenType

#include "zenBaseTypeRefCount.inl"

#endif
