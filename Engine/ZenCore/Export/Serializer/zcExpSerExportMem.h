#pragma once

namespace zcExp
{		
class SerializerExportMem : public ISerializerExporter
{
zenClassDeclare(SerializerExportMem, ISerializerExporter)
public:
					SerializerExportMem(size_t auAllocSize);
	virtual			~SerializerExportMem();
	bool			Save(const char* azFilename);
protected:
	struct Alloc : public zListItem<>
	{
	public:
		zU8*		mpMemoryStart;
		zU8*		mpMemoryCur;
		zU8*		mpMemoryEnd;
	};						

	void*			GetMemory	(size_t auSize);
	void			AddMemory	(size_t auSize);
	
	zList<Alloc>	mlstAllocs; //! @todo 2 Remove this book-keeping of allocation, just use large re sizable array
	size_t			muAllocSize;

//-------------------------------------------------------------------
// Serializer_Base Interface
//-------------------------------------------------------------------
public:
	virtual bool	Serialize(zU8&						_uValue);
	virtual bool	Serialize(zU16&						_uValue);
	virtual bool	Serialize(zU32&						_uValue);
	virtual bool	Serialize(zU64&						_uValue);
	virtual bool	Serialize(zI8&						_iValue);
	virtual bool	Serialize(zI16&						_iValue);
	virtual bool	Serialize(zI32&						_iValue);
	virtual bool	Serialize(zI64&						_iValue);
	virtual bool	Serialize(zArray<zU8>&	_aValues);
	virtual bool	Serialize(zArray<zU16>&	_aValues);
	virtual bool	Serialize(zArray<zU32>&	_aValues);
	virtual bool	Serialize(zArray<zU64>&	_aValues);
	virtual bool	Serialize(zArray<zI8>&	_aValues);
	virtual bool	Serialize(zArray<zI16>&	_aValues);
	virtual bool	Serialize(zArray<zI32>&	_aValues);
	virtual bool	Serialize(zArray<zI64>&	_aValues);
protected:
	virtual bool	ItemStarted(zcExp::ResourceData& aItem);

	//! @todo Missing: Support skipping failed item
	//virtual bool	ItemSkip(ExportItem& aItem);
};
}
