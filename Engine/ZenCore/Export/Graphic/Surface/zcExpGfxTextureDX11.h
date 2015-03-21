#pragma once
#if !defined(__zCore_Exp_Gfx_Texture_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_Gfx_Texture_DX11_h__

namespace zcExp
{	
	class SerialGfxTexture2D_DX11 : public SerialTexture2D_Base
	{
	ZENClassDeclare(SerialGfxTexture2D_DX11, SerialTexture2D_Base)
	//-------------------------------------------------------------------------------------------------
	// Serialization and Asset creation support
	//-------------------------------------------------------------------------------------------------
	public:
		virtual bool					Serialize( zcExp::Serializer_Base& _Serializer );	
		struct SerialUseOnly
		{
			zArrayStatic<MipInfo>		maMipData;
			zenConst::eTextureFormat		meFormat;
			zFlagResTexUse			mCreationFlags;
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