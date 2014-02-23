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

		virtual bool	Serialize(awU8&						_uValue)=0;
		virtual bool	Serialize(awU16&						_uValue)=0;
		virtual bool	Serialize(awU32&						_uValue)=0;
		virtual bool	Serialize(awU64&						_uValue)=0;
		virtual bool	Serialize(awS8&						_iValue)=0;
		virtual bool	Serialize(awS16&						_iValue)=0;
		virtual bool	Serialize(awS32&						_iValue)=0;
		virtual bool	Serialize(awS64&						_iValue)=0;
		virtual bool	Serialize(awArrayBase<awU8>&	_Values)=0;
		virtual bool	Serialize(awArrayBase<awU16>&	_Values)=0;
		virtual bool	Serialize(awArrayBase<awU32>&	_Values)=0;
		virtual bool	Serialize(awArrayBase<awU64>&	_Values)=0;
		virtual bool	Serialize(awArrayBase<awS8>&	_Values)=0;
		virtual bool	Serialize(awArrayBase<awS16>&	_Values)=0;
		virtual bool	Serialize(awArrayBase<awS32>&	_Values)=0;
		virtual bool	Serialize(awArrayBase<awS64>&	_Values)=0;		

				bool	Serialize(awHash32&			_Value);
				bool	Serialize(awHash64&			_Value);
				bool	Serialize(ISerialize&				_Value);
				bool	Serialize(awResourceID&		_Value);

	protected:
		virtual bool	ItemStarted(SerialItem&				_Item);
		virtual bool	ItemEnded(SerialItem&				_Item);	
		virtual bool	ItemSkip(SerialItem&				_Item);	
		awU32				muItemProcessed;
		awU32				muItemFailed;
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
		awU32		muItemSize;
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

		virtual void	SetNextItem(awUInt auOffset)=0;
		virtual void	SkipToNextItem()=0;
	};
}

#endif
