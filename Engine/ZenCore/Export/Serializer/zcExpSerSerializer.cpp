#include "zcCore.h"

namespace zcExp
{
//=================================================================================================
//! @brief		
//! @details	
//-------------------------------------------------------------------------------------------------
//=================================================================================================
Serializer_Base::Serializer_Base()
: muItemProcessed(0)
, muItemFailed(0)
{
}

//=================================================================================================
//! @brief		Called before we start Serializing items
//! @details	Child class should initialize itself, before a batch export/import
//=================================================================================================
void Serializer_Base::SerializeStart()
{
	muItemProcessed = 0;
	muItemFailed	= 0;
}

//=================================================================================================
//! @brief		Called before when we're done Serializing items
//! @details	Child class should free up resources here
//=================================================================================================
void Serializer_Base::SerializeStop()
{
}

//=================================================================================================
//! @brief		Called when an item failed to serialized
//! @details	Childclass should skip to next item here
//=================================================================================================
bool Serializer_Base::ItemSkip(SerialItem& aItem)
{
	++muItemFailed;
	return true;
}

//=================================================================================================
//! @brief		Called when starting a new SerialItem
//! @details	Entry point when a new resource is being serialized
//-------------------------------------------------------------------------------------------------
//! @param		aItem - SerialItem to serialize
//! @return		True if successful
//=================================================================================================
bool Serializer_Base::ItemSerialize(SerialItem& aItem)
{
	if( ItemStarted(aItem) && Serialize(aItem) )
	{
		return ItemEnded(aItem);
	}
	else
	{
		ItemSkip(aItem);
		return false;
	}
}

//=================================================================================================
//! @brief		Callback when a new item started
//! @details	Child class can do special processing here
//-------------------------------------------------------------------------------------------------
//! @param		aItem - New SerialItem started
//! @return		True successful
//=================================================================================================
bool Serializer_Base::ItemStarted(SerialItem& aItem)
{
	++muItemProcessed;
	return true;
}

//=================================================================================================
//! @brief		Callback when a item completed serializing
//! @details	Child class can do special processing here
//-------------------------------------------------------------------------------------------------
//! @param		aItem - SerialItem completed
//! @return		True successful
//=================================================================================================
bool Serializer_Base::ItemEnded(SerialItem& aItem)
{
	return true;
}	

//=================================================================================================
//! @brief		Serialize a zHash32
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param		_Value - value to serialize
//! @return		True successful
//=================================================================================================
bool Serializer_Base::Serialize(zHash32&	_Value)
{
	return Serialize(*(zU32*)&_Value);
}

//=================================================================================================
//! @brief		Serialize a zHash64
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param		_Value - value to serialize
//! @return		True successful
//=================================================================================================
bool Serializer_Base::Serialize(zHash64&	_Value)
{
	return Serialize(*(zU64*)&_Value);
}

//=================================================================================================
//! @brief		Serialize a ISerialize child class
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param		_Value - value to serialize
//! @return		True successful
//=================================================================================================
bool Serializer_Base::Serialize(ISerialize&	_Value)
{
	return _Value.Serialize(*this);
}

//=================================================================================================
//! @brief		Serialize a zResID
//! @details	Will convert the source from 'Export' to 'Loaded'
//-------------------------------------------------------------------------------------------------
//! @param		_Value - SerialItem ResourceId to save/load
//! @return		True successful
//=================================================================================================
bool Serializer_Base::Serialize(zResID& _Value)
{	
	if( GetStatus() == keStatus_Exporting)
	{
		ZENAssertMsg(_Value.IsExport(), "Can only save export SerialItem.");
		zResID ResId(_Value);
		ResId.SetSource(zenConst::keResSource_Loaded);
		return Serialize(*(zU64*)&ResId);	
	}
	return Serialize(*(zU64*)&_Value);	
}

//=================================================================================================
//! @brief		Callback when a new item started
//! @details	Exporter find out SerialItem size, before exporting it
//-------------------------------------------------------------------------------------------------
//! @param		aItem - New SerialItem started
//! @return		True successful
//=================================================================================================
bool ISerializerExporter::ItemStarted(SerialItem& aItem)
{
	Serializer_Base::ItemStarted(aItem);
	SerializerGetSize oGetItemSize;
	oGetItemSize.SerializeStart();
	bool bResult = oGetItemSize.ItemSerialize( aItem );
	oGetItemSize.SerializeStop();
	if( bResult )
	{
		aItem.muSize		= oGetItemSize.GetSerializeSize();			
		aItem.mExportTime	= 0;
		aItem.muVersion		= SerialItem::sVersions[aItem.mResID.meType];
		return true;
	}
	return false;
}


}

