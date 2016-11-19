namespace zen { namespace zenType {

void zArrayBits::Resize(zUInt _uIndexCount)
{
	zenAssertMsg(_uIndexCount != kuInvalid, "Invalid count, are you trying to set a invalid index (-1)?");
	zUInt uNeededArrayCount = zenMath::RoundUp<zUInt>(_uIndexCount, sizeof(zUInt))/sizeof(zUInt);	
	if( uNeededArrayCount != muDataArrayCount )
	{
		zUInt* pNewData		= zenNewDefault zUInt[uNeededArrayCount];
		zenMem::CopyRaw(pNewData, mpDataArray, zenMath::Min(muDataArrayCount, uNeededArrayCount)*sizeof(zUInt));
		if( uNeededArrayCount > muDataArrayCount)
			zenMem::Zero(&pNewData[muDataArrayCount], (uNeededArrayCount-muDataArrayCount)*sizeof(zUInt) );
		zenDelArray(mpDataArray);
		mpDataArray			= pNewData;
		muDataArrayCount	= uNeededArrayCount;
	}
}

void zArrayBits::GrowIfNeeded(zUInt _uIndexCount)
{
	if( _uIndexCount != kuInvalid )
	{
		zUInt uNeededSize = zenMath::RoundUp<zUInt>(_uIndexCount, sizeof(zUInt))/sizeof(zUInt);	
		if( uNeededSize > muDataArrayCount )
			Resize( _uIndexCount );
	}
}

zArrayBits::zArrayBits()
{
}

zArrayBits::zArrayBits(zU32 _uCount)
{
	Resize(_uCount);
}

zArrayBits::zArrayBits(std::initializer_list<bool> _Entries)
{
	Resize(_Entries.size());
	const bool* pValues = _Entries.begin();
	for( zUInt idx(0); idx < _Entries.size(); ++idx)
	{
		zUInt uArrayIndex			= idx / sizeof(zUInt);
		zUInt uBitIndex				= idx % sizeof(zUInt);
		mpDataArray[uArrayIndex]	|= pValues[idx] ? 1<<uBitIndex : 0;;
	}
}

zArrayBits::zArrayBits(const zArrayBits& _Copy)
{
	muDataArrayCount	= _Copy.muDataArrayCount;
	mpDataArray			= zenNewDefault zUInt[muDataArrayCount];
	zenMem::CopyRaw( mpDataArray, _Copy.mpDataArray, muDataArrayCount);
}

zArrayBits::~zArrayBits()
{
	zenDelArray(mpDataArray);
}

bool zArrayBits::operator[](zUInt _uIndex)const
{
	zUInt uArrayIndex	= _uIndex / sizeof(zUInt);
	zUInt uBitIndex		= _uIndex % sizeof(zUInt);
	if( uArrayIndex < muDataArrayCount )
		return (mpDataArray[uArrayIndex] & (zUInt(1)<<uBitIndex)) != 0;
	
	return false;
}

zUInt zArrayBits::GetFirstTrue()
{
	zUInt idx(0);
	while( idx < muDataArrayCount )
	{
		if( mpDataArray[idx] != 0 )
			return idx*sizeof(zUInt) + zenMath::BitsScan(mpDataArray[idx]);
		++idx;
	}
	return kuInvalid;	
}

zUInt zArrayBits::GetFirstFalse()
{
	zUInt idx(0);
	while( idx < muDataArrayCount )
	{
		if( (~mpDataArray[idx]) != 0 )
			return idx*sizeof(zUInt) + zenMath::BitsScan(~mpDataArray[idx]);
		++idx;
	}
	return kuInvalid;
}

zUInt zArrayBits::GetLastTrue()
{
}

zUInt zArrayBits::GetLastFalse()
{
}

zUInt zArrayBits::AddIndexTrue()
{
	zUInt uIndex = GetFirstFalse();
	if( uIndex == (zUInt)kuInvalid )
	{
		Resize( muDataArrayCount*sizeof(zUInt) + 1);
		uIndex = (muDataArrayCount-1)*sizeof(zUInt);
	}
	Set(uIndex, true);
	return uIndex;
}

zUInt zArrayBits::AddIndexFalse()
{
	zUInt uIndex = GetFirstTrue();
	if( uIndex == kuInvalid )
	{
		Resize( muDataArrayCount*sizeof(zUInt) + 1);
		uIndex = (muDataArrayCount-1)*sizeof(zUInt);
	}
	Set(uIndex, false);
	return uIndex;
}

bool zArrayBits::Toggle(zUInt _uIndex)
{
	GrowIfNeeded( _uIndex );
	zUInt uArrayIndex			= _uIndex / sizeof(zUInt);
	zUInt uBitIndex				= _uIndex % sizeof(zUInt);
	mpDataArray[uArrayIndex]	^= (zUInt(1)<<uBitIndex);
	return (mpDataArray[uArrayIndex] & (zUInt(1)<<uBitIndex)) != 0;
}

void zArrayBits::Set(zUInt _uIndex, bool _bValue)
{	
	GrowIfNeeded( _uIndex );
	zUInt uArrayIndex			= _uIndex / sizeof(zUInt);
	zUInt uBitIndex				= _uIndex % sizeof(zUInt);
	mpDataArray[uArrayIndex]	&= ~(1<<uBitIndex);
	mpDataArray[uArrayIndex]	|= (_bValue<<uBitIndex);
}

void zArrayBits::SetRange(zUInt _uFirst, zUInt _uLast, bool _bValue)
{
	GrowIfNeeded(_uLast);
	for(zUInt idx=_uFirst; idx<=_uLast; ++idx)
	{
		zUInt uArrayIndex			= idx / sizeof(zUInt);
		zUInt uBitIndex				= idx % sizeof(zUInt);
		mpDataArray[uArrayIndex]	&= ~(1<<uBitIndex);
		mpDataArray[uArrayIndex]	|= (_bValue<<uBitIndex);
	}
}

void zArrayBits::Reset()
{
	zenDelArray(mpDataArray);
	muDataArrayCount = 0;
}


} } //namespace zen, Type
