#pragma once
#ifndef __LibExport_Serializer_SerialItem_h__
#define __LibExport_Serializer_SerialItem_h__

namespace EExp
{		
	class Serializer_Base;
	
	class ISerialize
	{
	ZENClassDeclareNoParent(ISerialize)
	public:
		virtual				~ISerialize(){}
		virtual bool		Serialize( Serializer_Base& aSerializer )=0;
	};

	class SerialItem : public EExp::ISerialize
	{
	ZENClassDeclare(SerialItem, EExp::ISerialize)
	public:
												SerialItem();		
		virtual bool							Serialize( EExp::Serializer_Base& aSerializer );		

		zU32									muSize;
		zU32									muVersion;
		zTimeStamp								mExportTime;		
		zResID									mResID;				
		static const zenConst::eEngineVersion	sVersions[zenConst::keResType__Count];
	};
}

#endif