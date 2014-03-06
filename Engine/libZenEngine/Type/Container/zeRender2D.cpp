#include "libZenEngine.h"
/*
namespace zen { namespace zenType {
	zenU32 zenRender2D::SFillQuad(zenVec2DSimple* pxVB, zenS32 x1, zenS32 y1, zenS32 x2, zenS32 y2, zenU32 z, zenU32 uColour, const awRegionS32& xClipArea)
	{
		const zenS32 px1(xClipArea.mX1), py1(xClipArea.mY1), px2(xClipArea.mX2), py2(xClipArea.mY2);

		if(!(x2 < px1 || y2 < py1 || x1 > px2 || y1 > py2))
		{
			if(x1 < px1)		{x1 = px1;}
			else if(x2 > px2)	{x2 = px2;}
			if(y1 < py1)		{y1 = py1;}
			else if(y2 > py2)	{y2 = py2;}
			//TopLeft, TopRight, BottomRight,BottomLeft
			pxVB[0].Set(x1, y1, z, uColour);
			pxVB[1].Set(x2, y1, z, uColour);
			pxVB[2].Set(x2, y2, z, uColour);
			pxVB[3].Set(x1, y2, z, uColour);
			return 4;
		}
		return 0;
	}

	zenU32 zenRender2D::SFillQuad(zenVec2DFull*   pxVB, zenS32 x1, zenS32 y1, zenS32 x2, zenS32 y2, zenS32 u1, zenS32 v1, zenS32 u2, zenS32 v2, zenU32 uColour, zenU32 uMask, zenU32 z, const awRegionS32& xClipArea)
	{
		const zenS32 px1(xClipArea.mX1), py1(xClipArea.mY1), px2(xClipArea.mX2), py2(xClipArea.mY2);

		if(!(x2 < px1 || y2 < py1 || x1 > px2 || y1 > py2))
		{
			zenS32 uw = u2 - u1, vh = v2 - v1;
			if(x1 < px1)		{x1 = px1; zenS32 w = x2 - x1; if(w < uw) u1 += uw - w;}
			else if(x2 > px2)	{x2 = px2; zenS32 w = x2 - x1; if(w < uw) u2 -= uw - w;}
			if(y1 < py1)		{y1 = py1; zenS32 h = y2 - y1; if(h < vh) v1 += vh - h;}
			else if(y2 > py2)	{y2 = py2; zenS32 h = y2 - y1; if(h < vh) v2 -= vh - h;}
			//TopLeft, TopRight, BottomRight,BottomLeft
			pxVB[0].Set(x1, y1, z, uColour, uMask, u1, v1);
			pxVB[1].Set(x2, y1, z, uColour, uMask, u2, v1);
			pxVB[2].Set(x2, y2, z, uColour, uMask, u2, v2);
			pxVB[3].Set(x1, y2, z, uColour, uMask, u1, v2);
			return 4;
		}
		return 0;
	}

	void zenRender2D::Shutdown(void)
	{

	}

}}  //namespace zen, Type 
*/