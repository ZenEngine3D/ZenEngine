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
	while( !mlstAllocs.empty() )
	{
		zU8* pAdr = reinterpret_cast<zU8*>(mlstAllocs.pop_front());
		zenMem::DelSafe(pAdr);
	}
}

bool SerializerExportMem::ItemStarted(zcExp::ResourceData& aItem)
{
	if( ISerializerExporter::ItemStarted( aItem ) )
	{
		Alloc* pAlloc = mlstAllocs.front_check();
		if( !pAlloc || pAlloc->mpMemoryCur+aItem.muSize >= pAlloc->mpMemoryEnd )
		{
			size_t uAllocSize		= zenMath::Max<size_t>(muAllocSize, aItem.muSize);
			pAlloc					= reinterpret_cast<Alloc*>(zenMem::NewArray<zU8>(uAllocSize + sizeof(Alloc)));
			pAlloc->mpMemoryStart	= (zU8*)pAlloc + sizeof(Alloc);
			pAlloc->mpMemoryCur		= pAlloc->mpMemoryStart;
			pAlloc->mpMemoryEnd		= pAlloc->mpMemoryStart + uAllocSize;
			mlstAllocs.push_front(*pAlloc);
		}

		return true;
	}	
	return false;
}

//! @todo Clean: Abstract this
bool SerializerExportMem::Save(const char* azFilename)
{
//! todo clean Have platform independant file system abstraction
#if ZEN_PLATFORM_PC
	FILE* pFile(nullptr);
	if( fopen_s(&pFile, azFilename, "wb") != 0 )
		pFile = nullptr;
#else
	FILE* pFile = fopen( azFilename, "wb" );
#endif
	if(pFile==nullptr)
		return false;

	Alloc* pAlloc = mlstAllocs.front_check();
	while( pAlloc )
	{
		fwrite( ((Alloc*)pAlloc)->mpMemoryStart, ((Alloc*)pAlloc)->mpMemoryCur-((Alloc*)pAlloc)->mpMemoryStart, 1, pFile );
		pAlloc = mlstAllocs.GetNext(*pAlloc);
	}

	fclose( pFile );
	return true;
}

void* SerializerExportMem::GetMemory(zUInt _uSize)
{
	Alloc* pAlloc = (Alloc*)mlstAllocs.front_check();
	zenAssertMsg(pAlloc->mpMemoryCur+_uSize <= pAlloc->mpMemoryEnd, "Not enought memory pre-allocated for this item. Item is not respecting SetItemSize() value.");
	void* pMemory		= pAlloc->mpMemoryCur;
	pAlloc->mpMemoryCur	+= _uSize;
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

bool SerializerExportMem::Serialize(zArray<zU8>& _aValues)
{
	zUInt uSize(_aValues.SizeMem());
	Serialize(uSize);
	zU8* pValues	= static_cast<zU8*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.Data(), uSize );				
	return true;
}

bool SerializerExportMem::Serialize(zArray<zU16>& _aValues)
{
	zUInt uSize(_aValues.SizeMem());
	Serialize(uSize);
	zU16* pValues = static_cast<zU16*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.Data(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(zArray<zU32>& _aValues)
{
	zUInt uSize(_aValues.SizeMem());
	Serialize(uSize);
	zU32* pValues = static_cast<zU32*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.Data(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(zArray<zU64>& _aValues)
{
	zUInt uSize(_aValues.SizeMem());
	Serialize(uSize);
	zU64* pValues = static_cast<zU64*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.Data(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(zArray<zI8>& _aValues)
{
	zUInt uSize(_aValues.SizeMem());
	Serialize(uSize);
	zI8* pValues = static_cast<zI8*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.Data(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(zArray<zI16>& _aValues)
{
	zUInt uSize(_aValues.SizeMem());
	Serialize(uSize);
	zI16* pValues = static_cast<zI16*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.Data(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(zArray<zI32>& _aValues)
{
	zUInt uSize(_aValues.SizeMem());
	Serialize(uSize);
	zI32* pValues = static_cast<zI32*>(GetMemory(uSize));
	zenMem::Copy(pValues, _aValues.Data(), uSize );
	return true;
}

bool SerializerExportMem::Serialize(zArray<zI64>& _aValues)
{
	zUInt uSize(_aValues.SizeMem());
	if( Serialize(uSize) )
	{
		zI64* pValues = static_cast<zI64*>(GetMemory(_aValues.SizeMem()));
		zenMem::Copy(pValues, _aValues.Data(), _aValues.SizeMem() );
		return true;
	}
	return false;
}


}

