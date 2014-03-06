#pragma once
#if !defined(__LibExport_Graphics_GfxInputStreamBinding_DX11_h__) && AW_EXPORT_OR_RESOURCE_DX11 
#define __LibExport_Graphics_GfxInputStreamBinding_DX11_h__

namespace EExp
{	
	class SerialGfxInputStream_DX11 : public SerialInputStream_Base
	{
	ZENClassDeclare(SerialGfxInputStream_DX11, SerialInputStream_Base)
	//-------------------------------------------------------------------------------------------------
	// Serialization and Asset creation support
	//-------------------------------------------------------------------------------------------------
	public:		
		virtual bool				Serialize				( EExp::Serializer_Base& _Serializer );

		struct SerialUseOnly
		{
			zenResID		mVertexBufferID;
			zenResID		mShaderInputSignatureID;
		};
		
		SerialUseOnly				mSerial;

	//-------------------------------------------------------------------------------------------------
	// Export support section
	//-------------------------------------------------------------------------------------------------
		static zenResID	CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool				ExportWork(bool _bIsTHRTask);	
	};

}

#endif