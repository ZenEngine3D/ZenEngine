#pragma once

//! @todo Clean : move this to proper location and implement it
zenInline bool zenIsResourceCreationThread()
{
	return true;
}

namespace zcRes
{	
	template<class TResourceRef>
	ResourceAnyRef RuntimeExport(zcExp::ExportInfoBase& _ExportInfo)
	{
		typedef TResourceRef::Class::Super::RuntimeExporter ExporterClass;
		TResourceRef rResource		= zenMem::NewPool<TResourceRef::Class>();

		ExporterClass Exporter(rResource.HAL());
		Exporter.Export(_ExportInfo);
		if( _ExportInfo.IsSuccess() && rResource.HAL()->Initialize() )
			return rResource;

		return nullptr;
	}

	//=============================================================================================
	//! @brief	When a resource not created through exporter, 
	//!			call this method to finalize its initialization
	//=============================================================================================
	zenInline void RuntimeCreateFinalize( zenRes::zExportData& _NewResource, zenConst::eResType _eResType )
	{
		_NewResource.mResID			= zcMgr::Export.GetNewResourceID(_eResType);
		_NewResource.muVersion		= zcDepot::ExportData.GetEngineVersion(_eResType);
		_NewResource.mExportTime	= zenSys::GetTimeStamp();			
		zcDepot::ExportData.Set(&_NewResource);
	}
}
