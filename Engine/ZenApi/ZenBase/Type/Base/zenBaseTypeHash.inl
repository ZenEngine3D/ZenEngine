#include <string.h>

namespace zen { namespace zenType {

//=================================================================================================
// HASH32
//=================================================================================================
zHash32& zHash32::Append(const char*  _zString)
{
	while (*_zString)
	{
		muHash ^= *_zString++;
		muHash *= keFNV32_Prime;
	}
	return *this;
}

zHash32& zHash32::Append(const wchar_t* _zString)
{		
	while( *_zString )
	{
		muHash ^= (*_zString) & 0xFF;				
		muHash *= keFNV32_Prime;
		++_zString;
		muHash ^= (*_zString >> 8) & 0xFF;
		muHash *= keFNV32_Prime;
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
		muHash *= keFNV32_Prime;
	}
	return *this;
}

bool zHash32::operator==(zHash32 _hCmpr)	
{ 
	return muHash == _hCmpr.muHash; 
}

bool zHash32::operator!=(zHash32 _hCmpr)	
{ 
	return muHash != _hCmpr.muHash; 
}

zHash32& zHash32::operator=(zHash32 _hCopy)	
{ 
	muHash = _hCopy.muHash; return *this; 
}

zHash32& zHash32::operator=(zU32 _uCopy)		
{ 
	muHash = _uCopy; return *this; 
}

zHash32::operator const zU32&()const				
{ 
	return muHash; 
}

zHash32::zHash32()						
: muHash((zU32)keFNV32_Seed)
{
}

zHash32::zHash32(zU32 _uCopy)				
: muHash(_uCopy)
{
}

zHash32::zHash32(zI32 _iCopy)				
: muHash(_iCopy)
{
}

template<std::size_t TLen>
zHash32::zHash32(const char(&_zString)[TLen])
: muHash(HashFNV<zU32, keFNV32_Seed, keFNV32_Prime>(_zString))
{
}

zHash32::zHash32(ConstCharWrapper _zStr)		
: muHash((zU32)keFNV32_Seed)
{ 
	Append(_zStr.mzStr); 
}

zHash32::zHash32(char* _zString)
: muHash((zU32)keFNV32_Seed)
{
	Append(_zString);
}

zHash32::zHash32(const wchar_t* _zStr)			
: muHash((zU32)keFNV32_Seed)
{ 
	Append(_zStr); 
}

zHash32::zHash32(const void* _pData, zUInt _uSize)
: muHash((zU32)keFNV32_Seed)
{ 
	Append(_pData, _uSize); 
}
		
//=================================================================================================
// HASH64
//=================================================================================================		
zHash64& zHash64::Append(const char* _zString)
{
	while (*_zString)
	{
		muHash ^= *_zString++;
		muHash *= keFNV64_Prime;
	}
	return *this;
}

zHash64& zHash64::Append(const wchar_t* _zString)
{		
	while( *_zString )
	{
		muHash ^= (*_zString) & 0xFF;				
		muHash *= keFNV64_Prime;
		++_zString;
		muHash ^= (*_zString >> 8) & 0xFF;
		muHash *= keFNV64_Prime;
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
		muHash *= keFNV64_Prime;
	}
	return *this;
}

bool zHash64::operator==(zHash64 _hCmpr)	
{ 
	return muHash == _hCmpr.muHash; 
}

bool zHash64::operator!=(zHash64 _hCmpr)	
{ 
	return muHash != _hCmpr.muHash; 
}

zHash64& zHash64::operator=(const zHash64& _hCopy)	
{ 
	muHash = _hCopy.muHash; 
	return *this; 
}

zHash64& zHash64::operator=(zU64 _uCopy)		
{ 
	muHash = _uCopy; 
	return *this; 
}

zHash64::operator const zU64&()const				
{ 
	return muHash; 
}

zHash64::zHash64()								
: muHash(keFNV64_Seed)
{}

zHash64::zHash64(zU64 _uCopy)				
: muHash(_uCopy)
{}

zHash64::zHash64(zI64 _iCopy)				
: muHash(_iCopy)
{}

template<std::size_t TLen>
zHash64::zHash64(const char(&_zString)[TLen])
: muHash(HashFNV<zU64, keFNV64_Seed, keFNV64_Prime>(_zString))
{}

zHash64::zHash64(ConstCharWrapper _zStr)		
: muHash((zU64)keFNV64_Seed)
{ 
	Append(_zStr.mzStr);	
}

zHash64::zHash64(char* _zString)
: muHash((zU64)keFNV64_Seed)
{
	Append(_zString);
}

zHash64::zHash64(const wchar_t* _zStr)				
: muHash((zU64)keFNV64_Seed)
{ 
	Append(_zStr);			
}

zHash64::zHash64(const void* _pData, zUInt _uSize)	
: muHash((zU64)keFNV64_Seed)
{ 
	Append(_pData, _uSize); 
}

}} //namespace zen, Type 
