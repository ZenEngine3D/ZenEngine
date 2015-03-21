#pragma once
#if !defined(__zCore_Exp_Gfx_GfxInputStreamBinding_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11 
#define __zCore_Exp_Gfx_GfxInputStreamBinding_DX11_h__

namespace zcExp
{	
	class SerialGfxInputStream_DX11 : public SerialInputStream_Base
	{
	ZENClassDeclare(SerialGfxInputStream_DX11, SerialInputStream_Base)
	//-------------------------------------------------------------------------------------------------
	// Serialization and Asset creation support
	//-------------------------------------------------------------------------------------------------
	public:		
		virtual bool				Serialize				( zcExp::Serializer_Base& _Serializer );

		struct SerialUseOnly
		{
			zResID		mVertexBufferID;
			zResID		mShaderInputSignatureID;
		};
		
		SerialUseOnly				mSerial;

	//-------------------------------------------------------------------------------------------------
	// Export support section
	//-------------------------------------------------------------------------------------------------
		static zResID	CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool				ExportWork(bool _bIsTHRTask);	
	};

}

#endif