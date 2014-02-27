#pragma once
#ifndef __LibFramework_Api_Type_Vertex2D_h__
#define __LibFramework_Api_Type_Vertex2D_h__

namespace zen { namespace zenType {

template<class _TypeV_, class _TypeC_>
class awVertex2DSimple
{
ZENClassDeclareNoParent(awVertex2DSimple)
public:
	AWForceInline awVertex2DSimple(const _TypeV_& inX, const _TypeV_& inY, const _TypeV_& inZ, const _TypeC_& inColour);
	AWForceInline awVertex2DSimple(void);

	AWForceInline void SetPosition(const _TypeV_& inX, const _TypeV_& inY, const _TypeV_& inZ);
	AWForceInline void Set(const _TypeV_& inX, const _TypeV_& inY, const _TypeV_& inZ, const _TypeC_& inColour);

	_TypeV_ mX, mY, mZ;
	_TypeC_ mColour;
};

template<class _TypeV_, class _TypeC_, class _TypeM_, class _TypeUV>
class awVertex2DFull : public awVertex2DSimple<_TypeV_, _TypeC_>
{
public:
	AWForceInline awVertex2DFull(const _TypeV_& inX, const _TypeV_& inY, const _TypeV_& inZ, const _TypeC_& inColour, const _TypeM_& inMask, const _TypeUV& inU, const _TypeUV& inV);
	AWForceInline awVertex2DFull(void);

	AWForceInline void Set(const _TypeV_& inX, const _TypeV_& inY, const _TypeV_& inZ, const _TypeC_& inColour, const _TypeM_& inMask, const _TypeUV& inU, const _TypeUV& inV);

	_TypeM_ mMask;
	_TypeUV mU, mV;
};


typedef awVertex2DSimple<zenS32, zenU32>					zenVec2DSimple;	ZENSupportMemcopy(zenVec2DSimple);
typedef awVertex2DFull  <zenS32, zenU32, zenU32, zenS32>	zenVec2DFull;	ZENSupportMemcopy(zenVec2DFull);

}}  //namespace zen, Type 

#include "zenTypeVertex2D.inl"

#endif
