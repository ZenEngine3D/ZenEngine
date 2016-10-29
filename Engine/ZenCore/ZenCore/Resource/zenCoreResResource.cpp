#include "zcCore.h"


namespace zen { namespace zenRes {	

zExportData::~zExportData()
{
	if( mResID.IsValid() )
		zcDepot::ExportData.Remove( mResID );
}

}} // namespace zen, zenRes


