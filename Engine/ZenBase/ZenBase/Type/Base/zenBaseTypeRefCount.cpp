#include "zbBase.h"

namespace zen { namespace zenType {

zRefCounted::TypeList zRefCounted::sLstPendingDel[3];
zUInt zRefCounted::suLstPendingDelIndex = 0;

void zRefCounted::ReferenceRelease()
{
	suLstPendingDelIndex		= (suLstPendingDelIndex + 1) % zenArrayCount(sLstPendingDel);
	zRefCounted* pCurrentItem	= sLstPendingDel[suLstPendingDelIndex].PopHead();
	while( pCurrentItem )
	{
		if( pCurrentItem->miRefCount <= 0)
			pCurrentItem->ReferenceDeleteCB();
		pCurrentItem = sLstPendingDel[suLstPendingDelIndex].PopHead();
	}
}

void zRefCounted::ReferenceDeleteCB()
{
	delete this;
}

zReference::zReference()
: mpReference(nullptr)
{
}

zReference::zReference(zRefCounted* _pReference)
: mpReference(nullptr)
{
	*this = _pReference;	
}

zReference::zReference(const zReference& _Copy)
: mpReference(nullptr)
{
	*this = _Copy.mpReference;
}

zReference::~zReference()
{
	*this = nullptr;
}

const zReference& zReference::operator=(zRefCounted* _pReference)
{	
	if( mpReference )
		mpReference->ReferenceRem();

	mpReference = _pReference;
	
	if( mpReference )
		mpReference->ReferenceAdd();
	
	return *this;
}

const zReference& zReference::operator=(const zReference& _Copy)
{	
	if( &_Copy != this )
	{
		if (mpReference)
			mpReference->ReferenceRem();

		mpReference = _Copy.mpReference;

		if (mpReference)
			mpReference->ReferenceAdd();
	}
	return *this;
}



		
}} // namespace zen, zenType
