#pragma once

#include <atomic>

namespace zen { namespace zenType {
	
//! @todo clean make this all const so we can pass on read only resources
	class zRefCounted
	{
	zenClassDeclareNoParent(zRefCounted)
	public:
		zenInline void					ReferenceAdd();
		zenInline void					ReferenceRem();
		zenInline zInt					ReferenceCount();		
		static void						ReleasePendingDelete();
		static void						ReleasePendingAtProgramEnd();
		virtual							~zRefCounted(){};
	protected:
		virtual	void					ReferenceDeleteCB();	//!< Called when no reference are left on object
		zListLink						mLstPendingDelLink;
		mutable std::atomic<zI32>		miRefCount = 0;
		
		typedef zList<zRefCounted, &zRefCounted::mLstPendingDelLink, true> TypeList;		
		static zUInt					suLstPendingDelIndex;
		static TypeList					sLstPendingDel[3];		
	};
	
	class zReference
	{
	zenClassDeclareNoParent(zReference)
	public:
		 									zReference();
		 									zReference(zRefCounted* _pReference);
		 									zReference(const zReference& _Copy);
		 									~zReference();
		 const zReference&					operator=(zRefCounted* _pReference);
		 const zReference&					operator=(const zReference& _Copy);
		zenInline bool						operator==(const zReference& _Cmp)const;
		zenInline bool						operator!=(const zReference& _Cmp)const;
		zenInline bool						operator==(const zRefCounted* _pCmp)const;
		zenInline bool						operator!=(const zRefCounted* _pCmp)const;
		zenInline bool						IsValid()const;
	protected:
		zRefCounted*						mpReference;
	};

	template<class TRefCountedType>
	class zEngineRef : public zReference
	{
	zenClassDeclare(zEngineRef, zReference)
	public:
		zenInline 							zEngineRef();
		zenInline 							zEngineRef(TRefCountedType* _pReference);
		zenInline 							zEngineRef(const zEngineRef& _Copy);		
		zenInline const zEngineRef&			operator=(TRefCountedType* _pReference);
		zenInline const zEngineRef&			operator=(const zEngineRef& _Copy);
	#if ZEN_ENGINELIB
		zenInline TRefCountedType*			Get()const;				//!< @note Only use on engine side, else won't compile
		zenInline const TRefCountedType*	GetConst()const;		//!< @note Only use on engine side, else won't compile
		zenInline TRefCountedType*			GetSafe()const;			//!< @note Only use on engine side, else won't compile
		zenInline const TRefCountedType*	GetConstSafe()const;	//!< @note Only use on engine side, else won't compile
		zenInline TRefCountedType*			operator->()const;		//!< @note Only use on engine side, else won't compile				
	#endif
		using Super::operator==;
		using Super::operator!=;
		typedef TRefCountedType Class;
	};
	
	template<class TRefCountedType>
	class zEngineConstRef : public zReference
	{
	zenClassDeclare(zEngineConstRef, zReference)
	public:
		zenInline 							zEngineConstRef();
		zenInline 							zEngineConstRef(const TRefCountedType* _pReference);
		zenInline 							zEngineConstRef(const zEngineConstRef& _Copy);
		zenInline 							zEngineConstRef(const zEngineRef<TRefCountedType>& _Copy);
		zenInline const zEngineConstRef&	operator=(const TRefCountedType* _pReference);
		zenInline const zEngineConstRef&	operator=(const zEngineConstRef& _Copy);
		zenInline const zEngineConstRef&	operator=(const zEngineRef<TRefCountedType>& _Copy);
	#if ZEN_ENGINELIB
		zenInline const TRefCountedType*	Get()const;			//!< @note Only use on engine side, else won't compile
		zenInline const TRefCountedType*	GetSafe()const;		//!< @note Only use on engine side, else won't compile
		zenInline const TRefCountedType*	operator->()const;	//!< @note Only use on engine side, else won't compile
	#endif
		using Super::operator==;
		using Super::operator!=;
	};
	
	template<class TRefCountedType>
	class zGameRef : public zReference
	{
	zenClassDeclare(zGameRef, zReference)
	public:
		zenInline 							zGameRef();
		zenInline 							zGameRef(TRefCountedType* _pReference);
		zenInline 							zGameRef(const zEngineRef<TRefCountedType>& _Copy);
		zenInline const zGameRef&			operator=(TRefCountedType* _pReference);
		zenInline const zGameRef&			operator=(const zGameRef& _Copy);

		zenInline TRefCountedType&			operator*()const{return *Get();}		//!< Return a pointer to resource

		zenInline TRefCountedType*			Get()const;				//!< Return a pointer to resource
		zenInline TRefCountedType*			GetSafe()const;			//!< Return a pointer to resource and do nullptr check first
		zenInline TRefCountedType*			operator->()const;		//!< Return a pointer to resource
		using Super::operator==;
		using Super::operator!=;
	};
	
	template<class TRefCountedType>
	class zGameConstRef : public zReference
	{
	zenClassDeclare(zGameConstRef, zReference)
	public:
		zenInline 							zGameConstRef();
		zenInline 							zGameConstRef(const TRefCountedType* _pReference);
		zenInline 							zGameConstRef(const zGameConstRef& _Copy);
		zenInline 							zGameConstRef(const zGameRef<TRefCountedType>& _Copy);
		zenInline const zGameConstRef&		operator=(const TRefCountedType* _pReference);
		zenInline const zGameConstRef&		operator=(const zGameConstRef& _Copy);
		zenInline const zGameConstRef&		operator=(const zGameRef<TRefCountedType>& _Copy);

		zenInline const TRefCountedType*	Get()const;			//!< Return a const pointer to resource
		zenInline const TRefCountedType*	GetSafe()const;		//!< Return a const pointer to resource and do nullptr check first
		zenInline const TRefCountedType*	operator->()const;	//!< Return a const pointer to resource
		using Super::operator==;
		using Super::operator!=;
	};

}} // namespace zen, zenType

#include "zenBaseTypeRefCount.inl"

