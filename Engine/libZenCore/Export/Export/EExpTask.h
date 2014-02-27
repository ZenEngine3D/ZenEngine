#pragma once
#ifndef __LibExport_Item_h__
#define __LibExport_Item_h__

namespace EExp
{	
	class ExportTask : public CJob::JobItem
	{
	ZENClassDeclare(ExportTask, CJob::JobItem)
	public:			
									ExportTask(EExp::ExportItem* _pExportItem, EExp::ExportInfoBase* _pExportInfo);		
		virtual bool				Start();
		virtual bool				THRTask_Execute();		
		virtual bool				End();
		
	protected:
		ExportItem*					mpExportItem;	//!< Serializable item to generate	
	};	
}

#endif