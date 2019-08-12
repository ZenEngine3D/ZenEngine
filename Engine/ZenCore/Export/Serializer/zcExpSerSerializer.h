#pragma once

namespace zcExp
{			
	//=================================================================================================
	// Forward declares
	//=================================================================================================
	class Serializer_Base;
	class ResourceData;

	//=================================================================================================
	//! @class		Base class used to serialize/deserialize items
	//=================================================================================================
	class Serializer_Base
	{
	zenClassDeclareNoParent(Serializer_Base)
	public:
		enum eStatus{keStatus_Invalid, keStatus_GetSize, keStatus_Exporting, keStatus_Importing};		
						Serializer_Base();
		virtual 		~Serializer_Base(){}

		virtual void	SerializeStart();
		virtual void	SerializeStop();				
		virtual bool	ItemSerialize(ResourceData& aItem);
		virtual eStatus	GetStatus()=0;

		virtual bool	Serialize(zU8&				_uValue)=0;
		virtual bool	Serialize(zU16&				_uValue)=0;
		virtual bool	Serialize(zU32&				_uValue)=0;
		virtual bool	Serialize(zU64&				_uValue)=0;
		virtual bool	Serialize(zI8&				_iValue)=0;
		virtual bool	Serialize(zI16&				_iValue)=0;
		virtual bool	Serialize(zI32&				_iValue)=0;
		virtual bool	Serialize(zI64&				_iValue)=0;
		virtual bool	Serialize(zArray<zU8>&	_Values)=0;
		virtual bool	Serialize(zArray<zU16>&	_Values)=0;
		virtual bool	Serialize(zArray<zU32>&	_Values)=0;
		virtual bool	Serialize(zArray<zU64>&	_Values)=0;
		virtual bool	Serialize(zArray<zI8>&	_Values)=0;
		virtual bool	Serialize(zArray<zI16>&	_Values)=0;
		virtual bool	Serialize(zArray<zI32>&	_Values)=0;
		virtual bool	Serialize(zArray<zI64>&	_Values)=0;		

				bool	Serialize(zHash32&			_Value);
				bool	Serialize(zHash64&			_Value);
				bool	Serialize(ISerialize&		_Value);
				bool	Serialize(zResID&			_Value);

	protected:
		virtual bool	ItemStarted(ResourceData&		_Item);
		virtual bool	ItemEnded(ResourceData&		_Item);	
		virtual bool	ItemSkip(ResourceData&		_Item);	
		zU32			muItemProcessed;
		zU32			muItemFailed;
	};
	
	//=================================================================================================
	//! @class		Class that want to serialize items should be derived from this one
	//=================================================================================================
	class ISerializerExporter : public Serializer_Base
	{	
	zenClassDeclare(ISerializerExporter, Serializer_Base)
	public:
		virtual eStatus	GetStatus(){return keStatus_Exporting;}
		virtual bool	ItemStarted(ResourceData& aItem);
	
	protected:
		zU32			muItemSize;
	};
	
	//=================================================================================================
	//! @class		Class that want to deserialize items should be derived from this one
	//=================================================================================================
	class ISerializerImporter : public Serializer_Base
	{
	zenClassDeclare(ISerializerImporter, Serializer_Base)
	public:
		virtual eStatus	GetStatus(){return keStatus_Importing;}
	protected:

		virtual void	SetNextItem(zUInt auOffset)=0;
		virtual void	SkipToNextItem()=0;
	};
}
