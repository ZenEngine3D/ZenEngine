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
	virtual			~zenRefCounted();
	inline void		ReferenceAdd();
	inline void		ReferenceRem();
	int				ReferenceGet();
	protected:
		zenInt		miRefCount;		
	};

	
}} // namespace zen, zenType

#include "zenTypeRefCount.inl"

#endif
