#include "zcCore.h"
#include <stdio.h> //! @note Temporary

namespace zcExp
{		

SerializerExportMem::SerializerExportMem(zUInt auAllocSize)
: muAllocSize(auAllocSize)
{
}

SerializerExportMem::~SerializerExportMem()
{
	while( !mlstAllocs.IsEmpty() )
	{
		void* pAdr = mlstAllocs.PopHead();
		zenDelNullArray(pAdr);
	}
}

bool SerializerExportMem::ItemStarted(zcExp::ResourceData& aItem)
{
	if( ISerializerExporter::ItemStarted( aItem ) )
	{
		Alloc* pAlloc = (Alloc*)mlstAllocs.GetHead();
		if( pAlloc == mlstAllocs.GetInvalid() || pAlloc->pMemoryCur+aItem.muSize >= pAlloc->pMemoryEnd )
		{
			Alloc* pAlloc;
			zU32	uAllocSize		= zenMath::Max(muAllocSize, aItem.muSize);
			pAlloc					= (Alloc*) zenNewDefault zU8[ uAllocSize + sizeof(Alloc)] ;
			pAlloc->pMemoryStart	= (zU8*)pAlloc + sizeof(Alloc);
			pAlloc->pMemoryCur		= pAlloc->pMemoryStart;
			pAlloc->pMemoryEnd		= pAlloc->pMemoryStart + uAllocSize;
			mlstAllocs.AddHead(pAlloc);
		}

		return true;
	}	
	return false;
}

//! @todo Clean: Abstract this
bool SerializerExportMem::Save(const char* azFilename)
{
	FILE* pFile = fopen( azFilename, "wb" );
	if(pFile==NULL)
		return false;

	zList1xNode* pAlloc = mlstAllocs.GetHead();
	while( pAlloc != mlstAllocs.GetInvalid() )
	{
		fwrite( ((Alloc*)pAlloc)->pMemoryStart, ((Alloc*)pAlloc)->pMemoryCur-((Alloc*)pAlloc)->pMemoryStart, 1, pFile );
		pAlloc = pAlloc->LstNext();
	}

	fclose( pFile );
	return true;
}

void* SerializerExportMem::GetMemory(zUInt _uSize)
{
	Alloc* pAlloc = (Alloc*)mlstAllocs.GetHead();
	ZENAssertMsg(pAlloc->pMemoryCur+_uSize <= pAlloc->pMemoryEnd, "Not enought memory pre-allocated for this item. Item is not respecting SetItemSize() value.");
	void* pMemory		= pAlloc->pMemoryCur;
	pAlloc->pMemoryCur	+= _uSize;
	return pMemory;
}

bool SerializerExportMem::Serialize(zU8& _uValue)
{
	zU8* pValue	= static_cast<zU8*>(GetMemory(sizeof(_uValue)));
	*pValue		= _uValue;
	return true;
}

bool SerializerExportMem::Serialize(zU16& _uValue)
{
	zU16* pValue = static_cast<zU16*>(GetMemory(sizeof(_uValue)));
	*pValue		= _uValue;
	return true;
}

bool SerializerExportMem::Serialize(zU32& _uValue)
{
	zU32* pValue	= static_cast<zU32*>(GetMemory(sizeof(_uValue)));
	*pValue		= _uValue;
	return true;
}

bool SerializerExportMem::Serialize(zU64& _uValue)
{
	zU64* pValue	= static_cast<zU64*>(GetMemory(sizeof(_uValue)));
	*pValue		= _uValue;
	return true;
}

bool SerializerExportMem::Serialize(zI8& _iValue)
{
	zI8* pValue	= static_cast<zI8*>(GetMemory(sizeof(_iValue)));
	*pValue		= _iValue;
	return true;
}

bool SerializerExportMem::Serialize(zI16& _iValue)
{
	zI16* pValue	= static_cast<zI16*>(GetMemory(sizeof(_iValue)));
	*pValue		= _iValue;
	return true;
}

bool SerializerExportMem::Serialize(zI32& _iValue)
{
	zI32* pValue	= static_cast<zI32*>(GetMemory(sizeof(_iValue)));
	*pValue		= _iValue;
	return true;
}

bool SerializerExportMem::Serialize(zI64& _iValue)
{
	zI64* pValue	= static_cast<zI64*>(GetMemory(sizeof(_iValue)));
	*pValue		= _iValue;
	return true;
}

bool SerializerExportMem::Serialize(zArrayBase<zU8>& _aValues)
{
	zU32 uSize(_aValues.SizeMem());
	Serialize(uSize);
	zU8* pValues	= static_cast<zU8*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.First(), uSize );				
	return true;
}

bool SerializerExportMem::Serialize(zArrayBase<zU16>& _aValues)
{
	zU32 uSize(_aValues.SizeMem());
	Serialize(uSize);
	zU16* pValues = static_cast<zU16*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.First(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(zArrayBase<zU32>& _aValues)
{
	zUInt uSize(_aValues.SizeMem());
	Serialize(uSize);
	zU32* pValues = static_cast<zU32*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.First(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(zArrayBase<zU64>& _aValues)
{
	zUInt uSize(_aValues.SizeMem());
	Serialize(uSize);
	zU64* pValues = static_cast<zU64*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.First(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(zArrayBase<zI8>& _aValues)
{
	zI8 uSize(_aValues.SizeMem());
	Serialize(uSize);
	zI8* pValues = static_cast<zI8*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.First(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(zArrayBase<zI16>& _aValues)
{
	zU32 uSize(_aValues.SizeMem());
	Serialize(uSize);
	zI16* pValues = static_cast<zI16*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.First(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(zArrayBase<zI32>& _aValues)
{
	zU32 uSize(_aValues.SizeMem());
	Serialize(uSize);
	zI32* pValues = static_cast<zI32*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.First(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(zArrayBase<zI64>& _aValues)
{
	zU32 uSize(_aValues.SizeMem());
	if( Serialize(uSize) )
	{
		zI64* pValues = static_cast<zI64*>(GetMemory(_aValues.SizeMem()));
		zenMem::Copy(pValues, _aValues.First(), _aValues.SizeMem() );
		return true;
	}
	return false;
}


}

