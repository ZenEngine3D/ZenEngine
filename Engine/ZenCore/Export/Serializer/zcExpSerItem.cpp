#include "zcCore.h"

namespace zcExp
{
		
	ResourceData::ResourceData()
	: muSize(0)
	, mExportTime(0)	
	, muVersion(zenConst::keEngineVersion__Current)
	{

	}

	bool ResourceData::Serialize( zcExp::Serializer_Base& aSerializer )
	{				
		bool bSuccess	 = aSerializer.Serialize(muSize);		
		bSuccess		&= aSerializer.Serialize(mResID);		
		bSuccess		&= aSerializer.Serialize(muVersion);
		bSuccess		&= aSerializer.Serialize(mExportTime);		
		return bSuccess && muVersion <= zUInt(zcDepot::ResourceData.GetEngineVersion(mResID.GetType()));
	}
}