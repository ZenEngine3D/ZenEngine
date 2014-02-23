#include "libZenCore.h"
#include <stdio.h> //! @note Temporary

namespace EExp
{		

SerializerImportMem::SerializerImportMem(awU8* apBuffer, awUInt auBufferSize)
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

bool SerializerImportMem::Serialize(awU8& auValue)
{
	if( mpMemoryCur + sizeof(auValue) >= mpMemoryEnd )
		return false;
			
	auValue		= *(awU8*)mpMemoryCur;
	mpMemoryCur += sizeof(auValue);
	return true;
}

bool SerializerImportMem::Serialize(awU16& auValue)
{
	if( mpMemoryCur + sizeof(auValue) >= mpMemoryEnd )
		return false;

	auValue		= *(awU16*)mpMemoryCur;
	mpMemoryCur += sizeof(auValue);
	return true;
}

bool SerializerImportMem::Serialize(awU32& auValue)
{
	if( mpMemoryCur + sizeof(auValue) >= mpMemoryEnd )
		return false;

	auValue		= *(awU32*)mpMemoryCur;
	mpMemoryCur += sizeof(auValue);
	return true;
}

bool SerializerImportMem::Serialize(awU64& auValue)
{
	if( mpMemoryCur + sizeof(auValue) >= mpMemoryEnd )
		return false;

	auValue		= *(awU64*)mpMemoryCur;
	mpMemoryCur += sizeof(auValue);
	return true;
}

bool SerializerImportMem::Serialize(awS8& aiValue)
{
	if( mpMemoryCur + sizeof(aiValue) >= mpMemoryEnd )
		return false;

	aiValue		= *(awS8*)mpMemoryCur;
	mpMemoryCur += sizeof(aiValue);
	return true;
}

bool SerializerImportMem::Serialize(awS16& aiValue)
{
	if( mpMemoryCur + sizeof(aiValue) >= mpMemoryEnd )
		return false;

	aiValue		= *(awS16*)mpMemoryCur;
	mpMemoryCur += sizeof(aiValue);
	return true;
}

bool SerializerImportMem::Serialize(awS32& aiValue)
{
	if( mpMemoryCur + sizeof(aiValue) >= mpMemoryEnd )
		return false;

	aiValue		= *(awS32*)mpMemoryCur;
	mpMemoryCur += sizeof(aiValue);
	return true;
}

bool SerializerImportMem::Serialize(awS64& aiValue)
{
	if( mpMemoryCur + sizeof(aiValue) >= mpMemoryEnd )
		return false;

	aiValue		= *(awS64*)mpMemoryCur;
	mpMemoryCur += sizeof(aiValue);
	return true;
}

bool SerializerImportMem::Serialize(awArrayBase<awU8>& _aValues)
{
	awU32 uSize;
	if( Serialize(uSize) )
	{
		if( mpMemoryCur + uSize >= mpMemoryEnd )
			return false;

		_aValues.SetCount(uSize/sizeof(awU8));
		zenMem::Copy(_aValues.First(), mpMemoryCur, uSize );
		mpMemoryCur		+= uSize;
		return true;
	}
	return false;	
}

bool SerializerImportMem::Serialize(awArrayBase<awU16>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(awArrayBase<awU32>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(awArrayBase<awU64>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(awArrayBase<awS8>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(awArrayBase<awS16>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(awArrayBase<awS32>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(awArrayBase<awS64>& _aValues)
{
	return true;
}

}

