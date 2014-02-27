#pragma once
#ifndef __LibExport_Serializer_ImportMemory_h__
#define __LibExport_Serializer_ImportMemory_h__

namespace EExp
{		
class SerializerImportMem : public ISerializerImporter
{
ZENClassDeclare(SerializerImportMem, ISerializerImporter)
public:
					SerializerImportMem(zenU8* apBuffer, zenUInt auBufferSize);
	bool			Load(const char* azFilename);//! @todo: abstract this

protected:	
	zenU8*				mpMemoryStart;
	zenU8*				mpMemoryCur;
	zenU8*				mpMemoryEnd;
	zenU8*				mpMemoryItemStart;

//-------------------------------------------------------------------
// Serializer_Base Interface
//-------------------------------------------------------------------
public:
	virtual bool	Serialize(zenU8&						auValue);
	virtual bool	Serialize(zenU16&						auValue);
	virtual bool	Serialize(zenU32&						auValue);
	virtual bool	Serialize(zenU64&						auValue);
	virtual bool	Serialize(zenS8&						aiValue);
	virtual bool	Serialize(zenS16&						aiValue);
	virtual bool	Serialize(zenS32&						aiValue);
	virtual bool	Serialize(zenS64&						aiValue);
	virtual bool	Serialize(zenArrayBase<zenU8>&	aValues);
	virtual bool	Serialize(zenArrayBase<zenU16>&	aValues);
	virtual bool	Serialize(zenArrayBase<zenU32>&	aValues);
	virtual bool	Serialize(zenArrayBase<zenU64>&	aValues);
	virtual bool	Serialize(zenArrayBase<zenS8>&	aValues);
	virtual bool	Serialize(zenArrayBase<zenS16>&	aValues);
	virtual bool	Serialize(zenArrayBase<zenS32>&	aValues);
	virtual bool	Serialize(zenArrayBase<zenS64>&	aValues);	

protected:				
	virtual bool	ItemStarted(EExp::SerialItem&		aItem);
	virtual bool	ItemSkip(EExp::SerialItem&			aItem);
};
}

#endif