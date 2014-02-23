#pragma once
#ifndef __LibCore_Api_Type_RefCount_h__
#define __LibCore_Api_Type_RefCount_h__

namespace zen { namespace awtype {
	
	template<bool TAutoDelete>
	class awRefCounted
	{
	AWClassDeclareNoParent(awRefCounted)
	public:
					awRefCounted();
	virtual			~awRefCounted();
	inline void		ReferenceAdd();
	inline void		ReferenceRem();
	int				ReferenceGet();
	protected:
		awInt		miRefCount;		
	};

	
}} // namespace zen, awtype

#include "zenTypeRefCount.inl"

#endif
