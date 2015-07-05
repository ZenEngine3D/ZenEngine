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
		virtual bool		Serialize( zcExp::Serializer_Base& aSerializer );		

		zU32				muSize;			//!< Serialized data size
		zU32				muVersion;		//!< Resource version ID (Based on engine used to export it)
		zTimeStamp			mExportTime;	//!< When resource was exported
		zResID				mResID;			//!< Resource unique ID
	};

}

#endif