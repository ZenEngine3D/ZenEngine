#include <string.h>

namespace zen { namespace zenType {

//=================================================================================================
// HASH32
//=================================================================================================
zHash32& zHash32::Append(const char* _zString)
{		
	while( *_zString )
	{
		muHash ^= *_zString++;
		muHash *= FNV32_Prime;
	}
	return *this;
}

zHash32& zHash32::Append(const wchar_t* _zString)
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

zHash32& zHash32::Append(const void* _pData, zUInt _uSize)
{	
	const zU8* pDataCur = reinterpret_cast<const zU8*>(_pData);	
	const zU8* pDataEnd = pDataCur + _uSize;
	while( pDataCur < pDataEnd )
	{
		muHash ^= *pDataCur++;
		muHash *= FNV32_Prime;
	}
	return *this;
}
		
/*
zHash32& zHash32::Append( zHash32 _hHashToAdd)
{	
	zU8* pData = reinterpret_cast<zU8*>(&_hHashToAdd);
	muHash ^= pData[0];	muHash *= FNV32_Prime;
	muHash ^= pData[1];	muHash *= FNV32_Prime;
	muHash ^= pData[2];	muHash *= FNV32_Prime;
	muHash ^= pData[3];	muHash *= FNV32_Prime;
	return *this;
}
*/

bool zHash32::operator==(const zHash32& _hCmpr)	
{ 
	return muHash == _hCmpr.muHash; 
}

bool zHash32::operator!=(const zHash32& _hCmpr)	
{ 
	return muHash != _hCmpr.muHash; 
}

zHash32& zHash32::operator=(const zHash32& _hCopy)	
{ 
	muHash = _hCopy.muHash; return *this; 
}

zHash32& zHash32::operator=(const zU32& _uCopy)		
{ 
	muHash = _uCopy; return *this; 
}

zHash32::operator const zU32&()const				
{ 
	return muHash; 
}

zHash32::zHash32()						
: muHash(FNV32_Seed){}

zHash32::zHash32(const zU32& _uCopy)				
: muHash(_uCopy){}

zHash32::zHash32(const zI32& _iCopy)				
: muHash(_iCopy){}

zHash32::zHash32(ConstCharWrapper _zStr)		
: muHash(FNV32_Seed)
{ 
	Append(_zStr.mzStr); 
}

zHash32::zHash32(char* _zStr)					
: muHash(FNV32_Seed)
{ 
	Append(_zStr); 
}

zHash32::zHash32(const wchar_t* _zStr)			
: muHash(FNV32_Seed)
{ 
	Append(_zStr); 
}

zHash32::zHash32(const void* _pData, zUInt _uSize)
: muHash(FNV32_Seed)
{ 
	Append(_pData, _uSize); 
}
		
//=================================================================================================
// HASH64
//=================================================================================================		
zHash64& zHash64::Append(const char* _zString)
{		
	while( *_zString )
	{
		muHash ^= *_zString++;
		muHash *= FNV64_Prime;
	}
	return *this;
}

zHash64& zHash64::Append(const wchar_t* _zString)
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

zHash64& zHash64::Append(const void* _pData, zUInt _uSize )
{		
	const zU8* pDataCur	= reinterpret_cast<const zU8*>(_pData);
	const zU8* pDataEnd = pDataCur+_uSize;
	while( pDataCur < pDataEnd )
	{
		muHash ^= *pDataCur++;
		muHash *= FNV64_Prime;
	}
	return *this;
}
/*
zHash64& zHash64::Append(zHash64 _hHashToAdd)
{	
	zU8* pData = reinterpret_cast<zU8*>(&_hHashToAdd);
	muHash ^= pData[0];	muHash *= FNV64_Prime;
	muHash ^= pData[1];	muHash *= FNV64_Prime;
	muHash ^= pData[2];	muHash *= FNV64_Prime;
	muHash ^= pData[3];	muHash *= FNV64_Prime;
	muHash ^= pData[4];	muHash *= FNV64_Prime;
	muHash ^= pData[5];	muHash *= FNV64_Prime;
	muHash ^= pData[6];	muHash *= FNV64_Prime;
	muHash ^= pData[7];	muHash *= FNV64_Prime;
	return *this;
}*/

bool zHash64::operator==(const zHash64& _hCmpr)	
{ 
	return muHash == _hCmpr.muHash; 
}

bool zHash64::operator!=(const zHash64& _hCmpr)	
{ 
	return muHash != _hCmpr.muHash; 
}

zHash64& zHash64::operator=(const zHash64& _hCopy)	
{ 
	muHash = _hCopy.muHash; return *this; 
}

zHash64& zHash64::operator=(const zU64& _uCopy)		
{ 
	muHash = _uCopy; return *this; 
}

zHash64::operator const zU64&()const				
{ 
	return muHash; 
}

zHash64::zHash64()								
: muHash(FNV64_Seed)
{}

zHash64::zHash64(const zHash64& _hCopy)			
: muHash(_hCopy.muHash)
{}

zHash64::zHash64(const zU64& _uCopy)				
: muHash(_uCopy)
{}

zHash64::zHash64(const zI64& _iCopy)				
: muHash(_iCopy)
{}

zHash64::zHash64(ConstCharWrapper _zStr)		
: muHash(FNV64_Seed)
{ 
	Append(_zStr.mzStr);	
}

zHash64::zHash64(char* _zStr)					
: muHash(FNV64_Seed)
{ 
	Append(_zStr);			
}

zHash64::zHash64(const wchar_t* _zStr)				
: muHash(FNV64_Seed)
{ 
	Append(_zStr);			
}

zHash64::zHash64(const void* _pData, zUInt _uSize)	
: muHash(FNV64_Seed)
{ 
	Append(_pData, _uSize); 
}

}} //namespace zen, Type 
