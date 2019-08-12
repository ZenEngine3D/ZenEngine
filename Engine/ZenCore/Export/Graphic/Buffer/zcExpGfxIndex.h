#pragma once

namespace zcExp
{
	struct ExportInfoGfxIndex : public ExportInfoBase
	{			
		zArrayDyn<zU16>		maIndice16;			//!< 16bits Indices list (Pick 1st one valid)
		zArrayDyn<zU32>		maIndice32;			//!< 32bits Indices list
		ePrimitiveType		mePrimitiveType;	//!< Type of primitive the indices are for
	};
	
	class ExportGfxIndex : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxIndex, zenRes::zExportData)
	public:
		zArrayDyn<zU8>		maIndices;			//!< array of indices (packing either 16bits or 32bits index)
		zU32				muPrimitiveCount;	//!< Number of primitives generated by indices
		zU32				muIndiceCount;		//!< Number of indices in array (maIndices.Count/muIndiceSize)	
		zU16				muIndiceSize;		//!< Size of each indices (2Bytes, 4Bytes, ...)		
	};

	zResID CreateGfxIndexBuffer(const zArray<zU16>& _Indices, ePrimitiveType _ePrimitiveType);
	zResID CreateGfxIndexBuffer(const zArray<zU32>& _Indices, ePrimitiveType _ePrimitiveType);
}
