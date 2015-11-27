#pragma once
#ifndef __zCore_Exp_Gfx_MeshStrip_Base_h__
#define __zCore_Exp_Gfx_MeshStrip_Base_h__

namespace zcExp
{
	struct ExportInfoGfxMeshStrip : public ExportInfoBase
	{
		zResID									mVertexBufferID;
		zResID									mIndexBufferID;
		zResID									mShaderBindingID;			
		zArrayStatic<zenRes::zShaderTexture>	maTexture;				//!< List of textures assigned to shader
		zArrayStatic<zResID>					maShaderParamID;		//!< List of Shader Param Instance (1 per ParamDef entry)
		zU32									muVertexFirst;
		zU32									muIndexFirst;
		zU32									muIndexCount;
	};
	
	zResID CreateGfxMeshStrip(zResID _VertexBufferID, zResID _IndexBufferID, zResID _ShaderBindingID, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF, zU32 _uVertexFirst = 0, const zArrayBase<zResID>& _aShaderParamID=zArrayStatic<zResID>(), const zArrayBase<zenRes::zShaderTexture>& _aTexture=zArrayStatic<zenRes::zShaderTexture>());
}

#include "zcExpGfxMeshStripDX11.h"

#endif