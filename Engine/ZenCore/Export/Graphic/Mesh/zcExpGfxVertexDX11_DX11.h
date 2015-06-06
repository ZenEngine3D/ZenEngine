#pragma once
#ifndef __zCore_Exp_Gfx_SerialGfxVertex_DX11_DX11_h__
#define __zCore_Exp_Gfx_SerialGfxVertex_DX11_DX11_h__

namespace zcExp
{

class ExporterGfxVertexDX11_DX11 : public ExporterBase
{
ZENClassDeclare(ExporterGfxVertexDX11_DX11, ExporterBase)
public:				
typedef ResDataGfxVertexDX11	ResData;
typedef zGameRef<ResData>	ResDataRef;
								ExporterGfxVertexDX11_DX11(const ResDataRef& _rResData);
protected:	
	virtual bool				ExportWork(bool _bIsTHRTask);
	ResDataRef				mrResData;
};

}

#endif