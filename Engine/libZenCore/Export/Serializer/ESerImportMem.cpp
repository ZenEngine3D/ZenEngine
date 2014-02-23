#include "libZenCore.h"
#include <stdio.h> //! @note Temporary

namespace EExp
{		

SerializerImportMem::SerializerImportMem(zenU8* apBuffer, zenUInt auBufferSize)
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

bool SerializerImportMem::Serialize(zenU8& auValue)
{
	if( mpMemoryCur + sizeof(auValue) >= mpMemoryEnd )
		return false;
			
	auValue		= *(zenU8*)mpMemoryCur;
	mpMemoryCur += sizeof(auValue);
	return true;
}

bool SerializerImportMem::Serialize(zenU16& auValue)
{
	if( mpMemoryCur + sizeof(auValue) >= mpMemoryEnd )
		return false;

	auValue		= *(zenU16*)mpMemoryCur;
	mpMemoryCur += sizeof(auValue);
	return true;
}

bool SerializerImportMem::Serialize(zenU32& auValue)
{
	if( mpMemoryCur + sizeof(auValue) >= mpMemoryEnd )
		return false;

	auValue		= *(zenU32*)mpMemoryCur;
	mpMemoryCur += sizeof(auValue);
	return true;
}

bool SerializerImportMem::Serialize(zenU64& auValue)
{
	if( mpMemoryCur + sizeof(auValue) >= mpMemoryEnd )
		return false;

	auValue		= *(zenU64*)mpMemoryCur;
	mpMemoryCur += sizeof(auValue);
	return true;
}

bool SerializerImportMem::Serialize(zenS8& aiValue)
{
	if( mpMemoryCur + sizeof(aiValue) >= mpMemoryEnd )
		return false;

	aiValue		= *(zenS8*)mpMemoryCur;
	mpMemoryCur += sizeof(aiValue);
	return true;
}

bool SerializerImportMem::Serialize(zenS16& aiValue)
{
	if( mpMemoryCur + sizeof(aiValue) >= mpMemoryEnd )
		return false;

	aiValue		= *(zenS16*)mpMemoryCur;
	mpMemoryCur += sizeof(aiValue);
	return true;
}

bool SerializerImportMem::Serialize(zenS32& aiValue)
{
	if( mpMemoryCur + sizeof(aiValue) >= mpMemoryEnd )
		return false;

	aiValue		= *(zenS32*)mpMemoryCur;
	mpMemoryCur += sizeof(aiValue);
	return true;
}

bool SerializerImportMem::Serialize(zenS64& aiValue)
{
	if( mpMemoryCur + sizeof(aiValue) >= mpMemoryEnd )
		return false;

	aiValue		= *(zenS64*)mpMemoryCur;
	mpMemoryCur += sizeof(aiValue);
	return true;
}

bool SerializerImportMem::Serialize(zenArrayBase<zenU8>& _aValues)
{
	zenU32 uSize;
	if( Serialize(uSize) )
	{
		if( mpMemoryCur + uSize >= mpMemoryEnd )
			return false;

		_aValues.SetCount(uSize/sizeof(zenU8));
		zenMem::Copy(_aValues.First(), mpMemoryCur, uSize );
		mpMemoryCur		+= uSize;
		return true;
	}
	return false;	
}

bool SerializerImportMem::Serialize(zenArrayBase<zenU16>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(zenArrayBase<zenU32>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(zenArrayBase<zenU64>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(zenArrayBase<zenS8>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(zenArrayBase<zenS16>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(zenArrayBase<zenS32>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(zenArrayBase<zenS64>& _aValues)
{
	return true;
}

}

