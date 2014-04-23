#pragma once
#ifndef __LibExport_Serializer_ExportMemory_h__
#define __LibExport_Serializer_ExportMemory_h__

namespace EExp
{		
class SerializerExportMem : public ISerializerExporter
{
ZENClassDeclare(SerializerExportMem, ISerializerExporter)
public:
					SerializerExportMem(zUInt auAllocSize);
	virtual			~SerializerExportMem();
	bool			Save(const char* azFilename);
protected:
	struct Alloc : public zList1xNode
	{
		zU8*	pMemoryStart;
		zU8*	pMemoryCur;
		zU8*	pMemoryEnd;
	};						

	void*			GetMemory	(zUInt auSize);
	void			AddMemory	(zUInt auSize);
	
	zList1x		mlstAllocs;
	zUInt			muAllocSize;

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
	virtual bool	Serialize(zArrayBase<zU8>&	_aValues);
	virtual bool	Serialize(zArrayBase<zU16>&	_aValues);
	virtual bool	Serialize(zArrayBase<zU32>&	_aValues);
	virtual bool	Serialize(zArrayBase<zU64>&	_aValues);
	virtual bool	Serialize(zArrayBase<zI8>&	_aValues);
	virtual bool	Serialize(zArrayBase<zI16>&	_aValues);
	virtual bool	Serialize(zArrayBase<zI32>&	_aValues);
	virtual bool	Serialize(zArrayBase<zI64>&	_aValues);
protected:
	virtual bool	ItemStarted(EExp::SerialItem& aItem);

	//! @todo Missing: Support skipping failed item
	//virtual bool	ItemSkip(ExportItem& aItem);
};
}

#endif