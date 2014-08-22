#include "libZenCore.h"

namespace EExp
{
	const zenConst::eEngineVersion SerialItem::sVersions[]=
	{
		zenConst::keEngineVersion_Initial,		// keItem_GfxShaderVertex_DX11,
		zenConst::keEngineVersion_Initial,		// keItem_GfxShaderPixel_DX11,
		zenConst::keEngineVersion_Initial,		// keItem_ShaderVertex_GLSL4,
		zenConst::keEngineVersion_Initial,		// keItem_ShaderPixel_GLSL4,
	};

	SerialItem::SerialItem()
	: muSize(0)
	, mExportTime(0)	
	, muVersion(zenConst::keEngineVersion__Current)
	{
		ZENStaticAssert( ZENArrayCount(sVersions)==zenConst::keResType__Count );
	}

	bool SerialItem::Serialize( EExp::Serializer_Base& aSerializer )
	{				
		bool bSuccess	 = aSerializer.Serialize(muSize);		
		bSuccess		&= aSerializer.Serialize(mResID);		
		bSuccess		&= aSerializer.Serialize(muVersion);
		bSuccess		&= aSerializer.Serialize(mExportTime);		
		return bSuccess && muVersion <= zUInt(sVersions[mResID.Type()]);
	}
}