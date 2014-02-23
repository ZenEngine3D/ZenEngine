#pragma once
#ifndef __LibEngine_Api_ResourceRef_h__
#define __LibEngine_Api_ResourceRef_h__

namespace zen { namespace awres {	

	//=============================================================================================
	//! @class	Base class for all resources that can be created and used
	//=============================================================================================
	class awResource : public awRefCounted<false>
	{
	AWClassDeclare(awResource, awRefCounted<false>)
	public:									
		inline const awResourceID&	GetResID()const;	//!< @brief return resource ID of resource
		inline bool					IsValid()const;		//!< @brief true if the object is a valid resource
	protected:
		inline						awResource();
		awResourceID				mResID;
	};

	//=============================================================================================
	//! @class	SmartPointer for resource. Uses the resource refcounting mechanics, and check 
	//!			if resource that we assign, is part of the supported resource type.
	//=============================================================================================	
	class awResourceRef
	{
	AWClassDeclareNoParent(awResourceRef)
	public:
		inline				awResourceRef();
		inline				awResourceRef(awResource* _pResource);
		inline				awResourceRef(awFlagResType _SupportedTypes);		
		inline				awResourceRef(awFlagResType _SupportedTypes, awResource* _pResource);
							awResourceRef(awFlagResType _SupportedTypes, awResourceID _ResourceID);
		
		inline const		awResourceRef&	operator=(awResource* _pResource);
		inline const		awResourceRef&	operator=(const awResourceRef& _ResourceRef);
		 const				awResourceRef&	operator=(const awResourceID& _ResourceID);
	
		inline bool			IsValid()const;
		inline awResourceID	GetResID()const;
		inline bool			operator==(const awResourceRef& _rCmp)const;
		inline bool			operator!=(const awResourceRef& _rCmp)const;
		inline				operator awResourceID()const;
		
	protected:
		inline void			SetResource(awResource* _pResource);
		awResource*			mpResource;
		AWDbgCode( awFlagResType mSupportedTypeMask; )
	};

	//=============================================================================================
	//! @class	Specialized version of awResourceRef, that supports 1 resource type for this
	//!			object, when checking validity of resource type
	//=============================================================================================	
	template<awconst::eResType TType>
	class awResourceTypedRef : public awResourceRef
	{
	AWClassDeclare(awResourceTypedRef, awResourceRef)
	public:
		inline				awResourceTypedRef();
		inline				awResourceTypedRef(awResource* _pResource);
		inline				awResourceTypedRef(awResourceID _ResourceID);
	};

	//=============================================================================================
	//! @class	Specialized version of awResourceRef, that supports 2 resource type for this
	//!			object, when checking validity of resource type
	//=============================================================================================	
	template<awconst::eResType TType1, awconst::eResType TType2>
	class awResourceTyped2Ref : public awResourceRef
	{
	AWClassDeclare(awResourceTyped2Ref, awResourceRef)
	public:
		inline				awResourceTyped2Ref();		
		inline				awResourceTyped2Ref(awResource* _pResource);
		inline				awResourceTyped2Ref(awResourceID _ResourceID);
	};

}} // namespace zen, awres

#include "zenRes.inl"

#endif
