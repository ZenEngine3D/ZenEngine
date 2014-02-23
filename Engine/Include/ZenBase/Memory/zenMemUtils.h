#pragma once
#ifndef __zenBase_Memory_Utils_h__
#define __zenBase_Memory_Utils_h__

namespace zen { namespace zenMem
{
	void* Copy	(void* _pDestination, const void* _pSource, size_t _uSize);	
	void* Set	(void* _pDestination, int _pValue, size_t _uSize);
}} // namespace zen { namespace zenMem

#define memcpy zenMem::Copy	// Making sure program uses our version of memcpy
#define memset zenMem::Set	// Making sure program uses our version of memset

#endif