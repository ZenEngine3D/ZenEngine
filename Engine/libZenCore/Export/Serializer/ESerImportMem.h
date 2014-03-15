#pragma once
#ifndef __LibExport_Serializer_ImportMemory_h__
#define __LibExport_Serializer_ImportMemory_h__

namespace EExp
{		
class SerializerImportMem : public ISerializerImporter
{
ZENClassDeclare(SerializerImportMem, ISerializerImporter)
public:
					SerializerImportMem(zU8* apBuffer, zUInt auBufferSize);
	bool			Load(const char* azFilename);

protected:	
	zU8*				mpMemoryStart;
	zU8*				mpMemoryCur;
	zU8*				mpMemoryEnd;
	zU8*				mpMemoryItemStart;

//-------------------------------------------------------------------
// Serializer_Base Interface
//-------------------------------------------------------------------
public:
	virtual bool	Serialize(zU8&						auValue);
	virtual bool	Serialize(zU16&						auValue);
	virtual bool	Serialize(zU32&						auValue);
	virtual bool	Serialize(zU64&						auValue);
	virtual bool	Serialize(zS8&						aiValue);
	virtual bool	Serialize(zS16&						aiValue);
	virtual bool	Serialize(zS32&						aiValue);
	virtual bool	Serialize(zS64&						aiValue);
	virtual bool	Serialize(zArrayBase<zU8>&	aValues);
	virtual bool	Serialize(zArrayBase<zU16>&	aValues);
	virtual bool	Serialize(zArrayBase<zU32>&	aValues);
	virtual bool	Serialize(zArrayBase<zU64>&	aValues);
	virtual bool	Serialize(zArrayBase<zS8>&	aValues);
	virtual bool	Serialize(zArrayBase<zS16>&	aValues);
	virtual bool	Serialize(zArrayBase<zS32>&	aValues);
	virtual bool	Serialize(zArrayBase<zS64>&	aValues);	

protected:				
	virtual bool	ItemStarted(EExp::SerialItem&		aItem);
	virtual bool	ItemSkip(EExp::SerialItem&			aItem);
};
}

#endif