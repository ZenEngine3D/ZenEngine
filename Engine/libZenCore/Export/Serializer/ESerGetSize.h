#pragma once
#ifndef __LibExport_Serializer_GetSize_h__
#define __LibExport_Serializer_GetSize_h__

namespace EExp
{		
	class SerializerGetSize : public Serializer_Base
	{
	ZENClassDeclare(SerializerGetSize, Serializer_Base)
	public:
						SerializerGetSize():muSerializeSize(0){}
		virtual eStatus	GetStatus(){return keStatus_GetSize;}
		zU32				GetSerializeSize(){return muSerializeSize;}
		
		// Implement Serializer_Base
		virtual bool	Serialize(zU8&						auValue){muSerializeSize += sizeof(auValue); return true;}
		virtual bool	Serialize(zU16&						auValue){muSerializeSize += sizeof(auValue); return true;}
		virtual bool	Serialize(zU32&						auValue){muSerializeSize += sizeof(auValue); return true;}
		virtual bool	Serialize(zU64&						auValue){muSerializeSize += sizeof(auValue); return true;}
		virtual bool	Serialize(zS8&						aiValue){muSerializeSize += sizeof(aiValue); return true;}
		virtual bool	Serialize(zS16&						aiValue){muSerializeSize += sizeof(aiValue); return true;}
		virtual bool	Serialize(zS32&						aiValue){muSerializeSize += sizeof(aiValue); return true;}
		virtual bool	Serialize(zS64&						aiValue){muSerializeSize += sizeof(aiValue); return true;}
		virtual bool	Serialize(zArrayBase<zU8>&	aValues){muSerializeSize += sizeof(zU32)+aValues.Size(); return true;}
		virtual bool	Serialize(zArrayBase<zU16>&	aValues){muSerializeSize += sizeof(zU32)+aValues.Size(); return true;}
		virtual bool	Serialize(zArrayBase<zU32>&	aValues){muSerializeSize += sizeof(zU32)+aValues.Size(); return true;}
		virtual bool	Serialize(zArrayBase<zU64>&	aValues){muSerializeSize += sizeof(zU32)+aValues.Size(); return true;}
		virtual bool	Serialize(zArrayBase<zS8>&	aValues){muSerializeSize += sizeof(zU32)+aValues.Size(); return true;}
		virtual bool	Serialize(zArrayBase<zS16>&	aValues){muSerializeSize += sizeof(zU32)+aValues.Size(); return true;}
		virtual bool	Serialize(zArrayBase<zS32>&	aValues){muSerializeSize += sizeof(zU32)+aValues.Size(); return true;}
		virtual bool	Serialize(zArrayBase<zS64>&	aValues){muSerializeSize += sizeof(zU32)+aValues.Size(); return true;}

		virtual bool	ItemStarted(EExp::SerialItem& aItem)
		{
			muSerializeSize = 0;			
			return true;
		}
		
	protected:
		zU32				muSerializeSize;
	};
	
}

#endif