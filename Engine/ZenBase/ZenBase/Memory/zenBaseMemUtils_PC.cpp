#include "zbBase.h"

namespace zen { namespace zenMem 
{   

void CopyRaw(void* _pDestination, const void* _pSource, size_t _uSize)
{
	::memcpy( _pDestination, _pSource, _uSize );
}

void Set(void* _pDestination, int _pValue, size_t _uSize)
{
	::memset(_pDestination, _pValue, _uSize);
}

void Zero(void* _pDestination, size_t _uSize)
{
	::memset(_pDestination, 0, _uSize);
}

}} //namespace zen { namespace zenMem
