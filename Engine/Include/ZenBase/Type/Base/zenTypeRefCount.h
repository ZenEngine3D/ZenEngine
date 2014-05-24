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
		ZENInline bool						operator==(const zReference& _Cmp);
		ZENInline bool						operator!=(const zReference& _Cmp);
		ZENInline bool						IsValid()const;
	protected:
		zRefCounted*						mpReference;
	};

	template<class TRefCountedType>
	class zEngineRefConst : public zReference
	{
	ZENClassDeclare(zEngineRef, zReference)
	public:
		ZENInline 							zEngineRefConst();
		ZENInline 							zEngineRefConst(TRefCountedType* _pReference);
		ZENInline 							zEngineRefConst(const zEngineRefConst& _Copy);
		ZENInline const zEngineRefConst&	operator=(TRefCountedType* _pReference);
		ZENInline const zEngineRefConst&	operator=(const zEngineRefConst& _Copy);
		
#if ZEN_ENGINELIB
		ZENInline const TRefCountedType*	Get()const;			//!< @note Only use on engine side, else won't compile
		ZENInline const TRefCountedType*	operator->()const;	//!< @note Only use on engine side, else won't compile
#endif
	};

	template<class TRefCountedType>
	class zEngineRef : public zEngineRefConst<TRefCountedType>
	{
	ZENClassDeclare(zEngineRef, zEngineRefConst)
	public:
		ZENInline 							zEngineRef();
		ZENInline 							zEngineRef(TRefCountedType* _pReference);
		ZENInline 							zEngineRef(const zEngineRef& _Copy);		
		ZENInline const zEngineRef&			operator=(TRefCountedType* _pReference);
		ZENInline const zEngineRef&			operator=(const zEngineRef& _Copy);
	#if ZEN_ENGINELIB
		ZENInline TRefCountedType*			Get();				//!< @note Only use on engine side, else won't compile
		ZENInline TRefCountedType*			operator->();		//!< @note Only use on engine side, else won't compile
	#endif		
	};

	template<class TRefCountedType>
	class zGameRefConst : public zEngineRefConst<TRefCountedType>
	{
	ZENClassDeclare(zGameRefConst, zEngineRefConst)
	public:
		ZENInline 							zGameRefConst();
		ZENInline 							zGameRefConst(TRefCountedType* _pReference);
		ZENInline 							zGameRefConst(const zGameRefConst<TRefCountedType>& _Copy);
		ZENInline const zGameRefConst&		operator=(TRefCountedType* _pReference);
		ZENInline const zGameRefConst&		operator=(const zGameRefConst& _Copy);

		ZENInline const TRefCountedType*	Get()const;			//!< Return a const pointer to resource
		ZENInline const TRefCountedType*	operator->()const;	//!< Return a const pointer to resource
	};

	template<class TRefCountedType>
	class zGameRef : public zGameRefConst<TRefCountedType>
	{
	ZENClassDeclare(zGameRef, zGameRefConst)
	public:
		ZENInline 							zGameRef();
		ZENInline 							zGameRef(TRefCountedType* _pReference);
		ZENInline 							zGameRef(const zEngineRef<TRefCountedType>& _Copy);
		ZENInline const zGameRef&			operator=(TRefCountedType* _pReference);
		ZENInline const zGameRef&			operator=(const zGameRef& _Copy);

		ZENInline TRefCountedType*			Get();				//!< Return a pointer to resource
		ZENInline TRefCountedType*			operator->();		//!< Return a pointer to resource
	};
	
}} // namespace zen, zenType

#include "zenTypeRefCount.inl"

#endif
