#include "libZenCore.h"
#include <stdio.h> //! @note Temporary

namespace EExp
{		

SerializerExportMem::SerializerExportMem(zenUInt auAllocSize, CMem::Allocator* apAllocator)
:	mpAllocator(apAllocator)
,	muAllocSize(auAllocSize)
{
}

SerializerExportMem::~SerializerExportMem()
{
	while( !mlstAllocs.IsEmpty() )
	{
		void* pAdr = mlstAllocs.PopHead();
		AWDelNullArray(pAdr);
	}
}

bool SerializerExportMem::ItemStarted(EExp::SerialItem& aItem)
{
	if( ISerializerExporter::ItemStarted( aItem ) )
	{
		Alloc* pAlloc = (Alloc*)mlstAllocs.GetHead();
		if( pAlloc == mlstAllocs.GetInvalid() || pAlloc->pMemoryCur+aItem.muSize >= pAlloc->pMemoryEnd )
		{
			Alloc* pAlloc;
			zenU32	uAllocSize		= zenMath::Max(muAllocSize, aItem.muSize);
			pAlloc					= (Alloc*) AWNew(mpAllocator) zenU8[ uAllocSize + sizeof(Alloc)] ;
			pAlloc->pMemoryStart	= (zenU8*)pAlloc + sizeof(Alloc);
			pAlloc->pMemoryCur		= pAlloc->pMemoryStart;
			pAlloc->pMemoryEnd		= pAlloc->pMemoryStart + uAllocSize;
			mlstAllocs.AddHead(pAlloc);
		}

		return true;
	}	
	return false;
}

//! @todo: abstract this
bool SerializerExportMem::Save(const char* azFilename)
{
	FILE* pFile = fopen( azFilename, "wb" );
	if(pFile==NULL)
		return false;

	zenList1xNode* pAlloc = mlstAllocs.GetHead();
	while( pAlloc != mlstAllocs.GetInvalid() )
	{
		fwrite( ((Alloc*)pAlloc)->pMemoryStart, ((Alloc*)pAlloc)->pMemoryCur-((Alloc*)pAlloc)->pMemoryStart, 1, pFile );
		pAlloc = pAlloc->LstNext();
	}

	fclose( pFile );
	return true;
}

void* SerializerExportMem::GetMemory(zenUInt _uSize)
{
	Alloc* pAlloc = (Alloc*)mlstAllocs.GetHead();
	AWAssertMsg(pAlloc->pMemoryCur+_uSize <= pAlloc->pMemoryEnd, "Not enought memory pre-allocated for this item. Item is not respecting SetItemSize() value.");
	void* pMemory		= pAlloc->pMemoryCur;
	pAlloc->pMemoryCur	+= _uSize;
	return pMemory;
}

bool SerializerExportMem::Serialize(zenU8& _uValue)
{
	zenU8* pValue	= static_cast<zenU8*>(GetMemory(sizeof(_uValue)));
	*pValue		= _uValue;
	return true;
}

bool SerializerExportMem::Serialize(zenU16& _uValue)
{
	zenU16* pValue = static_cast<zenU16*>(GetMemory(sizeof(_uValue)));
	*pValue		= _uValue;
	return true;
}

bool SerializerExportMem::Serialize(zenU32& _uValue)
{
	zenU32* pValue	= static_cast<zenU32*>(GetMemory(sizeof(_uValue)));
	*pValue		= _uValue;
	return true;
}

bool SerializerExportMem::Serialize(zenU64& _uValue)
{
	zenU64* pValue	= static_cast<zenU64*>(GetMemory(sizeof(_uValue)));
	*pValue		= _uValue;
	return true;
}

bool SerializerExportMem::Serialize(zenS8& _iValue)
{
	zenS8* pValue	= static_cast<zenS8*>(GetMemory(sizeof(_iValue)));
	*pValue		= _iValue;
	return true;
}

bool SerializerExportMem::Serialize(zenS16& _iValue)
{
	zenS16* pValue	= static_cast<zenS16*>(GetMemory(sizeof(_iValue)));
	*pValue		= _iValue;
	return true;
}

bool SerializerExportMem::Serialize(zenS32& _iValue)
{
	zenS32* pValue	= static_cast<zenS32*>(GetMemory(sizeof(_iValue)));
	*pValue		= _iValue;
	return true;
}

bool SerializerExportMem::Serialize(zenS64& _iValue)
{
	zenS64* pValue	= static_cast<zenS64*>(GetMemory(sizeof(_iValue)));
	*pValue		= _iValue;
	return true;
}

bool SerializerExportMem::Serialize(zenArrayBase<zenU8>& _aValues)
{
	zenU32 uSize(_aValues.Size());
	Serialize(uSize);
	zenU8* pValues	= static_cast<zenU8*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.First(), uSize );				
	return true;
}

bool SerializerExportMem::Serialize(zenArrayBase<zenU16>& _aValues)
{
	zenU32 uSize(_aValues.Size());
	Serialize(uSize);
	zenU16* pValues = static_cast<zenU16*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.First(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(zenArrayBase<zenU32>& _aValues)
{
	zenUInt uSize(_aValues.Size());
	Serialize(uSize);
	zenU32* pValues = static_cast<zenU32*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.First(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(zenArrayBase<zenU64>& _aValues)
{
	zenUInt uSize(_aValues.Size());
	Serialize(uSize);
	zenU64* pValues = static_cast<zenU64*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.First(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(zenArrayBase<zenS8>& _aValues)
{
	zenS8 uSize(_aValues.Size());
	Serialize(uSize);
	zenS8* pValues = static_cast<zenS8*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.First(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(zenArrayBase<zenS16>& _aValues)
{
	zenU32 uSize(_aValues.Size());
	Serialize(uSize);
	zenS16* pValues = static_cast<zenS16*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.First(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(zenArrayBase<zenS32>& _aValues)
{
	zenU32 uSize(_aValues.Size());
	Serialize(uSize);
	zenS32* pValues = static_cast<zenS32*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.First(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(zenArrayBase<zenS64>& _aValues)
{
	zenU32 uSize(_aValues.Size());
	if( Serialize(uSize) )
	{
		zenS64* pValues = static_cast<zenS64*>(GetMemory(_aValues.Size()));
		zenMem::Copy(pValues, _aValues.First(), _aValues.Size() );
		return true;
	}
	return false;
}


}

