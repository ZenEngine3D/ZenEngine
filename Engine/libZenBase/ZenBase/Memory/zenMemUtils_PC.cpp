#include "libZenBase.h"

namespace zen { namespace zenMem 
{   

void* memcpy(void* _pDestination, const void* _pSource, size_t _uSize)
{
	return ::memcpy( _pDestination, _pSource, _uSize );
}

void* memset(void* _pDestination, int _pValue, size_t _uSize)
{
	return ::memset(_pDestination, _pValue, _uSize);
}

}} //namespace zen { namespace zenMem
