#pragma once
#if !defined(__LibExport_Graphics_Texture_DX11_h__) && AW_EXPORT_OR_RESOURCE_DX11
#define __LibExport_Graphics_Texture_DX11_h__

namespace EExp
{	
	class SerialGfxTexture2D_DX11 : public SerialTexture2D_Base
	{
	ZENClassDeclare(SerialGfxTexture2D_DX11, SerialTexture2D_Base)
	//-------------------------------------------------------------------------------------------------
	// Serialization and Asset creation support
	//-------------------------------------------------------------------------------------------------
	public:
		virtual bool					Serialize( EExp::Serializer_Base& _Serializer );	
		struct SerialUseOnly
		{
			zArrayStatic<MipInfo>		maMipData;
			zenConst::eTextureFormat		meFormat;
			zenFlagResTexCreate			mCreationFlags;
		};
		SerialUseOnly					mSerial;		
		
	//-------------------------------------------------------------------------------------------------
	// Export support section
	//-------------------------------------------------------------------------------------------------
	protected:	
		virtual bool					ExportWork(bool _bIsTHRTask);	
	};

}

#endif