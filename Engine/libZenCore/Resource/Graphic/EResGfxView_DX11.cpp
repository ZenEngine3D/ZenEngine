#include "libZenCore.h"

namespace ERes
{
	GfxView_DX11::GfxView_DX11()
	{
		mInstanceInfo.marTargetColor.SetCount(0);
		mInstanceInfo.mrTargetDepth	= NULL;
	}

	GfxView_DX11::~GfxView_DX11()
	{

	}

	bool GfxView_DX11::ResourceInit()
	{
		mInstanceInfo.marTargetColor.Copy<awResourceID>( mInstanceInfo.mSerial.maTargetColorID );
		mInstanceInfo.mrTargetDepth = mInstanceInfo.mSerial.mTargetDepthID;
		return true;
	}

	void GfxView_DX11::Clear( bool _bClearColor, const awVec4F& _vRGBA, bool _bClearDepth, float _fDepth, bool _bClearStencil, awU8 _uStencil )
	{
		if( _bClearColor )
		{
			for(awUInt rtIdx(0), rtCount(mInstanceInfo.marTargetColor.Count()); rtIdx<rtCount; ++rtIdx)
			{
				if( mInstanceInfo.marTargetColor[rtIdx].IsValid() )
					mInstanceInfo.marTargetColor[rtIdx]->Clear(_vRGBA);
			}
		}
		if( mInstanceInfo.mrTargetDepth.IsValid()&& (_bClearDepth || _bClearStencil) )
		{
			mInstanceInfo.mrTargetDepth->Clear(_fDepth, _uStencil, _bClearDepth, _bClearStencil);
		}
	}
	
}