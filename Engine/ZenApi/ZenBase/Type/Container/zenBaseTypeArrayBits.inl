namespace zen { namespace zenType {

void zArrayBits::Resize(zUInt _uBitCount)
{
	zenAssertMsg(_uBitCount != kuInvalid, "Invalid count, are you trying to set a invalid index (-1)?");
	zUInt uNeededArrayCount = zenMath::RoundUp<zUInt>(_uBitCount/8, sizeof(zUInt));	
	if( uNeededArrayCount != muDataArrayCount )
	{
		zUInt* pNewData		= zenNew zUInt[uNeededArrayCount];
		zenMem::CopyRaw(pNewData, mpDataArray, zenMath::Min(muDataArrayCount, uNeededArrayCount)*sizeof(zUInt));
		if( uNeededArrayCount > muDataArrayCount)
			zenMem::Zero(&pNewData[muDataArrayCount], (uNeededArrayCount-muDataArrayCount)*sizeof(zUInt) );
		zenDelArray(mpDataArray);
		mpDataArray			= pNewData;
		muDataArrayCount	= uNeededArrayCount;
	}
}

void zArrayBits::GrowIfNeeded(zUInt _uBitCount)
{
	if( _uBitCount != kuInvalid )
	{
		zUInt uNeededCount = zenMath::RoundUp<zUInt>(_uBitCount/8, sizeof(zUInt));
		if( uNeededCount > muDataArrayCount )
			Resize( uNeededCount*8 );
	}
}

zArrayBits::zArrayBits()
{
}

zArrayBits::zArrayBits(zU32 _uBitCount)
{
	Resize(_uBitCount);
}

zArrayBits::zArrayBits(std::initializer_list<bool> _Entries)
{
	Resize(_Entries.size());
	const bool* pValues = _Entries.begin();
	for( zUInt idx(0); idx < _Entries.size(); ++idx)
	{
		zUInt uArrayIndex			= idx / (sizeof(zUInt)*8);
		zUInt uBitIndex				= idx % (sizeof(zUInt)*8);
		mpDataArray[uArrayIndex]	|= pValues[idx] ? zUInt(1)<<uBitIndex : 0;;
	}
}

zArrayBits::zArrayBits(const zArrayBits& _Copy)
{
	muDataArrayCount	= _Copy.muDataArrayCount;
	mpDataArray			= zenNew zUInt[muDataArrayCount];
	zenMem::CopyRaw( mpDataArray, _Copy.mpDataArray, muDataArrayCount);
}

zArrayBits::~zArrayBits()
{
	zenDelArray(mpDataArray);
}

bool zArrayBits::operator[](zUInt _uBitIndex)const
{
	zUInt uArrayIndex			= _uBitIndex / (sizeof(zUInt)*8);
	zUInt uBitIndex				= _uBitIndex % (sizeof(zUInt)*8);
	if( uArrayIndex < muDataArrayCount )
		return (mpDataArray[uArrayIndex] & (zUInt(1)<<uBitIndex)) != 0;
	
	return false;
}

//! @todo 3 Optim : Use value mip to quickly skip range of values
zUInt zArrayBits::GetNextTrue(zUInt _uStartBitIndex)
{
	zUInt idx(_uStartBitIndex+1);
	while( idx < muDataArrayCount )
	{
		if( mpDataArray[idx] != 0 )
			return (idx*sizeof(zUInt)*8) + zenMath::BitsScan(mpDataArray[idx]);
		++idx;
	}
	return kuInvalid;
}

zUInt zArrayBits::GetNextFalse(zUInt _uStartBitIndex)
{
	zUInt idx(_uStartBitIndex+1);
	while( idx < muDataArrayCount )
	{
		if( mpDataArray[idx] != ~0 )
			return (idx*sizeof(zUInt)*8) + zenMath::BitsScan(~mpDataArray[idx]);
		++idx;
	}
	return kuInvalid;
}

zUInt zArrayBits::AddTrue()
{
	zUInt uBitIndex = GetNextFalse();
	if( uBitIndex == (zUInt)kuInvalid )
	{
		uBitIndex = muDataArrayCount*sizeof(zUInt)*8;
		Resize(uBitIndex);
		
	}
	Set(uBitIndex, true);
	return uBitIndex;
}

zUInt zArrayBits::AddFalse()
{
	zUInt uBitIndex = GetNextTrue();
	if( uBitIndex == kuInvalid )
	{
		uBitIndex = muDataArrayCount*sizeof(zUInt)*8;
		Resize(uBitIndex);
	}
	Set(uBitIndex, false);
	return uBitIndex;
}

zUInt zArrayBits::AddRangeTrue(zUInt _uCount)
{
	zUInt uIndexFalse(kuInvalid), uIndexTrue(kuInvalid);	
	do	
	{
		uIndexFalse = GetNextFalse(uIndexTrue);
		uIndexTrue	= GetNextTrue(uIndexFalse);		
	}
	while( uIndexTrue-uIndexFalse < _uCount && uIndexTrue != kuInvalid );

	if( uIndexTrue == kuInvalid && uIndexFalse+_uCount > muDataArrayCount*sizeof(zUInt)*8 )
	{
		uIndexFalse = muDataArrayCount*sizeof(zUInt)*8;
		Resize(uIndexFalse+_uCount);
	}
	SetRange(uIndexFalse, uIndexFalse+_uCount-1, true);
	return uIndexFalse;
}

zUInt zArrayBits::AddRangeFalse(zUInt _uCount)
{
	zUInt uIndexFalse(kuInvalid), uIndexTrue(kuInvalid);
	do
	{
		uIndexTrue	= GetNextTrue(uIndexFalse);
		uIndexFalse = GetNextFalse(uIndexTrue);		
	}
	while( uIndexTrue-uIndexFalse < _uCount && uIndexFalse != kuInvalid);

	if( uIndexFalse == kuInvalid && uIndexTrue+_uCount > muDataArrayCount*sizeof(zUInt)*8 )
	{
		uIndexTrue = muDataArrayCount*sizeof(zUInt)*8;
		Resize(uIndexTrue+_uCount);
	}
	SetRange(uIndexTrue, uIndexTrue+_uCount-1, false);
	return uIndexTrue;
}

bool zArrayBits::Toggle(zUInt _uBitIndex)
{
	GrowIfNeeded( _uBitIndex );
	zUInt uArrayIndex			= _uBitIndex / (sizeof(zUInt)*8);
	zUInt uBitIndex				= _uBitIndex % (sizeof(zUInt)*8);
	mpDataArray[uArrayIndex]	^= (zUInt(1)<<uBitIndex);
	return (mpDataArray[uArrayIndex] & (zUInt(1)<<uBitIndex)) != 0;
}

void zArrayBits::Set(zUInt _uBitIndex, bool _bValue)
{	
	GrowIfNeeded( _uBitIndex );
	zUInt uArrayIndex			= _uBitIndex / (sizeof(zUInt)*8);
	zUInt uBitIndex				= _uBitIndex % (sizeof(zUInt)*8);
	mpDataArray[uArrayIndex]	&= ~(1<<uBitIndex);
	mpDataArray[uArrayIndex]	|= (_bValue<<uBitIndex);
}

void zArrayBits::SetRange(zUInt _uBitIndexFirst, zUInt _uBitIndexLast, bool _bValue)
{
	GrowIfNeeded(_uBitIndexLast);
	for(zUInt idx=_uBitIndexFirst; idx<=_uBitIndexLast; ++idx)
	{
		zUInt uArrayIndex			= idx / (sizeof(zUInt)*8);
		zUInt uBitIndex				= idx % (sizeof(zUInt)*8);
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
