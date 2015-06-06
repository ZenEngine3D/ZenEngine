#pragma once
#ifndef __zCore_Exp_Gfx_SerialGfxIndex_DX11_DX11_h__
#define __zCore_Exp_Gfx_SerialGfxIndex_DX11_DX11_h__

namespace zcExp
{	

class ExporterGfxIndexDX11_DX11 : public ExporterBase
{
ZENClassDeclare(ExporterGfxIndexDX11_DX11, ExporterBase)
public:				
typedef ResDataGfxIndexDX11	ResData;
typedef zGameRef<ResData>	ResDataRef;
								ExporterGfxIndexDX11_DX11(const ResDataRef& _rResData);		

protected:	
	virtual bool				ExportWork(bool _bIsTHRTask);	
	ResDataRef				mrResData;
};

}

#endif