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
	AWClassDeclareNoParent(Serializer_Base)
	public:
		enum eStatus{keStatus_Invalid, keStatus_GetSize, keStatus_Exporting, keStatus_Importing};		
						Serializer_Base();
		virtual 		~Serializer_Base(){}

		virtual void	SerializeStart();
		virtual void	SerializeStop();				
		virtual bool	ItemSerialize(SerialItem& aItem);
		virtual eStatus	GetStatus()=0;

		virtual bool	Serialize(zenU8&						_uValue)=0;
		virtual bool	Serialize(zenU16&						_uValue)=0;
		virtual bool	Serialize(zenU32&						_uValue)=0;
		virtual bool	Serialize(zenU64&						_uValue)=0;
		virtual bool	Serialize(zenS8&						_iValue)=0;
		virtual bool	Serialize(zenS16&						_iValue)=0;
		virtual bool	Serialize(zenS32&						_iValue)=0;
		virtual bool	Serialize(zenS64&						_iValue)=0;
		virtual bool	Serialize(zenArrayBase<zenU8>&	_Values)=0;
		virtual bool	Serialize(zenArrayBase<zenU16>&	_Values)=0;
		virtual bool	Serialize(zenArrayBase<zenU32>&	_Values)=0;
		virtual bool	Serialize(zenArrayBase<zenU64>&	_Values)=0;
		virtual bool	Serialize(zenArrayBase<zenS8>&	_Values)=0;
		virtual bool	Serialize(zenArrayBase<zenS16>&	_Values)=0;
		virtual bool	Serialize(zenArrayBase<zenS32>&	_Values)=0;
		virtual bool	Serialize(zenArrayBase<zenS64>&	_Values)=0;		

				bool	Serialize(zenHash32&			_Value);
				bool	Serialize(zenHash64&			_Value);
				bool	Serialize(ISerialize&				_Value);
				bool	Serialize(zenResID&		_Value);

	protected:
		virtual bool	ItemStarted(SerialItem&				_Item);
		virtual bool	ItemEnded(SerialItem&				_Item);	
		virtual bool	ItemSkip(SerialItem&				_Item);	
		zenU32				muItemProcessed;
		zenU32				muItemFailed;
	};
	
	//=================================================================================================
	//! @class		Class that want to serialize items should be derived from this one
	//=================================================================================================
	class ISerializerExporter : public Serializer_Base
	{	
	AWClassDeclare(ISerializerExporter, Serializer_Base)
	public:
		virtual eStatus	GetStatus(){return keStatus_Exporting;}
		virtual bool	ItemStarted(SerialItem& aItem);
	
	protected:
		zenU32		muItemSize;
	};
	
	//=================================================================================================
	//! @class		Class that want to deserialize items should be derived from this one
	//=================================================================================================
	class ISerializerImporter : public Serializer_Base
	{
	AWClassDeclare(ISerializerImporter, Serializer_Base)
	public:
		virtual eStatus	GetStatus(){return keStatus_Importing;}
	protected:

		virtual void	SetNextItem(zenUInt auOffset)=0;
		virtual void	SkipToNextItem()=0;
	};
}

#endif
