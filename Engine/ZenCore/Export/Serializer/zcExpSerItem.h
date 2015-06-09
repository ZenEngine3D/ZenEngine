#pragma once
#ifndef __zCore_Exp_Ser_ResourceData_h__
#define __zCore_Exp_Ser_ResourceData_h__

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
	
	class ResourceData : public ISerialize, public zRefCounted
	{
	ZENClassDeclare(ResourceData, ISerialize)
	public:
												ResourceData();		
		virtual bool							Serialize( zcExp::Serializer_Base& aSerializer );		

		zU32									muSize;
		zU32									muVersion;
		zTimeStamp								mExportTime;
		zResID									mResID;		
	};

}

#endif