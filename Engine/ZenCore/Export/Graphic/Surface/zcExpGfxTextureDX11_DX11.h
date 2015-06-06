#pragma once
#ifndef __zCore_Exp_Gfx_Texture2d_DX11_DX11_h__
#define __zCore_Exp_Gfx_Texture2d_DX11_DX11_h__

namespace zcExp
{	

class ExporterGfxTexture2dDX11_DX11 : public ExporterBase
{
ZENClassDeclare(ExporterGfxTexture2dDX11_DX11, ExporterBase)
public:				
typedef ResDataGfxTexture2dDX11	ResData;
typedef zGameRef<ResData>		ResDataRef;
									ExporterGfxTexture2dDX11_DX11(const ResDataRef& _rResData);
protected:	
	virtual bool					ExportWork(bool _bIsTHRTask);	
	ResDataRef					mrResData;
};

}

#endif
