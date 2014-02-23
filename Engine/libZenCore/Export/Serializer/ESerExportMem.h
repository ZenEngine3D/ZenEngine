#pragma once
#ifndef __LibExport_Serializer_ExportMemory_h__
#define __LibExport_Serializer_ExportMemory_h__

namespace EExp
{		
class SerializerExportMem : public ISerializerExporter
{
AWClassDeclare(SerializerExportMem, ISerializerExporter)
public:
						SerializerExportMem(zenUInt auAllocSize, CMem::Allocator* apAllocator=NULL);
	virtual				~SerializerExportMem();
			bool		Save(const char* azFilename);//! @todo: abstract this
protected:
	struct Alloc : public zenList1xNode
	{
		zenU8*	pMemoryStart;
		zenU8*	pMemoryCur;
		zenU8*	pMemoryEnd;
	};						

	void*				GetMemory	(zenUInt auSize);
	void				AddMemory	(zenUInt auSize);
	
	zenList1x	mlstAllocs;
	zenUInt				muAllocSize;
	CMem::Allocator*	mpAllocator;

//-------------------------------------------------------------------
// Serializer_Base Interface
//-------------------------------------------------------------------
public:
	virtual bool	Serialize(zenU8&						_uValue);
	virtual bool	Serialize(zenU16&						_uValue);
	virtual bool	Serialize(zenU32&						_uValue);
	virtual bool	Serialize(zenU64&						_uValue);
	virtual bool	Serialize(zenS8&						_iValue);
	virtual bool	Serialize(zenS16&						_iValue);
	virtual bool	Serialize(zenS32&						_iValue);
	virtual bool	Serialize(zenS64&						_iValue);
	virtual bool	Serialize(zenArrayBase<zenU8>&	_aValues);
	virtual bool	Serialize(zenArrayBase<zenU16>&	_aValues);
	virtual bool	Serialize(zenArrayBase<zenU32>&	_aValues);
	virtual bool	Serialize(zenArrayBase<zenU64>&	_aValues);
	virtual bool	Serialize(zenArrayBase<zenS8>&	_aValues);
	virtual bool	Serialize(zenArrayBase<zenS16>&	_aValues);
	virtual bool	Serialize(zenArrayBase<zenS32>&	_aValues);
	virtual bool	Serialize(zenArrayBase<zenS64>&	_aValues);
protected:
	virtual bool	ItemStarted(EExp::SerialItem& aItem);

	//! @todo : support skipping failed item
	//virtual bool	ItemSkip(ExportItem& aItem);
};
}

#endif