#pragma once
#ifndef __zCore_Exp_Gfx_Surface_RenderTarget_h__
#define __zCore_Exp_Gfx_Surface_RenderTarget_h__

namespace zcExp
{
	class SerialRenderTarget_Base : public zcExp::ExportItem
	{
	ZENClassDeclare(SerialRenderTarget_Base, zcExp::ExportItem)
	public:
		struct ExportInfo : public zcExp::ExportInfoBase
		{				
			zenConst::eTextureFormat	meFormat;
			zVec2U16				mvDim;
			bool					mbSRGB;
		};

		zenConst::eTextureFormat	meFormat;	
		zVec2U16				mvDim;
		bool					mbSRGB;

	//-------------------------------------------------------------------------------------------------
	// Export support section
	//-------------------------------------------------------------------------------------------------
	protected:	
		virtual bool	ExportEnd();
	};

	zResID CreateGfxRenderTarget(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, bool _bSrgb=true);
}

#endif