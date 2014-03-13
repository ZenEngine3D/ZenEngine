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
		struct Version
		{	
			Version():muEngine(zenConst::keEngineVersion__Current),muMajor(0),muMinor(0){};
			Version(zU8 auMajor, zU8 auMinor):muEngine(zenConst::keEngineVersion__Current),muMajor(auMajor),muMinor(auMinor){};
			Version(const Version& aCopy):muEngine(aCopy.muEngine),muMajor(aCopy.muMajor),muMinor(aCopy.muMinor){};
			bool IsValid( zenConst::eResType _eResType )const {return muEngine==zenConst::keEngineVersion__Current && SerialItem::sVersions[_eResType].muMajor == muMajor && SerialItem::sVersions[_eResType].muMinor == muMinor; }
			zU16 muEngine;
			zU8 muMajor;	
			zU8 muMinor;	
		};
									SerialItem();		
		virtual bool				Serialize( EExp::Serializer_Base& aSerializer );		

		zU32							muSize;
		Version						mVersion;
		zTimeStamp			mExportTime;		
		zResID			mResID;				
		static const Version		sVersions[zenConst::keResType__Count];
	};
}

#endif