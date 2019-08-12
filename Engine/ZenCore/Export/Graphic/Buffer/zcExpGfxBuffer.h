#pragma once

namespace zcExp
{	
	struct ExportInfoGfxBuffer : public ExportInfoBase
	{			
		zArrayDyn<zU8>	maData;
		zU32			muElementSize;
		zU32			muElementCount;
		zFlagResTexUse	mUseFlags;
	};
	
	class ExportGfxBuffer : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxBuffer, zenRes::zExportData)
	public:
		zArrayDyn<zU8>	maData;	
		zU32			muElementSize;
		zU32			muElementCount;
		zU32			muElementStride;
		zFlagResTexUse	mUseFlags;
	};

	zResID CreateGfxBuffer(const zU8* _pData, size_t _uDataSize, size_t _uElemSize, zU32 _uElemCount/*, zFlagResTexUse _UseFlags*/);	//! @brief Create a new StructBuffer Resource
}
