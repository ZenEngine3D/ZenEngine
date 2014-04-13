#include "libZenBase.h"

namespace zen { namespace zenMem 
{   

void* Copy(void* _pDestination, const void* _pSource, size_t _uSize)
{
	return ::memcpy( _pDestination, _pSource, _uSize );
}

void* Set(void* _pDestination, int _pValue, size_t _uSize)
{
	return ::memset(_pDestination, _pValue, _uSize);
}

void* Zero(void* _pDestination, size_t _uSize)
{
	return ::memset(_pDestination, 0, _uSize);
}

}} //namespace zen { namespace zenMem
