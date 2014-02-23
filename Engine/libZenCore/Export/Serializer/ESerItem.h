#pragma once
#ifndef __LibExport_Serializer_SerialItem_h__
#define __LibExport_Serializer_SerialItem_h__

namespace EExp
{		
	class Serializer_Base;
	
	class ISerialize
	{
	AWClassDeclareNoParent(ISerialize)
	public:
		virtual				~ISerialize(){}
		virtual bool		Serialize( Serializer_Base& aSerializer )=0;
	};

	class SerialItem : public EExp::ISerialize
	{
	AWClassDeclare(SerialItem, EExp::ISerialize)
	public:
		struct Version
		{	
			Version():muEngine(awconst::keEngineVersion__Current),muMajor(0),muMinor(0){};
			Version(awU8 auMajor, awU8 auMinor):muEngine(awconst::keEngineVersion__Current),muMajor(auMajor),muMinor(auMinor){};
			Version(const Version& aCopy):muEngine(aCopy.muEngine),muMajor(aCopy.muMajor),muMinor(aCopy.muMinor){};
			bool IsValid( awconst::eResType _eResType )const {return muEngine==awconst::keEngineVersion__Current && SerialItem::sVersions[_eResType].muMajor == muMajor && SerialItem::sVersions[_eResType].muMinor == muMinor; }
			awU16 muEngine;
			awU8 muMajor;	
			awU8 muMinor;	
		};
									SerialItem();		
		virtual bool				Serialize( EExp::Serializer_Base& aSerializer );		

		awU32							muSize;
		Version						mVersion;
		awTimeStamp			mExportTime;		
		awResourceID			mResID;				
		static const Version		sVersions[awconst::keResType__Count];
	};
}

#endif