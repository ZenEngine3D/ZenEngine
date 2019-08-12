namespace zen { namespace zenType {

zString::zString()
{
}

zString::zString(const zString& _zString)	
: maChar(_zString.maChar)
{
}

zString::zString(const char* _zString)
{
	if( _zString != nullptr && _zString[0] != 0)
		maChar.Copy(_zString, zUInt(strlen(_zString)+1));
}

zString& zString::operator=(const zString& _zString)
{
	maChar = _zString.maChar;
	return *this;
}
	
zString& zString::operator=(const char* _zString)
{	
	maChar.Copy(_zString, zUInt(strlen(_zString)+1));
	return *this;
}

zString& zString::operator+=(const zString& _zString)
{		
	return *this += (const char*)_zString;
}

zString& zString::operator+=(const char* _zString)
{
	if( maChar.empty() )
		return operator=(_zString);

	zUInt uLen		= static_cast<zUInt>(strlen(_zString));
	zUInt uOldCount	= maChar.size();
	maChar.resize(uOldCount + uLen);
	zenMem::Copy(&maChar[uOldCount-1], _zString, uLen+1);
	return *this;
}

bool zString::operator==(const char* _zCmpString)const
{
	const char* pLocalCur = &maChar.front();
	const char* pExternCur = _zCmpString;
	while( *pLocalCur && (*pLocalCur==*pExternCur) )
	{
		++pLocalCur;
		++pExternCur;
	}
	return *pLocalCur == *pExternCur;
}

bool zString::operator==(const zString& _zString)const
{	
	return	(maChar.size() == _zString.maChar.size()) &&
			(*this == &_zString.maChar.front());
}

bool zString::operator!=(const zString& _zString)const
{
	return !(*this==_zString);
}

bool zString::operator!=(const char* _zString)const
{
	return !( *this==_zString);
}

zUInt zString::Len()const
{
	return maChar.empty() ? 0 : maChar.size()-1;
}

zString::operator const char*() const 
{ 
	return maChar.empty() ? "" : &maChar.front();
}

const char* zString::Last(zUInt index)const
{
	return maChar.size() <= 1 ? nullptr : &maChar[ zenMath::Max<zInt>(0, maChar.size()-2-index) ];
}

void zString::Split(char _Separator, zArrayDyn<zString>& _aStringOut, zUInt _uAdditionalArraySize )const
{
	if( !maChar.empty() )
		Split(&maChar.front(), _Separator, _aStringOut, _uAdditionalArraySize);
}

void zString::Merge(const zArray<zString>& _aStrings, char _Separator, zInt _iMaxEntry)
{
	Merge(_aStrings, _Separator, *this, _iMaxEntry);
}

//=================================================================================================
// STRING HASH
//=================================================================================================		
template<class THashType>
zStringHash<THashType>::zStringHash(const char* _zString)
: mzName(_zString)
, mhName(_zString)
{
}

template<class THashType>
const zStringHash<THashType>& zStringHash<THashType>::operator=(const zStringHash& _Copy)
{
	mhName = _Copy.mhName;
	mzName = _Copy.mzName;
	return *this;
}

template<class THashType>
bool zStringHash<THashType>::operator==(const zStringHash& _Compare)const
{
	return mhName == _Compare.mhName;
}

template<class THashType>
bool zStringHash<THashType>::operator!=(const THashType& _Compare)const
{
	return mhName != _Compare.mhName;
}

template<class THashType>
bool zStringHash<THashType>::operator==(const THashType& _hCompare)const
{
	return mhName == _hCompare;
}

template<class THashType>
bool zStringHash<THashType>::operator!=(const zStringHash& _hCompare)const
{
	return mhName != _hCompare;
}

template<class THashType>
zUInt zStringHash<THashType>::Find( THashType _HashName, const zStringHash* _pFirst, zUInt _uCount )
{
	for(zUInt idx(0); idx<_uCount; ++idx )
		if( _pFirst[idx].mhName == _HashName )
			return idx;
	return _uCount;
}


}}  //namespace zen, Type 


