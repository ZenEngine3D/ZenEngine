#include "libZenCore.h"
#include <stdio.h> //! @note Temporary

namespace EExp
{		

SerializerExportMem::SerializerExportMem(awUInt auAllocSize, CMem::Allocator* apAllocator)
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
			awU32	uAllocSize		= zenMath::Max(muAllocSize, aItem.muSize);
			pAlloc					= (Alloc*) AWNew(mpAllocator) awU8[ uAllocSize + sizeof(Alloc)] ;
			pAlloc->pMemoryStart	= (awU8*)pAlloc + sizeof(Alloc);
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

	awList1xNode* pAlloc = mlstAllocs.GetHead();
	while( pAlloc != mlstAllocs.GetInvalid() )
	{
		fwrite( ((Alloc*)pAlloc)->pMemoryStart, ((Alloc*)pAlloc)->pMemoryCur-((Alloc*)pAlloc)->pMemoryStart, 1, pFile );
		pAlloc = pAlloc->LstNext();
	}

	fclose( pFile );
	return true;
}

void* SerializerExportMem::GetMemory(awUInt _uSize)
{
	Alloc* pAlloc = (Alloc*)mlstAllocs.GetHead();
	AWAssertMsg(pAlloc->pMemoryCur+_uSize <= pAlloc->pMemoryEnd, "Not enought memory pre-allocated for this item. Item is not respecting SetItemSize() value.");
	void* pMemory		= pAlloc->pMemoryCur;
	pAlloc->pMemoryCur	+= _uSize;
	return pMemory;
}

bool SerializerExportMem::Serialize(awU8& _uValue)
{
	awU8* pValue	= static_cast<awU8*>(GetMemory(sizeof(_uValue)));
	*pValue		= _uValue;
	return true;
}

bool SerializerExportMem::Serialize(awU16& _uValue)
{
	awU16* pValue = static_cast<awU16*>(GetMemory(sizeof(_uValue)));
	*pValue		= _uValue;
	return true;
}

bool SerializerExportMem::Serialize(awU32& _uValue)
{
	awU32* pValue	= static_cast<awU32*>(GetMemory(sizeof(_uValue)));
	*pValue		= _uValue;
	return true;
}

bool SerializerExportMem::Serialize(awU64& _uValue)
{
	awU64* pValue	= static_cast<awU64*>(GetMemory(sizeof(_uValue)));
	*pValue		= _uValue;
	return true;
}

bool SerializerExportMem::Serialize(awS8& _iValue)
{
	awS8* pValue	= static_cast<awS8*>(GetMemory(sizeof(_iValue)));
	*pValue		= _iValue;
	return true;
}

bool SerializerExportMem::Serialize(awS16& _iValue)
{
	awS16* pValue	= static_cast<awS16*>(GetMemory(sizeof(_iValue)));
	*pValue		= _iValue;
	return true;
}

bool SerializerExportMem::Serialize(awS32& _iValue)
{
	awS32* pValue	= static_cast<awS32*>(GetMemory(sizeof(_iValue)));
	*pValue		= _iValue;
	return true;
}

bool SerializerExportMem::Serialize(awS64& _iValue)
{
	awS64* pValue	= static_cast<awS64*>(GetMemory(sizeof(_iValue)));
	*pValue		= _iValue;
	return true;
}

bool SerializerExportMem::Serialize(awArrayBase<awU8>& _aValues)
{
	awU32 uSize(_aValues.Size());
	Serialize(uSize);
	awU8* pValues	= static_cast<awU8*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.First(), uSize );				
	return true;
}

bool SerializerExportMem::Serialize(awArrayBase<awU16>& _aValues)
{
	awU32 uSize(_aValues.Size());
	Serialize(uSize);
	awU16* pValues = static_cast<awU16*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.First(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(awArrayBase<awU32>& _aValues)
{
	awUInt uSize(_aValues.Size());
	Serialize(uSize);
	awU32* pValues = static_cast<awU32*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.First(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(awArrayBase<awU64>& _aValues)
{
	awUInt uSize(_aValues.Size());
	Serialize(uSize);
	awU64* pValues = static_cast<awU64*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.First(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(awArrayBase<awS8>& _aValues)
{
	awS8 uSize(_aValues.Size());
	Serialize(uSize);
	awS8* pValues = static_cast<awS8*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.First(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(awArrayBase<awS16>& _aValues)
{
	awU32 uSize(_aValues.Size());
	Serialize(uSize);
	awS16* pValues = static_cast<awS16*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.First(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(awArrayBase<awS32>& _aValues)
{
	awU32 uSize(_aValues.Size());
	Serialize(uSize);
	awS32* pValues = static_cast<awS32*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.First(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(awArrayBase<awS64>& _aValues)
{
	awU32 uSize(_aValues.Size());
	if( Serialize(uSize) )
	{
		awS64* pValues = static_cast<awS64*>(GetMemory(_aValues.Size()));
		zenMem::Copy(pValues, _aValues.First(), _aValues.Size() );
		return true;
	}
	return false;
}


}

