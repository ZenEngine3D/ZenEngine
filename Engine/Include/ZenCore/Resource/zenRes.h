#pragma once
#ifndef __zenCore_ResourceRef_h__
#define __zenCore_ResourceRef_h__

namespace zen { namespace zenRes {	

	//=============================================================================================
	//! @class	Base class for all resources that can be created and used
	//=============================================================================================
	class awResource : public zenRefCounted<false>
	{
	ZENClassDeclare(awResource, zenRefCounted<false>)
	public:									
		ZENInline const zenResID&	GetResID()const;	//!< @brief return resource ID of resource
		ZENInline bool				IsValid()const;		//!< @brief true if the object is a valid resource
	protected:
		ZENInline					awResource();
		zenResID				mResID;
	};

	//=============================================================================================
	//! @class	SmartPointer for resource. Uses the resource refcounting mechanics, and check 
	//!			if resource that we assign, is part of the supported resource type.
	//=============================================================================================	
	class awResourceRef
	{
	ZENClassDeclareNoParent(awResourceRef)
	public:
		ZENInline				awResourceRef();
		ZENInline				awResourceRef(awResource* _pResource);
		ZENInline				awResourceRef(zenFlagResType _SupportedTypes);		
		ZENInline				awResourceRef(zenFlagResType _SupportedTypes, awResource* _pResource);
							awResourceRef(zenFlagResType _SupportedTypes, zenResID _ResourceID);
		
		ZENInline const		awResourceRef&	operator=(awResource* _pResource);
		ZENInline const		awResourceRef&	operator=(const awResourceRef& _ResourceRef);
		 const				awResourceRef&	operator=(const zenResID& _ResourceID);
	
		ZENInline bool			IsValid()const;
		ZENInline zenResID		GetResID()const;
		ZENInline bool			operator==(const awResourceRef& _rCmp)const;
		ZENInline bool			operator!=(const awResourceRef& _rCmp)const;
		ZENInline				operator zenResID()const;
		
	protected:
		ZENInline void			SetResource(awResource* _pResource);
		awResource*			mpResource;
		ZENDbgCode( zenFlagResType mSupportedTypeMask; )
	};

	//=============================================================================================
	//! @class	Specialized version of awResourceRef, that supports 1 resource type for this
	//!			object, when checking validity of resource type
	//=============================================================================================	
	template<zenConst::eResType TType>
	class awResourceTypedRef : public awResourceRef
	{
	ZENClassDeclare(awResourceTypedRef, awResourceRef)
	public:
		ZENInline				awResourceTypedRef();
		ZENInline				awResourceTypedRef(awResource* _pResource);
		ZENInline				awResourceTypedRef(zenResID _ResourceID);
	};

	//=============================================================================================
	//! @class	Specialized version of awResourceRef, that supports 2 resource type for this
	//!			object, when checking validity of resource type
	//=============================================================================================	
	template<zenConst::eResType TType1, zenConst::eResType TType2>
	class awResourceTyped2Ref : public awResourceRef
	{
	ZENClassDeclare(awResourceTyped2Ref, awResourceRef)
	public:
		ZENInline				awResourceTyped2Ref();		
		ZENInline				awResourceTyped2Ref(awResource* _pResource);
		ZENInline				awResourceTyped2Ref(zenResID _ResourceID);
	};

}} // namespace zen, zenRes

#include "zenRes.inl"

#endif
