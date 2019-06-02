#include "zbBase.h"

namespace zen { namespace zenType {

zRefCounted::PendingDelList zRefCounted::sLstPendingDel[3];
zUInt zRefCounted::suLstPendingDelIndex = 0;

void zRefCounted::ReleasePendingDelete()
{
	suLstPendingDelIndex		= (suLstPendingDelIndex + 1) % zenArrayCount(sLstPendingDel);
	zRefCounted* pCurrentItem	= sLstPendingDel[suLstPendingDelIndex].pop_front();
	while( pCurrentItem )
	{
		if( pCurrentItem->miRefCount <= 0)
			pCurrentItem->ReferenceDeleteCB();
		pCurrentItem = sLstPendingDel[suLstPendingDelIndex].pop_front();
	}
}

void zRefCounted::ReleasePendingAtProgramEnd()
{
	for(int i(0); i<zenArrayCount(sLstPendingDel); ++i)
		ReleasePendingDelete();
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
