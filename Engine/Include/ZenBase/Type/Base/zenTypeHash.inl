#include <string.h>

namespace zen { namespace awtype {

//=================================================================================================
// HASH32
//=================================================================================================
awHash32& awHash32::Append(const char* _zString)
{		
	while( *_zString )
	{
		muHash ^= *_zString++;
		muHash *= FNV32_Prime;
	}
	return *this;
}

awHash32& awHash32::Append(const wchar_t* _zString)
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

awHash32& awHash32::Append(const void* _pData, awUInt _uSize)
{	
	const awU8* pDataCur = static_cast<const awU8*>(_pData);	
	const awU8* pDataEnd = pDataCur + _uSize;
	while( pDataCur < pDataEnd )
	{
		muHash ^= *pDataCur++;
		muHash *= FNV32_Prime;
	}
	return *this;
}
		
awHash32& awHash32::Append( awHash32 _hHashToAdd)
{	
	awU8* pData = reinterpret_cast<awU8*>(&_hHashToAdd);
	muHash ^= pData[0];	muHash *= FNV32_Prime;
	muHash ^= pData[1];	muHash *= FNV32_Prime;
	muHash ^= pData[2];	muHash *= FNV32_Prime;
	muHash ^= pData[3];	muHash *= FNV32_Prime;
	return *this;
}

bool awHash32::operator==(const awHash32& _hCmpr)	
{ 
	return muHash == _hCmpr.muHash; 
}

bool awHash32::operator!=(const awHash32& _hCmpr)	
{ 
	return muHash != _hCmpr.muHash; 
}

awHash32& awHash32::operator=(const awHash32& _hCopy)	
{ 
	muHash = _hCopy.muHash; return *this; 
}

awHash32& awHash32::operator=(const awU32& _uCopy)		
{ 
	muHash = _uCopy; return *this; 
}

awHash32::operator const awU32&()const				
{ 
	return muHash; 
}

awHash32::awHash32()						
: muHash(FNV32_Seed){}

awHash32::awHash32(const awHash32& _hCopy)	
: muHash(_hCopy.muHash){}

awHash32::awHash32(const awU32& _uCopy)				
: muHash(_uCopy){}

awHash32::awHash32(const awS32& _iCopy)				
: muHash(_iCopy){}

awHash32::awHash32(ConstCharWrapper _zStr)		
: muHash(FNV32_Seed)
{ 
	Append(_zStr.mzStr); 
}

awHash32::awHash32(char* _zStr)					
: muHash(FNV32_Seed)
{ 
	Append(_zStr); 
}

awHash32::awHash32(const wchar_t* _zStr)			
: muHash(FNV32_Seed)
{ 
	Append(_zStr); 
}

awHash32::awHash32(const void* _pData, awUInt _uSize)
: muHash(FNV32_Seed)
{ 
	Append(_pData, _uSize); 
}
		
//=================================================================================================
// HASH64
//=================================================================================================		
awHash64& awHash64::Append(const char* _zString)
{		
	while( *_zString )
	{
		muHash ^= *_zString++;
		muHash *= FNV64_Prime;
	}
	return *this;
}

awHash64& awHash64::Append(const wchar_t* _zString)
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

awHash64& awHash64::Append(const awU8* _pData, awUInt _uSize )
{		
	const awU8* pDataEnd = _pData+_uSize;
	while( _pData < pDataEnd )
	{
		muHash ^= *_pData++;
		muHash *= FNV64_Prime;
	}
	return *this;
}

awHash64& awHash64::Append(awHash64 _hHashToAdd)
{	
	awU8* pData = reinterpret_cast<awU8*>(&_hHashToAdd);
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

bool awHash64::operator==(const awHash64& _hCmpr)	
{ 
	return muHash == _hCmpr.muHash; 
}

bool awHash64::operator!=(const awHash64& _hCmpr)	
{ 
	return muHash != _hCmpr.muHash; 
}

awHash64& awHash64::operator=(const awHash64& _hCopy)	
{ 
	muHash = _hCopy.muHash; return *this; 
}

awHash64& awHash64::operator=(const awU64& _uCopy)		
{ 
	muHash = _uCopy; return *this; 
}

awHash64::operator const awU64&()const				
{ 
	return muHash; 
}

awHash64::awHash64()								
: muHash(FNV64_Seed)
{}

awHash64::awHash64(const awHash64& _hCopy)			
: muHash(_hCopy.muHash)
{}

awHash64::awHash64(const awU64& _uCopy)				
: muHash(_uCopy)
{}

awHash64::awHash64(const awS64& _iCopy)				
: muHash(_iCopy)
{}

awHash64::awHash64(ConstCharWrapper _zStr)		
: muHash(FNV64_Seed)
{ 
	Append(_zStr.mzStr);	
}

awHash64::awHash64(char* _zStr)					
: muHash(FNV64_Seed)
{ 
	Append(_zStr);			
}

awHash64::awHash64(wchar_t* _zStr)				
: muHash(FNV64_Seed)
{ 
	Append(_zStr);			
}

awHash64::awHash64(const awU8* _pData, awUInt _uSize)	
: muHash(FNV64_Seed)
{ 
	Append(_pData, _uSize); 
}

//=================================================================================================
// STRING HASH
//=================================================================================================		
template<class THashType>
awStringHash<THashType>::awStringHash(const char* _zString)
: mzName(_zString)
, mhName(_zString)
{
}

template<class THashType>
awUInt awStringHash<THashType>::Find( THashType _HashName, const awStringHash* _pFirst, awUInt _uCount )
{
	for(awUInt idx(0); idx<_uCount; ++idx )
		if( _pFirst[idx].mhName == _HashName )
			return idx;
	return _uCount;
}

}} //namespace zen, Type 
