#pragma once
#ifndef __LibResource_Graphics_GfxView_DX11_h__
#define __LibResource_Graphics_GfxView_DX11_h__

namespace ERes
{
	class GfxView_DX11 : public Resource
	{
		struct InstanceInfo : public EExp::SerialGfxView_Base
		{
			zenArrayStatic<GfxRenderTargetRef>	marTargetColor;
			GfxRenderTargetRef					mrTargetDepth;
		};
		AWResourceDeclare(GfxView_DX11, InstanceInfo, awconst::keResType_GfxView)
	//------------------------------------------------------------------
	// Common to all RenderTarget
	//------------------------------------------------------------------
	public:
		void						Clear( bool _bClearColor, const zenVec4F& _vRGBA, bool _bClearDepth=true, float _fDepth=1, bool _bClearStencil=false, zenU8 _uStencil=0 );
		
	//------------------------------------------------------------------
	// Platform Implementation
	//------------------------------------------------------------------
	public:
		virtual						~GfxView_DX11();
		virtual bool				ResourceInit();		

		
	};
}

#endif