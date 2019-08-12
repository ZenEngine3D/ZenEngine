#include "zcCore.h"
#include <stdio.h> //! @note Temporary

namespace zcExp
{		

SerializerImportMem::SerializerImportMem(zU8* apBuffer, zUInt auBufferSize)
:	mpMemoryStart(apBuffer)
,	mpMemoryCur(apBuffer)
,	mpMemoryEnd(apBuffer+auBufferSize)
,	mpMemoryItemStart(nullptr)
{
}

bool SerializerImportMem::ItemStarted(zcExp::ResourceData& aItem)
{
	if( ISerializerImporter::ItemStarted(aItem) )
	{
		mpMemoryItemStart = mpMemoryCur;
		return true;
	}
	return false;
}

bool SerializerImportMem::ItemSkip(zcExp::ResourceData& aItem)
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

bool SerializerImportMem::Serialize(zI8& aiValue)
{
	if( mpMemoryCur + sizeof(aiValue) >= mpMemoryEnd )
		return false;

	aiValue		= *(zI8*)mpMemoryCur;
	mpMemoryCur += sizeof(aiValue);
	return true;
}

bool SerializerImportMem::Serialize(zI16& aiValue)
{
	if( mpMemoryCur + sizeof(aiValue) >= mpMemoryEnd )
		return false;

	aiValue		= *(zI16*)mpMemoryCur;
	mpMemoryCur += sizeof(aiValue);
	return true;
}

bool SerializerImportMem::Serialize(zI32& aiValue)
{
	if( mpMemoryCur + sizeof(aiValue) >= mpMemoryEnd )
		return false;

	aiValue		= *(zI32*)mpMemoryCur;
	mpMemoryCur += sizeof(aiValue);
	return true;
}

bool SerializerImportMem::Serialize(zI64& aiValue)
{
	if( mpMemoryCur + sizeof(aiValue) >= mpMemoryEnd )
		return false;

	aiValue		= *(zI64*)mpMemoryCur;
	mpMemoryCur += sizeof(aiValue);
	return true;
}

bool SerializerImportMem::Serialize(zArrayDyn<zU8>& _aValues)
{
	zU32 uSize;
	if( Serialize(uSize) )
	{
		if( mpMemoryCur + uSize >= mpMemoryEnd )
			return false;

		_aValues.Copy(mpMemoryCur, uSize);
		mpMemoryCur	+= uSize;
		return true;
	}
	return false;	
}

bool SerializerImportMem::Serialize(zArrayDyn<zU16>& _aValues)
{
	//! @todo 2 ?
	return true;
}

bool SerializerImportMem::Serialize(zArrayDyn<zU32>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(zArrayDyn<zU64>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(zArrayDyn<zI8>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(zArrayDyn<zI16>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(zArrayDyn<zI32>& _aValues)
{
	return true;
}

bool SerializerImportMem::Serialize(zArrayDyn<zI64>& _aValues)
{
	return true;
}

}

