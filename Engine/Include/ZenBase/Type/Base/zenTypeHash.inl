#include <string.h>

namespace zen { namespace zenType {

//=================================================================================================
// HASH32
//=================================================================================================
zenHash32& zenHash32::Append(const char* _zString)
{		
	while( *_zString )
	{
		muHash ^= *_zString++;
		muHash *= FNV32_Prime;
	}
	return *this;
}

zenHash32& zenHash32::Append(const wchar_t* _zString)
{		
	while( *_zString )
	{
		muHash ^= (*_zString) & 0xFF;				
		muHash *= FNV32_Prime;
		++_zString;
		muHash ^= (*_zString >> 8) & 0xFF;
		muHash *= FNV32_Prime;
		++_zString;
	}
	return *this;
}

zenHash32& zenHash32::Append(const void* _pData, zenUInt _uSize)
{	
	const zenU8* pDataCur = static_cast<const zenU8*>(_pData);	
	const zenU8* pDataEnd = pDataCur + _uSize;
	while( pDataCur < pDataEnd )
	{
		muHash ^= *pDataCur++;
		muHash *= FNV32_Prime;
	}
	return *this;
}
		
zenHash32& zenHash32::Append( zenHash32 _hHashToAdd)
{	
	zenU8* pData = reinterpret_cast<zenU8*>(&_hHashToAdd);
	muHash ^= pData[0];	muHash *= FNV32_Prime;
	muHash ^= pData[1];	muHash *= FNV32_Prime;
	muHash ^= pData[2];	muHash *= FNV32_Prime;
	muHash ^= pData[3];	muHash *= FNV32_Prime;
	return *this;
}

bool zenHash32::operator==(const zenHash32& _hCmpr)	
{ 
	return muHash == _hCmpr.muHash; 
}

bool zenHash32::operator!=(const zenHash32& _hCmpr)	
{ 
	return muHash != _hCmpr.muHash; 
}

zenHash32& zenHash32::operator=(const zenHash32& _hCopy)	
{ 
	muHash = _hCopy.muHash; return *this; 
}

zenHash32& zenHash32::operator=(const zenU32& _uCopy)		
{ 
	muHash = _uCopy; return *this; 
}

zenHash32::operator const zenU32&()const				
{ 
	return muHash; 
}

zenHash32::zenHash32()						
: muHash(FNV32_Seed){}

zenHash32::zenHash32(const zenHash32& _hCopy)	
: muHash(_hCopy.muHash){}

zenHash32::zenHash32(const zenU32& _uCopy)				
: muHash(_uCopy){}

zenHash32::zenHash32(const zenS32& _iCopy)				
: muHash(_iCopy){}

zenHash32::zenHash32(ConstCharWrapper _zStr)		
: muHash(FNV32_Seed)
{ 
	Append(_zStr.mzStr); 
}

zenHash32::zenHash32(char* _zStr)					
: muHash(FNV32_Seed)
{ 
	Append(_zStr); 
}

zenHash32::zenHash32(const wchar_t* _zStr)			
: muHash(FNV32_Seed)
{ 
	Append(_zStr); 
}

zenHash32::zenHash32(const void* _pData, zenUInt _uSize)
: muHash(FNV32_Seed)
{ 
	Append(_pData, _uSize); 
}
		
//=================================================================================================
// HASH64
//=================================================================================================		
zenHash64& zenHash64::Append(const char* _zString)
{		
	while( *_zString )
	{
		muHash ^= *_zString++;
		muHash *= FNV64_Prime;
	}
	return *this;
}

zenHash64& zenHash64::Append(const wchar_t* _zString)
{		
	while( *_zString )
	{
		muHash ^= (*_zString) & 0xFF;				
		muHash *= FNV64_Prime;
		++_zString;
		muHash ^= (*_zString >> 8) & 0xFF;
		muHash *= FNV64_Prime;
		++_zString;
	}
	return *this;
}

zenHash64& zenHash64::Append(const zenU8* _pData, zenUInt _uSize )
{		
	const zenU8* pDataEnd = _pData+_uSize;
	while( _pData < pDataEnd )
	{
		muHash ^= *_pData++;
		muHash *= FNV64_Prime;
	}
	return *this;
}

zenHash64& zenHash64::Append(zenHash64 _hHashToAdd)
{	
	zenU8* pData = reinterpret_cast<zenU8*>(&_hHashToAdd);
	muHash ^= pData[0];	muHash *= FNV64_Prime;
	muHash ^= pData[1];	muHash *= FNV64_Prime;
	muHash ^= pData[2];	muHash *= FNV64_Prime;
	muHash ^= pData[3];	muHash *= FNV64_Prime;
	muHash ^= pData[4];	muHash *= FNV64_Prime;
	muHash ^= pData[5];	muHash *= FNV64_Prime;
	muHash ^= pData[6];	muHash *= FNV64_Prime;
	muHash ^= pData[7];	muHash *= FNV64_Prime;
	return *this;
}

bool zenHash64::operator==(const zenHash64& _hCmpr)	
{ 
	return muHash == _hCmpr.muHash; 
}

bool zenHash64::operator!=(const zenHash64& _hCmpr)	
{ 
	return muHash != _hCmpr.muHash; 
}

zenHash64& zenHash64::operator=(const zenHash64& _hCopy)	
{ 
	muHash = _hCopy.muHash; return *this; 
}

zenHash64& zenHash64::operator=(const zenU64& _uCopy)		
{ 
	muHash = _uCopy; return *this; 
}

zenHash64::operator const zenU64&()const				
{ 
	return muHash; 
}

zenHash64::zenHash64()								
: muHash(FNV64_Seed)
{}

zenHash64::zenHash64(const zenHash64& _hCopy)			
: muHash(_hCopy.muHash)
{}

zenHash64::zenHash64(const zenU64& _uCopy)				
: muHash(_uCopy)
{}

zenHash64::zenHash64(const zenS64& _iCopy)				
: muHash(_iCopy)
{}

zenHash64::zenHash64(ConstCharWrapper _zStr)		
: muHash(FNV64_Seed)
{ 
	Append(_zStr.mzStr);	
}

zenHash64::zenHash64(char* _zStr)					
: muHash(FNV64_Seed)
{ 
	Append(_zStr);			
}

zenHash64::zenHash64(wchar_t* _zStr)				
: muHash(FNV64_Seed)
{ 
	Append(_zStr);			
}

zenHash64::zenHash64(const zenU8* _pData, zenUInt _uSize)	
: muHash(FNV64_Seed)
{ 
	Append(_pData, _uSize); 
}

//=================================================================================================
// STRING HASH
//=================================================================================================		
template<class THashType>
zenStringHash<THashType>::zenStringHash(const char* _zString)
: mzName(_zString)
, mhName(_zString)
{
}

template<class THashType>
zenUInt zenStringHash<THashType>::Find( THashType _HashName, const zenStringHash* _pFirst, zenUInt _uCount )
{
	for(zenUInt idx(0); idx<_uCount; ++idx )
		if( _pFirst[idx].mhName == _HashName )
			return idx;
	return _uCount;
}

}} //namespace zen, Type 
