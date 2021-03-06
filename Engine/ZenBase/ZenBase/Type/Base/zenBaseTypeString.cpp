#include "zbBase.h"

namespace zen { namespace zenType {

zString& zString::Copy(const char* _zString, zUInt _uCount)
{
	zUInt len = zenMath::Min(_uCount, zUInt(strlen(_zString) )) + 1;
	maChar.Copy(_zString, len);
	maChar.back() = 0;
	return *this;
}

void zString::Split(const char* _zString, char _Separator, zArrayDyn<zString>& _aStringOut, zUInt _uAdditionalArraySize )
{
	zenAssert(_zString);	
	zUInt uGroupEnd[128];
	zUInt uGroupCount(0);
	zUInt uStrPos(0);

	// Find all item delimited by a separator
	for(; _zString[uStrPos] != 0; ++uStrPos)
	{
		if( _zString[uStrPos] == _Separator )
		{
			uGroupEnd[uGroupCount++] = uStrPos;
			zenAssert( uGroupCount+1<zenArrayCount(uGroupEnd) );
		}
	}
	if( uStrPos > 0 && _zString[uStrPos-1] != _Separator )
		uGroupEnd[uGroupCount++] = uStrPos;

	// Copy each item to destination array
	uStrPos	= 0;
	_aStringOut.resize(uGroupCount+_uAdditionalArraySize);
	for(zUInt idx(0); idx<uGroupCount; ++idx)
	{
		zUInt len = uGroupEnd[idx]-uStrPos;
		_aStringOut[idx].Copy(&_zString[uStrPos], len);
		uStrPos = uGroupEnd[idx]+1;
	}
}

void zString::Merge(const zArray<zString>& _aStrings, char _Separator, zString& _zStringOut, zInt _iMaxEntry)
{
	// Count string size needed
	zUInt len(0), pos(0), sepSize(_Separator != 0 ? 1 : 0);	
	_iMaxEntry = (_iMaxEntry <= 0) ? zenMath::Max<zInt>(0, _aStrings.size() + _iMaxEntry) : zenMath::Min<zInt>(_iMaxEntry, _aStrings.size());	
	for(zInt idx(0); idx<_iMaxEntry; ++idx)
	{
		len += _aStrings[idx].Len()+sepSize;
	}
	
	// Copy each array element in the string
	if( len > 0 )
	{
		_zStringOut.maChar.resize(len+1);
		len = 0;
		for(zInt idx(0); idx<_iMaxEntry; ++idx)
		{
			len = _aStrings[idx].Len();
			zenMem::Copy( &_zStringOut.maChar[pos], (const char*)_aStrings[idx], len);
			pos += len;
			if( sepSize > 0 )
			{
				_zStringOut.maChar[pos] = _Separator;
				pos += sepSize;
			}		
		}
		_zStringOut.maChar[sepSize ? pos-1 : pos] = 0;
	}
	else
	{
		_zStringOut = "";
	}
}
}}  //namespace zen, Type 
