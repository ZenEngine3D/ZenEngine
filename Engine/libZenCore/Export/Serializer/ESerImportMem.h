#pragma once
#ifndef __LibExport_Serializer_ImportMemory_h__
#define __LibExport_Serializer_ImportMemory_h__

namespace EExp
{		
class SerializerImportMem : public ISerializerImporter
{
AWClassDeclare(SerializerImportMem, ISerializerImporter)
public:
					SerializerImportMem(awU8* apBuffer, awUInt auBufferSize);
	bool			Load(const char* azFilename);//! @todo: abstract this

protected:	
	awU8*				mpMemoryStart;
	awU8*				mpMemoryCur;
	awU8*				mpMemoryEnd;
	awU8*				mpMemoryItemStart;

//-------------------------------------------------------------------
// Serializer_Base Interface
//-------------------------------------------------------------------
public:
	virtual bool	Serialize(awU8&						auValue);
	virtual bool	Serialize(awU16&						auValue);
	virtual bool	Serialize(awU32&						auValue);
	virtual bool	Serialize(awU64&						auValue);
	virtual bool	Serialize(awS8&						aiValue);
	virtual bool	Serialize(awS16&						aiValue);
	virtual bool	Serialize(awS32&						aiValue);
	virtual bool	Serialize(awS64&						aiValue);
	virtual bool	Serialize(awArrayBase<awU8>&	aValues);
	virtual bool	Serialize(awArrayBase<awU16>&	aValues);
	virtual bool	Serialize(awArrayBase<awU32>&	aValues);
	virtual bool	Serialize(awArrayBase<awU64>&	aValues);
	virtual bool	Serialize(awArrayBase<awS8>&	aValues);
	virtual bool	Serialize(awArrayBase<awS16>&	aValues);
	virtual bool	Serialize(awArrayBase<awS32>&	aValues);
	virtual bool	Serialize(awArrayBase<awS64>&	aValues);	

protected:				
	virtual bool	ItemStarted(EExp::SerialItem&		aItem);
	virtual bool	ItemSkip(EExp::SerialItem&			aItem);
};
}

#endif