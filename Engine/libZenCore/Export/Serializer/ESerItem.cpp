#include "libZenCore.h"

namespace EExp
{
	const SerialItem::Version SerialItem::sVersions[]=
	{
		//							Major		Minor		
		SerialItem::Version(		0,			1		),		// keItem_GfxShaderVertex_DX11,
		SerialItem::Version(		0,			1		),		// keItem_GfxShaderPixel_DX11,
		SerialItem::Version(		0,			1		),		// keItem_ShaderVertex_GLSL4,
		SerialItem::Version(		0,			1		),		// keItem_ShaderPixel_GLSL4,
	};

	SerialItem::SerialItem()
	: muSize(0)
	, mExportTime(0)	
	, mVersion(0,0)
	{
		AWStaticAssert( AWArrayCount(sVersions)==awconst::keResType__Count );
	}

	bool SerialItem::Serialize( EExp::Serializer_Base& aSerializer )
	{				
		bool bSuccess	 = aSerializer.Serialize(muSize);		
		bSuccess		&= aSerializer.Serialize(mResID);		
		bSuccess		&= aSerializer.Serialize(mVersion.muEngine);
		bSuccess		&= aSerializer.Serialize(mVersion.muMajor);
		bSuccess		&= aSerializer.Serialize(mVersion.muMinor);		
		bSuccess		&= aSerializer.Serialize(mExportTime);
		return bSuccess && mVersion.IsValid(mResID.Type());
	}
}