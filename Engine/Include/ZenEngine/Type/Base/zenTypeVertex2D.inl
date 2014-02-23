#pragma once
#ifndef __LibFramework_Api_Type_Vertex2D_inl__
#define __LibFramework_Api_Type_Vertex2D_inl__

namespace zen { namespace awtype {

template<>
AWForceInline awVertex2DSimple<awS32, awU32>::awVertex2DSimple(const awS32& inX, const awS32& inY, const awS32& inZ, const awU32& inColour)
: mX( inX << 1 )
, mY(-inY << 1 )
, mZ( inZ)
, mColour(inColour)
{
}

template<>
AWForceInline awVertex2DSimple<awS32, awU32>::awVertex2DSimple(void)
: mX(0)
, mY(0)
, mZ(0)
, mColour(0)
{
}

template<>
AWForceInline void awVertex2DSimple<awS32, awU32>::SetPosition(const awS32& inX, const awS32& inY, const awS32& inZ)
{
	mX = ( inX << 1);
	mY = (-inY << 1);
	mZ =   inZ;
}

template<>
AWForceInline void awVertex2DSimple<awS32, awU32>::Set(const awS32& inX, const awS32& inY, const awS32& inZ, const awU32& inColour)
{
	SetPosition(inX, inY, inZ);
	mColour = inColour;
}

template<>
AWForceInline awVertex2DFull<awS32, awU32, awU32, awS32>::awVertex2DFull(const awS32& inX, const awS32& inY, const awS32& inZ, const awU32& inColour, const awU32& inMask, const awS32& inU, const awS32& inV)
: awVertex2DSimple<awS32, awU32>(inX, inY, inZ, inColour)
, mMask(inMask)
, mU(inU)
, mV(inV)
{
}

template<>
AWForceInline awVertex2DFull<awS32, awU32, awU32, awS32>::awVertex2DFull( void )
: awVertex2DSimple<awS32, awU32>()
, mMask(0xFFFFFFFF)
, mU(0)
, mV(0)
{
}

template<>
AWForceInline void awVertex2DFull<awS32, awU32, awU32, awS32>::Set(const awS32& inX, const awS32& inY, const awS32& inZ, const awU32& inColour, const awU32& inMask, const awS32& inU, const awS32& inV)
{
	awVertex2DSimple<awS32, awU32>::Set(inX, inY, inZ, inColour);
	mMask	= inMask;
	mU		= inU;
	mV		= inV;
}

}}  //namespace zen, Type 

#endif