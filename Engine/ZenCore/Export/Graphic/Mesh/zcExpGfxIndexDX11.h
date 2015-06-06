#pragma once
#if !defined(__zCore_Exp_Gfx_SerialGfxIndex_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_Gfx_SerialGfxIndex_DX11_h__

namespace zcExp
{	

class ResDataGfxIndexDX11 : public ResDataBase
{
ZENClassDeclare(ResDataGfxIndexDX11, ResDataBase)
public:
	virtual bool				Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
	zArrayStatic<zU8>			maIndices;			//!< array of indices (packing either 16bits or 32bits index)
	D3D11_PRIMITIVE_TOPOLOGY	mePrimitiveType;	//!< Primitive type (TRIANGLELIST, TRIANGLESTRIP, ...)
	DXGI_FORMAT					meIndiceFormat;		//!< Indice size (DXGI_FORMAT_R16_UINT,DXGI_FORMAT_R32_UINT, ...)
	zU32						muIndiceCount;		//!< Number of indice in array (maIndices.Count/muIndiceSize)	
	zU16						muIndiceSize;		//!< Size of each indice (2Bytes, 4Bytes, ...)
	zU32						muPrimitiveCount;	//!< Number of primitives generated by indices
};

}

#endif