#pragma once

namespace zcExp
{		
class SerializerImportMem : public ISerializerImporter
{
zenClassDeclare(SerializerImportMem, ISerializerImporter)
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
	virtual bool	Serialize(zI8&						aiValue);
	virtual bool	Serialize(zI16&						aiValue);
	virtual bool	Serialize(zI32&						aiValue);
	virtual bool	Serialize(zI64&						aiValue);
	virtual bool	Serialize(zArrayBase<zU8>&	aValues);
	virtual bool	Serialize(zArrayBase<zU16>&	aValues);
	virtual bool	Serialize(zArrayBase<zU32>&	aValues);
	virtual bool	Serialize(zArrayBase<zU64>&	aValues);
	virtual bool	Serialize(zArrayBase<zI8>&	aValues);
	virtual bool	Serialize(zArrayBase<zI16>&	aValues);
	virtual bool	Serialize(zArrayBase<zI32>&	aValues);
	virtual bool	Serialize(zArrayBase<zI64>&	aValues);	

protected:				
	virtual bool	ItemStarted(zcExp::ResourceData&		aItem);
	virtual bool	ItemSkip(zcExp::ResourceData&			aItem);
};
}
