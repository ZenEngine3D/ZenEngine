#pragma once
#ifndef __LibFramework_Api_Type_Vertex2D_inl__
#define __LibFramework_Api_Type_Vertex2D_inl__

namespace zen { namespace zenType {

template<>
AWForceInline awVertex2DSimple<zenS32, zenU32>::awVertex2DSimple(const zenS32& inX, const zenS32& inY, const zenS32& inZ, const zenU32& inColour)
: mX( inX << 1 )
, mY(-inY << 1 )
, mZ( inZ)
, mColour(inColour)
{
}

template<>
AWForceInline awVertex2DSimple<zenS32, zenU32>::awVertex2DSimple(void)
: mX(0)
, mY(0)
, mZ(0)
, mColour(0)
{
}

template<>
AWForceInline void awVertex2DSimple<zenS32, zenU32>::SetPosition(const zenS32& inX, const zenS32& inY, const zenS32& inZ)
{
	mX = ( inX << 1);
	mY = (-inY << 1);
	mZ =   inZ;
}

template<>
AWForceInline void awVertex2DSimple<zenS32, zenU32>::Set(const zenS32& inX, const zenS32& inY, const zenS32& inZ, const zenU32& inColour)
{
	SetPosition(inX, inY, inZ);
	mColour = inColour;
}

template<>
AWForceInline awVertex2DFull<zenS32, zenU32, zenU32, zenS32>::awVertex2DFull(const zenS32& inX, const zenS32& inY, const zenS32& inZ, const zenU32& inColour, const zenU32& inMask, const zenS32& inU, const zenS32& inV)
: awVertex2DSimple<zenS32, zenU32>(inX, inY, inZ, inColour)
, mMask(inMask)
, mU(inU)
, mV(inV)
{
}

template<>
AWForceInline awVertex2DFull<zenS32, zenU32, zenU32, zenS32>::awVertex2DFull( void )
: awVertex2DSimple<zenS32, zenU32>()
, mMask(0xFFFFFFFF)
, mU(0)
, mV(0)
{
}

template<>
AWForceInline void awVertex2DFull<zenS32, zenU32, zenU32, zenS32>::Set(const zenS32& inX, const zenS32& inY, const zenS32& inZ, const zenU32& inColour, const zenU32& inMask, const zenS32& inU, const zenS32& inV)
{
	awVertex2DSimple<zenS32, zenU32>::Set(inX, inY, inZ, inColour);
	mMask	= inMask;
	mU		= inU;
	mV		= inV;
}

}}  //namespace zen, Type 

#endif