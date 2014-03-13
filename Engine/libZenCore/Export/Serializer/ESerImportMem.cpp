#include "libZenCore.h"
#include <stdio.h> //! @note Temporary

namespace EExp
{		

SerializerImportMem::SerializerImportMem(zU8* apBuffer, zUInt auBufferSize)
:	mpMemoryStart(apBuffer)
,	mpMemoryCur(apBuffer)
,	mpMemoryEnd(apBuffer+auBufferSize)
,	mpMemoryItemStart(NULL)
{
}

bool SerializerImportMem::ItemStarted(EExp::SerialItem& aItem)
{
	if( ISerializerImporter::ItemStarted(aItem) )
	{
		mpMemoryItemStart = mpMemoryCur;
		return true;
	}
	return false;
}

bool SerializerImportMem::ItemSkip(EExp::SerialItem& aItem)
{
	mpMemoryCur = mpMemoryItemStart + aItem.muSize;
	return true;
}

bool SerializerImportMem::Serialize(zU8& auValue)
{
	if( mpMemoryCur + sizeof(auValue) >= mpMemoryEnd )
		return false;
			
	auValue		= *(zU8*)mpMemoryCur;
	mpMemoryCur += sizeof(auValue);
	return true;
}

bool SerializerImportMem::Serialize(zU16& auValue)
{
	if( mpMemoryCur + sizeof(auValue) >= mpMemoryEnd )
		return false;

	auValue		= *(zU16*)mpMemoryCur;
	mpMemoryCur += sizeof(auValue);
	return true;
}

bool SerializerImportMem::Serialize(zU32& auValue)
{
	if( mpMemoryCur + sizeof(auValue) >= mpMemoryEnd )
		return false;

	auValue		= *(zU32*)mpMemoryCur;
	mpMemoryCur += sizeof(auValue);
	return true;
}

bool SerializerImportMem::Serialize(zU64& auValue)
{
	if( mpMemoryCur + sizeof(auValue) >= mpMemoryEnd )
		return false;

	auValue		= *(zU64*)mpMemoryCur;
	mpMemoryCur += sizeof(auValue);
	return true;
}

bool SerializerImportMem::Serialize(zS8& aiValue)
{
	if( mpMemoryCur + sizeof(aiValue) >= mpMemoryEnd )
		return false;

	aiValue		= *(zS8*)mpMemoryCur;
	mpMemoryCur += sizeof(aiValue);
	return true;
}

bool SerializerImportMem::Serialize(zS16& aiValue)
{
	if( mpMemoryCur + sizeof(aiValue) >= mpMemoryEnd )
		return false;

	aiValue		= *(zS16*)mpMemoryCur;
	mpMemoryCur += sizeof(aiValue);
	return true;
}

bool SerializerImportMem::Serialize(zS32& aiValue)
{
	if( mpMemoryCur + sizeof(aiValue) >= mpMemoryEnd )
		return false;

	aiValue		= *(zS32*)mpMemoryCur;
	mpMemoryCur += sizeof(aiValue);
	return true;
}

bool SerializerImportMem::Serialize(zS64& aiValue)
{
	if( mpMemoryCur + sizeof(aiValue) >= mpMemoryEnd )
		return false;

	aiValue		= *(zS64*)mpMemoryCur;
	mpMemoryCur += sizeof(aiValue);
	return true;
}

bool SerializerImportMem::Serialize(zArrayBase<zU8>& _aValues)
{
	zU32 uSize;
	if( Serialize(uSize) )
	{
		if( mpMemoryCur + uSize >= mpMemoryEnd )
			return false;

		_aValues.SetCount(uSize/sizeof(zU8));
		zenMem::Copy(_aValues.First(), mpMemoryCur, uSize );
		mpMemoryCur		+= uSize;
		return true;
	}
	return false;	
}

bool SerializerImportMem::Serialize(zArrayBase<zU16>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(zArrayBase<zU32>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(zArrayBase<zU64>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(zArrayBase<zS8>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(zArrayBase<zS16>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(zArrayBase<zS32>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(zArrayBase<zS64>& _aValues)
{
	return true;
}

}

