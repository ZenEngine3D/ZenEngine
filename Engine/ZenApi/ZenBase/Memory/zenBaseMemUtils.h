#pragma once

namespace zen { namespace zenMem
{
	template<class TType>
	void Copy		(TType* _pDestination, const TType* _pSource, zUInt _uCount);
	void CopyRaw	(void* _pDestination, const void* _pSource, size_t _uSize);
	void Set		(void* _pDestination, int _pValue, size_t _uSize);
	void Zero		(void* _pDestination, size_t _uSize);
	template<class TType>
	void Zero		(TType& _Destination);
}} // namespace zen { namespace zenMem


#include "zenBaseMemUtils.inl"

//#define memcpy // Making sure program uses our version of memcpy
//#define memset // Making sure program uses our version of memset
