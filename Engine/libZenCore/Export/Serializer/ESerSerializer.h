#pragma once
#ifndef __LibExport_Serializer_h__
#define __LibExport_Serializer_h__

namespace EExp
{			
	//=================================================================================================
	// Forward declares
	//=================================================================================================
	class Serializer_Base;
	class SerialItem;

	//=================================================================================================
	//! @class		Base class used to serialize/deserialize items
	//=================================================================================================
	class Serializer_Base
	{
	ZENClassDeclareNoParent(Serializer_Base)
	public:
		enum eStatus{keStatus_Invalid, keStatus_GetSize, keStatus_Exporting, keStatus_Importing};		
						Serializer_Base();
		virtual 		~Serializer_Base(){}

		virtual void	SerializeStart();
		virtual void	SerializeStop();				
		virtual bool	ItemSerialize(SerialItem& aItem);
		virtual eStatus	GetStatus()=0;

		virtual bool	Serialize(zU8&						_uValue)=0;
		virtual bool	Serialize(zU16&						_uValue)=0;
		virtual bool	Serialize(zU32&						_uValue)=0;
		virtual bool	Serialize(zU64&						_uValue)=0;
		virtual bool	Serialize(zS8&						_iValue)=0;
		virtual bool	Serialize(zS16&						_iValue)=0;
		virtual bool	Serialize(zS32&						_iValue)=0;
		virtual bool	Serialize(zS64&						_iValue)=0;
		virtual bool	Serialize(zArrayBase<zU8>&	_Values)=0;
		virtual bool	Serialize(zArrayBase<zU16>&	_Values)=0;
		virtual bool	Serialize(zArrayBase<zU32>&	_Values)=0;
		virtual bool	Serialize(zArrayBase<zU64>&	_Values)=0;
		virtual bool	Serialize(zArrayBase<zS8>&	_Values)=0;
		virtual bool	Serialize(zArrayBase<zS16>&	_Values)=0;
		virtual bool	Serialize(zArrayBase<zS32>&	_Values)=0;
		virtual bool	Serialize(zArrayBase<zS64>&	_Values)=0;		

				bool	Serialize(zHash32&			_Value);
				bool	Serialize(zHash64&			_Value);
				bool	Serialize(ISerialize&				_Value);
				bool	Serialize(zResID&		_Value);

	protected:
		virtual bool	ItemStarted(SerialItem&				_Item);
		virtual bool	ItemEnded(SerialItem&				_Item);	
		virtual bool	ItemSkip(SerialItem&				_Item);	
		zU32				muItemProcessed;
		zU32				muItemFailed;
	};
	
	//=================================================================================================
	//! @class		Class that want to serialize items should be derived from this one
	//=================================================================================================
	class ISerializerExporter : public Serializer_Base
	{	
	ZENClassDeclare(ISerializerExporter, Serializer_Base)
	public:
		virtual eStatus	GetStatus(){return keStatus_Exporting;}
		virtual bool	ItemStarted(SerialItem& aItem);
	
	protected:
		zU32		muItemSize;
	};
	
	//=================================================================================================
	//! @class		Class that want to deserialize items should be derived from this one
	//=================================================================================================
	class ISerializerImporter : public Serializer_Base
	{
	ZENClassDeclare(ISerializerImporter, Serializer_Base)
	public:
		virtual eStatus	GetStatus(){return keStatus_Importing;}
	protected:

		virtual void	SetNextItem(zUInt auOffset)=0;
		virtual void	SkipToNextItem()=0;
	};
}

#endif
