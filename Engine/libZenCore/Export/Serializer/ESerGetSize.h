#pragma once
#ifndef __LibExport_Serializer_GetSize_h__
#define __LibExport_Serializer_GetSize_h__

namespace EExp
{		
	class SerializerGetSize : public Serializer_Base
	{
	AWClassDeclare(SerializerGetSize, Serializer_Base)
	public:
						SerializerGetSize():muSerializeSize(0){}
		virtual eStatus	GetStatus(){return keStatus_GetSize;}
		zenU32				GetSerializeSize(){return muSerializeSize;}
		
		// Implement Serializer_Base
		virtual bool	Serialize(zenU8&						auValue){muSerializeSize += sizeof(auValue); return true;}
		virtual bool	Serialize(zenU16&						auValue){muSerializeSize += sizeof(auValue); return true;}
		virtual bool	Serialize(zenU32&						auValue){muSerializeSize += sizeof(auValue); return true;}
		virtual bool	Serialize(zenU64&						auValue){muSerializeSize += sizeof(auValue); return true;}
		virtual bool	Serialize(zenS8&						aiValue){muSerializeSize += sizeof(aiValue); return true;}
		virtual bool	Serialize(zenS16&						aiValue){muSerializeSize += sizeof(aiValue); return true;}
		virtual bool	Serialize(zenS32&						aiValue){muSerializeSize += sizeof(aiValue); return true;}
		virtual bool	Serialize(zenS64&						aiValue){muSerializeSize += sizeof(aiValue); return true;}
		virtual bool	Serialize(zenArrayBase<zenU8>&	aValues){muSerializeSize += sizeof(zenU32)+aValues.Size(); return true;}
		virtual bool	Serialize(zenArrayBase<zenU16>&	aValues){muSerializeSize += sizeof(zenU32)+aValues.Size(); return true;}
		virtual bool	Serialize(zenArrayBase<zenU32>&	aValues){muSerializeSize += sizeof(zenU32)+aValues.Size(); return true;}
		virtual bool	Serialize(zenArrayBase<zenU64>&	aValues){muSerializeSize += sizeof(zenU32)+aValues.Size(); return true;}
		virtual bool	Serialize(zenArrayBase<zenS8>&	aValues){muSerializeSize += sizeof(zenU32)+aValues.Size(); return true;}
		virtual bool	Serialize(zenArrayBase<zenS16>&	aValues){muSerializeSize += sizeof(zenU32)+aValues.Size(); return true;}
		virtual bool	Serialize(zenArrayBase<zenS32>&	aValues){muSerializeSize += sizeof(zenU32)+aValues.Size(); return true;}
		virtual bool	Serialize(zenArrayBase<zenS64>&	aValues){muSerializeSize += sizeof(zenU32)+aValues.Size(); return true;}

		virtual bool	ItemStarted(EExp::SerialItem& aItem)
		{
			muSerializeSize = 0;			
			return true;
		}
		
	protected:
		zenU32				muSerializeSize;
	};
	
}

#endif