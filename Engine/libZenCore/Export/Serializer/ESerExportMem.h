#pragma once
#ifndef __LibExport_Serializer_ExportMemory_h__
#define __LibExport_Serializer_ExportMemory_h__

namespace EExp
{		
class SerializerExportMem : public ISerializerExporter
{
AWClassDeclare(SerializerExportMem, ISerializerExporter)
public:
						SerializerExportMem(awUInt auAllocSize, CMem::Allocator* apAllocator=NULL);
	virtual				~SerializerExportMem();
			bool		Save(const char* azFilename);//! @todo: abstract this
protected:
	struct Alloc : public awList1xNode
	{
		awU8*	pMemoryStart;
		awU8*	pMemoryCur;
		awU8*	pMemoryEnd;
	};						

	void*				GetMemory	(awUInt auSize);
	void				AddMemory	(awUInt auSize);
	
	awList1x	mlstAllocs;
	awUInt				muAllocSize;
	CMem::Allocator*	mpAllocator;

//-------------------------------------------------------------------
// Serializer_Base Interface
//-------------------------------------------------------------------
public:
	virtual bool	Serialize(awU8&						_uValue);
	virtual bool	Serialize(awU16&						_uValue);
	virtual bool	Serialize(awU32&						_uValue);
	virtual bool	Serialize(awU64&						_uValue);
	virtual bool	Serialize(awS8&						_iValue);
	virtual bool	Serialize(awS16&						_iValue);
	virtual bool	Serialize(awS32&						_iValue);
	virtual bool	Serialize(awS64&						_iValue);
	virtual bool	Serialize(awArrayBase<awU8>&	_aValues);
	virtual bool	Serialize(awArrayBase<awU16>&	_aValues);
	virtual bool	Serialize(awArrayBase<awU32>&	_aValues);
	virtual bool	Serialize(awArrayBase<awU64>&	_aValues);
	virtual bool	Serialize(awArrayBase<awS8>&	_aValues);
	virtual bool	Serialize(awArrayBase<awS16>&	_aValues);
	virtual bool	Serialize(awArrayBase<awS32>&	_aValues);
	virtual bool	Serialize(awArrayBase<awS64>&	_aValues);
protected:
	virtual bool	ItemStarted(EExp::SerialItem& aItem);

	//! @todo : support skipping failed item
	//virtual bool	ItemSkip(ExportItem& aItem);
};
}

#endif