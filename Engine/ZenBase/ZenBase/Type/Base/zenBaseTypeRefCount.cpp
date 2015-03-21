#include "zbBase.h"

namespace zen { namespace zenType {

zRefCounted::~zRefCounted()
{
}

void zRefCounted::ReferenceNoneCB()
{
}
	
void zRefCountedAutoDel::ReferenceNoneCB()
{
	delete this;
}

zReference::zReference()
: mpReference(NULL)
{
}

zReference::zReference(zRefCounted* _pReference)
: mpReference(NULL)
{
	*this = _pReference;	
}

zReference::zReference(const zReference& _Copy)
: mpReference(NULL)
{
	*this = _Copy.mpReference;
}

zReference::~zReference()
{
	*this = NULL;
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
