#pragma once
#ifndef __LibFramework_Api_Type_Vertex2D_h__
#define __LibFramework_Api_Type_Vertex2D_h__

namespace zen { namespace awtype {

template<class _TypeV_, class _TypeC_>
class awVertex2DSimple
{
AWClassDeclareNoParent(awVertex2DSimple)
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


typedef awVertex2DSimple<awS32, awU32>					awVec2DSimple;	AWAddSupportsMemCopy(awVec2DSimple);
typedef awVertex2DFull  <awS32, awU32, awU32, awS32>	awVec2DFull;	AWAddSupportsMemCopy(awVec2DFull);

}}  //namespace zen, Type 

#include "zenTypeVertex2D.inl"

#endif
