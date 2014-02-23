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
		awU32				GetSerializeSize(){return muSerializeSize;}
		
		// Implement Serializer_Base
		virtual bool	Serialize(awU8&						auValue){muSerializeSize += sizeof(auValue); return true;}
		virtual bool	Serialize(awU16&						auValue){muSerializeSize += sizeof(auValue); return true;}
		virtual bool	Serialize(awU32&						auValue){muSerializeSize += sizeof(auValue); return true;}
		virtual bool	Serialize(awU64&						auValue){muSerializeSize += sizeof(auValue); return true;}
		virtual bool	Serialize(awS8&						aiValue){muSerializeSize += sizeof(aiValue); return true;}
		virtual bool	Serialize(awS16&						aiValue){muSerializeSize += sizeof(aiValue); return true;}
		virtual bool	Serialize(awS32&						aiValue){muSerializeSize += sizeof(aiValue); return true;}
		virtual bool	Serialize(awS64&						aiValue){muSerializeSize += sizeof(aiValue); return true;}
		virtual bool	Serialize(awArrayBase<awU8>&	aValues){muSerializeSize += sizeof(awU32)+aValues.Size(); return true;}
		virtual bool	Serialize(awArrayBase<awU16>&	aValues){muSerializeSize += sizeof(awU32)+aValues.Size(); return true;}
		virtual bool	Serialize(awArrayBase<awU32>&	aValues){muSerializeSize += sizeof(awU32)+aValues.Size(); return true;}
		virtual bool	Serialize(awArrayBase<awU64>&	aValues){muSerializeSize += sizeof(awU32)+aValues.Size(); return true;}
		virtual bool	Serialize(awArrayBase<awS8>&	aValues){muSerializeSize += sizeof(awU32)+aValues.Size(); return true;}
		virtual bool	Serialize(awArrayBase<awS16>&	aValues){muSerializeSize += sizeof(awU32)+aValues.Size(); return true;}
		virtual bool	Serialize(awArrayBase<awS32>&	aValues){muSerializeSize += sizeof(awU32)+aValues.Size(); return true;}
		virtual bool	Serialize(awArrayBase<awS64>&	aValues){muSerializeSize += sizeof(awU32)+aValues.Size(); return true;}

		virtual bool	ItemStarted(EExp::SerialItem& aItem)
		{
			muSerializeSize = 0;			
			return true;
		}
		
	protected:
		awU32				muSerializeSize;
	};
	
}

#endif