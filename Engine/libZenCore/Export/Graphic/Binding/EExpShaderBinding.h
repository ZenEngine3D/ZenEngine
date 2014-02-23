#pragma once
#ifndef __LibExport_Graphics_ShaderBinding_Base_h__
#define __LibExport_Graphics_ShaderBinding_Base_h__

namespace EExp
{
	class SerialShaderBinding_Base : public EExp::ExportItem
	{
	AWClassDeclare(SerialShaderBinding_Base,EExp::ExportItem)
	public:
		struct ExportInfo : public EExp::ExportInfoBase
		{						
			zenArrayStatic<zenResID>	maShaderID;				//!< List of shaders to tie together (vertex, pixel, ...)			
			zenMap<zenU32>::Key64				mdStagePerParamDef;		//!< ParamDef needed for this binding, with stage mask they applied to (automatically computed at export)
		};
		virtual bool		ExportStart();
		static zenResID	CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	};

	zenResID CreateGfxShaderBinding(const zenArrayBase<zenResID>& _aShaderID);
	
}

#include "EExpShaderBindingDX11.h"


#endif
