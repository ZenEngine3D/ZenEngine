#include "libZenEngine.h"

namespace zen { namespace awtype {
	awU32 AWRender2D::SFillQuad(awVec2DSimple* pxVB, awS32 x1, awS32 y1, awS32 x2, awS32 y2, awU32 z, awU32 uColour, const awRegionS32& xClipArea)
	{
		const awS32 px1(xClipArea.mX1), py1(xClipArea.mY1), px2(xClipArea.mX2), py2(xClipArea.mY2);

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

	awU32 AWRender2D::SFillQuad(awVec2DFull*   pxVB, awS32 x1, awS32 y1, awS32 x2, awS32 y2, awS32 u1, awS32 v1, awS32 u2, awS32 v2, awU32 uColour, awU32 uMask, awU32 z, const awRegionS32& xClipArea)
	{
		const awS32 px1(xClipArea.mX1), py1(xClipArea.mY1), px2(xClipArea.mX2), py2(xClipArea.mY2);

		if(!(x2 < px1 || y2 < py1 || x1 > px2 || y1 > py2))
		{
			awS32 uw = u2 - u1, vh = v2 - v1;
			if(x1 < px1)		{x1 = px1; awS32 w = x2 - x1; if(w < uw) u1 += uw - w;}
			else if(x2 > px2)	{x2 = px2; awS32 w = x2 - x1; if(w < uw) u2 -= uw - w;}
			if(y1 < py1)		{y1 = py1; awS32 h = y2 - y1; if(h < vh) v1 += vh - h;}
			else if(y2 > py2)	{y2 = py2; awS32 h = y2 - y1; if(h < vh) v2 -= vh - h;}
			//TopLeft, TopRight, BottomRight,BottomLeft
			pxVB[0].Set(x1, y1, z, uColour, uMask, u1, v1);
			pxVB[1].Set(x2, y1, z, uColour, uMask, u2, v1);
			pxVB[2].Set(x2, y2, z, uColour, uMask, u2, v2);
			pxVB[3].Set(x1, y2, z, uColour, uMask, u1, v2);
			return 4;
		}
		return 0;
	}

	void AWRender2D::Shutdown(void)
	{

	}

}}  //namespace zen, Type 
