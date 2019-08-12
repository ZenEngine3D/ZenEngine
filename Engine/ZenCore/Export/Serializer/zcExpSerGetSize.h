#pragma once

namespace zcExp
{		
	class SerializerGetSize : public Serializer_Base
	{
	zenClassDeclare(SerializerGetSize, Serializer_Base)
	public:
						SerializerGetSize():muSerializeSize(0){}
		virtual eStatus	GetStatus(){return keStatus_GetSize;}
		zUInt			GetSerializeSize(){return muSerializeSize;}
		
		// Implement Serializer_Base
		virtual bool	Serialize(zU8&						auValue){muSerializeSize += sizeof(auValue); return true;}
		virtual bool	Serialize(zU16&						auValue){muSerializeSize += sizeof(auValue); return true;}
		virtual bool	Serialize(zU32&						auValue){muSerializeSize += sizeof(auValue); return true;}
		virtual bool	Serialize(zU64&						auValue){muSerializeSize += sizeof(auValue); return true;}
		virtual bool	Serialize(zI8&						aiValue){muSerializeSize += sizeof(aiValue); return true;}
		virtual bool	Serialize(zI16&						aiValue){muSerializeSize += sizeof(aiValue); return true;}
		virtual bool	Serialize(zI32&						aiValue){muSerializeSize += sizeof(aiValue); return true;}
		virtual bool	Serialize(zI64&						aiValue){muSerializeSize += sizeof(aiValue); return true;}
		virtual bool	Serialize(zArray<zU8>&	aValues){muSerializeSize += sizeof(zU32)+aValues.SizeMem(); return true;}
		virtual bool	Serialize(zArray<zU16>&	aValues){muSerializeSize += sizeof(zU32)+aValues.SizeMem(); return true;}
		virtual bool	Serialize(zArray<zU32>&	aValues){muSerializeSize += sizeof(zU32)+aValues.SizeMem(); return true;}
		virtual bool	Serialize(zArray<zU64>&	aValues){muSerializeSize += sizeof(zU32)+aValues.SizeMem(); return true;}
		virtual bool	Serialize(zArray<zI8>&	aValues){muSerializeSize += sizeof(zU32)+aValues.SizeMem(); return true;}
		virtual bool	Serialize(zArray<zI16>&	aValues){muSerializeSize += sizeof(zU32)+aValues.SizeMem(); return true;}
		virtual bool	Serialize(zArray<zI32>&	aValues){muSerializeSize += sizeof(zU32)+aValues.SizeMem(); return true;}
		virtual bool	Serialize(zArray<zI64>&	aValues){muSerializeSize += sizeof(zU32)+aValues.SizeMem(); return true;}

		virtual bool	ItemStarted(zcExp::ResourceData& aItem)
		{
			muSerializeSize = 0;			
			return true;
		}
		
	protected:
		zUInt			muSerializeSize;
	};
	
}
