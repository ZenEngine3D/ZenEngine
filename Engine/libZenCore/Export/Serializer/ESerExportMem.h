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
	bool			Save(const char* azFilename);//! @todo: abstract this
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
	virtual bool	Serialize(zS8&						_iValue);
	virtual bool	Serialize(zS16&						_iValue);
	virtual bool	Serialize(zS32&						_iValue);
	virtual bool	Serialize(zS64&						_iValue);
	virtual bool	Serialize(zArrayBase<zU8>&	_aValues);
	virtual bool	Serialize(zArrayBase<zU16>&	_aValues);
	virtual bool	Serialize(zArrayBase<zU32>&	_aValues);
	virtual bool	Serialize(zArrayBase<zU64>&	_aValues);
	virtual bool	Serialize(zArrayBase<zS8>&	_aValues);
	virtual bool	Serialize(zArrayBase<zS16>&	_aValues);
	virtual bool	Serialize(zArrayBase<zS32>&	_aValues);
	virtual bool	Serialize(zArrayBase<zS64>&	_aValues);
protected:
	virtual bool	ItemStarted(EExp::SerialItem& aItem);

	//! @todo : support skipping failed item
	//virtual bool	ItemSkip(ExportItem& aItem);
};
}

#endif