#include "zcCore.h"


namespace zen { namespace zenRes {	

zExportData::~zExportData()
{
	zcDepot::ExportData.Remove( mResID );
}

}} // namespace zen, zenRes


