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
		inline const zenResID&	GetResID()const;	//!< @brief return resource ID of resource
		inline bool				IsValid()const;		//!< @brief true if the object is a valid resource
	protected:
		inline					awResource();
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
		inline				awResourceRef();
		inline				awResourceRef(awResource* _pResource);
		inline				awResourceRef(awFlagResType _SupportedTypes);		
		inline				awResourceRef(awFlagResType _SupportedTypes, awResource* _pResource);
							awResourceRef(awFlagResType _SupportedTypes, zenResID _ResourceID);
		
		inline const		awResourceRef&	operator=(awResource* _pResource);
		inline const		awResourceRef&	operator=(const awResourceRef& _ResourceRef);
		 const				awResourceRef&	operator=(const zenResID& _ResourceID);
	
		inline bool			IsValid()const;
		inline zenResID		GetResID()const;
		inline bool			operator==(const awResourceRef& _rCmp)const;
		inline bool			operator!=(const awResourceRef& _rCmp)const;
		inline				operator zenResID()const;
		
	protected:
		inline void			SetResource(awResource* _pResource);
		awResource*			mpResource;
		ZENDbgCode( awFlagResType mSupportedTypeMask; )
	};

	//=============================================================================================
	//! @class	Specialized version of awResourceRef, that supports 1 resource type for this
	//!			object, when checking validity of resource type
	//=============================================================================================	
	template<awconst::eResType TType>
	class awResourceTypedRef : public awResourceRef
	{
	ZENClassDeclare(awResourceTypedRef, awResourceRef)
	public:
		inline				awResourceTypedRef();
		inline				awResourceTypedRef(awResource* _pResource);
		inline				awResourceTypedRef(zenResID _ResourceID);
	};

	//=============================================================================================
	//! @class	Specialized version of awResourceRef, that supports 2 resource type for this
	//!			object, when checking validity of resource type
	//=============================================================================================	
	template<awconst::eResType TType1, awconst::eResType TType2>
	class awResourceTyped2Ref : public awResourceRef
	{
	ZENClassDeclare(awResourceTyped2Ref, awResourceRef)
	public:
		inline				awResourceTyped2Ref();		
		inline				awResourceTyped2Ref(awResource* _pResource);
		inline				awResourceTyped2Ref(zenResID _ResourceID);
	};

}} // namespace zen, zenRes

#include "zenRes.inl"

#endif
