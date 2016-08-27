#pragma once
#ifndef __zCore_Exp_Gfx_Buffer_h__
#define __zCore_Exp_Gfx_Buffer_h__

namespace zcExp
{	
	struct ExportInfoGfxBuffer : public ExportInfoBase
	{			
		zArrayStatic<zU8>			maData;
		zU32						muElementSize;
		zU32						muElementCount;
		zFlagResTexUse				mUseFlags;
	};
	
	class ExportGfxBuffer : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxBuffer, zenRes::zExportData)
	public:
		zArrayStatic<zU8>			maData;	
		zU32						muElementSize;
		zU32						muElementCount;
		zU32						muElementStride;
		zFlagResTexUse				mUseFlags;
	};

	zResID CreateGfxBuffer(const zU8* _pData, zUInt _uDataSize, zUInt _uElemSize, zU32 _uElemCount/*, zFlagResTexUse _UseFlags*/);	//! @brief Create a new Buffer Resource
}

#endif
