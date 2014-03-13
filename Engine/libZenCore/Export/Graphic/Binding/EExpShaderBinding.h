#pragma once
#ifndef __LibExport_Graphics_ShaderBinding_Base_h__
#define __LibExport_Graphics_ShaderBinding_Base_h__

namespace EExp
{
	class SerialShaderBinding_Base : public EExp::ExportItem
	{
	ZENClassDeclare(SerialShaderBinding_Base,EExp::ExportItem)
	public:
		struct ExportInfo : public EExp::ExportInfoBase
		{						
			zArrayStatic<zResID>	maShaderID;				//!< List of shaders to tie together (vertex, pixel, ...)			
			zMap<zU32>::Key64				mdStagePerParamDef;		//!< ParamDef needed for this binding, with stage mask they applied to (automatically computed at export)
		};
		virtual bool		ExportStart();
		static zResID	CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	};

	zResID CreateGfxShaderBinding(const zArrayBase<zResID>& _aShaderID);
	
}

#include "EExpShaderBindingDX11.h"


#endif
