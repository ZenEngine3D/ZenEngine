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
bool Serializer_Base::ItemSkip(ResourceData& aItem)
{
	++muItemFailed;
	return true;
}

//=================================================================================================
//! @brief		Called when starting a new ResourceData
//! @details	Entry point when a new resource is being serialized
//-------------------------------------------------------------------------------------------------
//! @param		aItem - ResourceData to serialize
//! @return		True if successful
//=================================================================================================
bool Serializer_Base::ItemSerialize(ResourceData& aItem)
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
//! @param		aItem - New ResourceData started
//! @return		True successful
//=================================================================================================
bool Serializer_Base::ItemStarted(ResourceData& aItem)
{
	++muItemProcessed;
	return true;
}

//=================================================================================================
//! @brief		Callback when a item completed serializing
//! @details	Child class can do special processing here
//-------------------------------------------------------------------------------------------------
//! @param		aItem - ResourceData completed
//! @return		True successful
//=================================================================================================
bool Serializer_Base::ItemEnded(ResourceData& aItem)
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
//! @param		_Value - ResourceData ResourceId to save/load
//! @return		True successful
//=================================================================================================
bool Serializer_Base::Serialize(zResID& _Value)
{	
	if( GetStatus() == keStatus_Exporting)
	{
		zenAssertMsg(_Value.IsExport(), "Can only save export ResourceData.");
		zResID ResId(_Value);
		ResId.SetSource(zenConst::keResSource_Loaded);
		return Serialize(*(zU64*)&ResId);	
	}
	return Serialize(*(zU64*)&_Value);	
}

//=================================================================================================
//! @brief		Callback when a new item started
//! @details	Exporter find out ResourceData size, before exporting it
//-------------------------------------------------------------------------------------------------
//! @param		aItem - New ResourceData started
//! @return		True successful
//=================================================================================================
bool ISerializerExporter::ItemStarted(ResourceData& aItem)
{
	Serializer_Base::ItemStarted(aItem);
	SerializerGetSize oGetItemSize;
	oGetItemSize.SerializeStart();
	bool bResult = oGetItemSize.ItemSerialize( aItem );
	oGetItemSize.SerializeStop();
	if( bResult )
	{
		aItem.muSize		= static_cast<zU32>(oGetItemSize.GetSerializeSize());
		aItem.mExportTime	= 0;
		aItem.muVersion		= zcDepot::ExportData.GetEngineVersion( aItem.mResID.GetType() );
		return true;
	}
	return false;
}


}

