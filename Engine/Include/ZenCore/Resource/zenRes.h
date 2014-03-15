#pragma once
#ifndef __zenCore_ResourceRef_h__
#define __zenCore_ResourceRef_h__

namespace zen { namespace zenRes {	

	//=============================================================================================
	//! @class	Base class for all resources that can be created and used
	//=============================================================================================
	class zResource : public zRefCounted
	{
	ZENClassDeclare(zResource, zRefCounted)
	public:									
		ZENInline const zResID&	GetResID()const;	//!< @brief return resource ID of resource
		ZENInline bool			IsValid()const;		//!< @brief true if the object is a valid resource
	protected:
		ZENInline				zResource();
		zResID					mResID;
	};

	//=============================================================================================
	//! @class	SmartPointer for resource. Uses the resource refcounting mechanics, and check 
	//!			if resource that we assign, is part of the supported resource type.
	//=============================================================================================	
	class zResourceRef
	{
	ZENClassDeclareNoParent(zResourceRef)
	public:
		ZENInline			zResourceRef();
		ZENInline			zResourceRef(zResource* _pResource);
		ZENInline			zResourceRef(zFlagResType _SupportedTypes);		
		ZENInline			zResourceRef(zFlagResType _SupportedTypes, zResource* _pResource);
							zResourceRef(zFlagResType _SupportedTypes, zResID _ResourceID);
		
		ZENInline const		zResourceRef&	operator=(zResource* _pResource);
		ZENInline const		zResourceRef&	operator=(const zResourceRef& _ResourceRef);
		 const				zResourceRef&	operator=(const zResID& _ResourceID);
	
		ZENInline bool		IsValid()const;
		ZENInline zResID	GetResID()const;
		ZENInline bool		operator==(const zResourceRef& _rCmp)const;
		ZENInline bool		operator!=(const zResourceRef& _rCmp)const;
		ZENInline			operator zResID()const;
		
	protected:
		ZENInline void		SetResource(zResource* _pResource);
		zResource*			mpResource;
		ZENDbgCode( zFlagResType mSupportedTypeMask; )
	};

	//=============================================================================================
	//! @class	Specialized version of zResourceRef, that supports 1 resource type for this
	//!			object, when checking validity of resource type
	//=============================================================================================	
	template<zenConst::eResType TType>
	class zResourceTypedRef : public zResourceRef
	{
	ZENClassDeclare(zResourceTypedRef, zResourceRef)
	public:
		ZENInline				zResourceTypedRef();
		ZENInline				zResourceTypedRef(zResource* _pResource);
		ZENInline				zResourceTypedRef(zResID _ResourceID);
	};

	//=============================================================================================
	//! @class	Specialized version of zResourceRef, that supports 2 resource type for this
	//!			object, when checking validity of resource type
	//=============================================================================================	
	template<zenConst::eResType TType1, zenConst::eResType TType2>
	class zResourceTyped2Ref : public zResourceRef
	{
	ZENClassDeclare(zResourceTyped2Ref, zResourceRef)
	public:
		ZENInline				zResourceTyped2Ref();		
		ZENInline				zResourceTyped2Ref(zResource* _pResource);
		ZENInline				zResourceTyped2Ref(zResID _ResourceID);
	};

}} // namespace zen, zenRes

#include "zenRes.inl"

#endif
