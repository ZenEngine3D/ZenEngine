#pragma once
#ifndef __LibExport_Graphics_Surface_RenderTarget_h__
#define __LibExport_Graphics_Surface_RenderTarget_h__

namespace EExp
{
	class SerialRenderTarget_Base : public EExp::ExportItem
	{
	ZENClassDeclare(SerialRenderTarget_Base, EExp::ExportItem)
	public:
		struct ExportInfo : public EExp::ExportInfoBase
		{				
			zenConst::eTextureFormat	meFormat;
			zenVec2U16				mvDim;
			bool					mbSRGB;
		};

		zenConst::eTextureFormat	meFormat;	
		zenVec2U16				mvDim;
		bool					mbSRGB;

	//-------------------------------------------------------------------------------------------------
	// Export support section
	//-------------------------------------------------------------------------------------------------
	protected:	
		virtual bool	ExportEnd();
	};

	zenResID CreateGfxRenderTarget(zenConst::eTextureFormat _eFormat, zenVec2U16 _vDim, bool _bSrgb=true);
}

#endif