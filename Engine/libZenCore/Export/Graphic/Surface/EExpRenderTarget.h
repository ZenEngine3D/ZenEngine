#pragma once
#ifndef __LibExport_Graphics_Surface_RenderTarget_h__
#define __LibExport_Graphics_Surface_RenderTarget_h__

namespace EExp
{
	class SerialRenderTarget_Base : public EExp::ExportItem
	{
	AWClassDeclare(SerialRenderTarget_Base, EExp::ExportItem)
	public:
		struct ExportInfo : public EExp::ExportInfoBase
		{				
			awconst::eTextureFormat	meFormat;
			zenVec2U16				mvDim;
			bool					mbSRGB;
		};

		awconst::eTextureFormat	meFormat;	
		zenVec2U16				mvDim;
		bool					mbSRGB;

	//-------------------------------------------------------------------------------------------------
	// Export support section
	//-------------------------------------------------------------------------------------------------
	protected:	
		virtual bool	ExportEnd();
	};

	zenResID CreateGfxRenderTarget(awconst::eTextureFormat _eFormat, zenVec2U16 _vDim, bool _bSrgb=true);
}

#endif