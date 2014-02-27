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
			Version():muEngine(awconst::keEngineVersion__Current),muMajor(0),muMinor(0){};
			Version(zenU8 auMajor, zenU8 auMinor):muEngine(awconst::keEngineVersion__Current),muMajor(auMajor),muMinor(auMinor){};
			Version(const Version& aCopy):muEngine(aCopy.muEngine),muMajor(aCopy.muMajor),muMinor(aCopy.muMinor){};
			bool IsValid( awconst::eResType _eResType )const {return muEngine==awconst::keEngineVersion__Current && SerialItem::sVersions[_eResType].muMajor == muMajor && SerialItem::sVersions[_eResType].muMinor == muMinor; }
			zenU16 muEngine;
			zenU8 muMajor;	
			zenU8 muMinor;	
		};
									SerialItem();		
		virtual bool				Serialize( EExp::Serializer_Base& aSerializer );		

		zenU32							muSize;
		Version						mVersion;
		zenTimeStamp			mExportTime;		
		zenResID			mResID;				
		static const Version		sVersions[awconst::keResType__Count];
	};
}

#endif