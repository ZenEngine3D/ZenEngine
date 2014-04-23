#include "libZenEngine.h"
/*
namespace zen { namespace zenType {
	zU32 zenRender2D::SFillQuad(zVec2DSimple* pxVB, zI32 x1, zI32 y1, zI32 x2, zI32 y2, zU32 z, zU32 uColour, const zRegionS32& xClipArea)
	{
		const zI32 px1(xClipArea.mX1), py1(xClipArea.mY1), px2(xClipArea.mX2), py2(xClipArea.mY2);

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

	zU32 zenRender2D::SFillQuad(zVec2DFull*   pxVB, zI32 x1, zI32 y1, zI32 x2, zI32 y2, zI32 u1, zI32 v1, zI32 u2, zI32 v2, zU32 uColour, zU32 uMask, zU32 z, const zRegionS32& xClipArea)
	{
		const zI32 px1(xClipArea.mX1), py1(xClipArea.mY1), px2(xClipArea.mX2), py2(xClipArea.mY2);

		if(!(x2 < px1 || y2 < py1 || x1 > px2 || y1 > py2))
		{
			zI32 uw = u2 - u1, vh = v2 - v1;
			if(x1 < px1)		{x1 = px1; zI32 w = x2 - x1; if(w < uw) u1 += uw - w;}
			else if(x2 > px2)	{x2 = px2; zI32 w = x2 - x1; if(w < uw) u2 -= uw - w;}
			if(y1 < py1)		{y1 = py1; zI32 h = y2 - y1; if(h < vh) v1 += vh - h;}
			else if(y2 > py2)	{y2 = py2; zI32 h = y2 - y1; if(h < vh) v2 -= vh - h;}
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