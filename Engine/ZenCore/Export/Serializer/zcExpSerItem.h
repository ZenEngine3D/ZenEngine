#pragma once
#ifndef __zCore_Exp_Ser_SerialItem_h__
#define __zCore_Exp_Ser_SerialItem_h__

namespace zcExp
{		
	class Serializer_Base;
	
	class ISerialize
	{
	ZENClassDeclareNoParent(ISerialize)
	public:
		virtual				~ISerialize(){}
		virtual bool		Serialize( Serializer_Base& aSerializer )=0;
	};
	
	//! @todo cleanup delete this
	class SerialItem : public zcExp::ISerialize
	{
	ZENClassDeclare(SerialItem, zcExp::ISerialize)
	public:
												SerialItem();		
		virtual bool							Serialize( zcExp::Serializer_Base& aSerializer );		

		zU32									muSize;
		zU32									muVersion;
		zTimeStamp								mExportTime;
		zResID									mResID;
		static const zenConst::eEngineVersion	sVersions[zenConst::keResType__Count];
	};
	

}

#endif