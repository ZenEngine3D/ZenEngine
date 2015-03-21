#pragma once
#ifndef __zCore_Exp_Gfx_ShaderBinding_Base_h__
#define __zCore_Exp_Gfx_ShaderBinding_Base_h__

namespace zcExp
{
	class SerialShaderBinding_Base : public zcExp::ExportItem
	{
	ZENClassDeclare(SerialShaderBinding_Base,zcExp::ExportItem)
	public:
		struct ExportInfo : public zcExp::ExportInfoBase
		{						
			zArrayStatic<zResID>	maShaderID;				//!< List of shaders to tie together (vertex, pixel, ...)			
			zMap<zU32>::Key64				mdStagePerParamDef;		//!< ParamDef needed for this binding, with stage mask they applied to (automatically computed at export)
		};
		virtual bool		ExportStart();
		static zResID	CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	};

	zResID CreateGfxShaderBinding(const zArrayBase<zResID>& _aShaderID);
	
}

#include "zcExpGfxShaderBindingDX11.h"


#endif
