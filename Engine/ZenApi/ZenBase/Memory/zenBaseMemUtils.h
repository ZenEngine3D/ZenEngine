#pragma once
#ifndef __zenApi_Base_Memory_Utils_h__
#define __zenApi_Base_Memory_Utils_h__

namespace zen { namespace zenMem
{
	void* Copy	(void* _pDestination, const void* _pSource, size_t _uSize);	
	void* Set	(void* _pDestination, int _pValue, size_t _uSize);
	void* Zero	(void* _pDestination, size_t _uSize);
}} // namespace zen { namespace zenMem

//#define memcpy // Making sure program uses our version of memcpy
//#define memset // Making sure program uses our version of memset

#endif