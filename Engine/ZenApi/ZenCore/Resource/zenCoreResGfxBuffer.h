#pragma once
#ifndef __zenApi_Core_ResGfxBuffer_h__
#define __zenApi_Core_ResGfxBuffer_h__

namespace zen { namespace zenRes 
{	
	ZENClassResourceRefDeclare(zGfxVertex, zenConst::keResType_GfxVertex)
	public:
		struct Element
		{
		public:
			Element(){};
			Element(zenConst::eShaderElementType _eType, zU8 _ElementCount, zenConst::eShaderSemantic _eSemantic, zU8 _Offset )
			: meType(_eType)
			, meSemantic(_eSemantic)
			, muVectorSize(_ElementCount)
			, muOffset(_Offset)
			{}

			zenConst::eShaderElementType	meType;
			zenConst::eShaderSemantic		meSemantic;
			zU8								muVectorSize;
			zU8								muOffset;		
		};

		struct Stream
		{
		public:
			zArrayStatic<zU8>				maData;
			zArrayStatic<Element>			maElements;
			zU8								muStride;
			Stream& operator=(const Stream& _Copy)
			{
				maData		= _Copy.maData;
				maElements	= _Copy.maElements;
				muStride	= _Copy.muStride;
				return *this;
			}
		};
	public:		
		zU8*					Lock();
		void					Unlock();
		
		static zGfxVertex		Create(const zArrayBase<zGfxVertex::Stream>& _aStreams, zFlagResUse _ResourceUse);
	};

	ZENClassResourceRefDeclare(zGfxIndex, zenConst::keResType_GfxIndex)
	public:		
		zU8*					Lock();
		void					Unlock();

		static zGfxIndex		Create(const zArrayBase<zU16>& _Indices, zenConst::ePrimitiveType _ePrimitiveType);
		static zGfxIndex		Create(const zArrayBase<zU32>& _Indices, zenConst::ePrimitiveType _ePrimitiveType);
	};

	ZENClassResourceRefDeclare(zGfxTexture2d, zenConst::keResType_GfxTexture2D)
	public:		
		const zVec2U16&			GetDim();
		static zGfxTexture2d	Create(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, zFlagResTexUse _CreationFlags=zFlagResTexUse());		
		static zGfxTexture2d	Create(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, const zArrayBase<zU8>& _aRawData, zFlagResTexUse _CreationFlags=zFlagResTexUse());
	};

	ZENClassResourceRefDeclare(zGfxRenderTarget, zenConst::keResType_GfxRenderTarget)
	public:		
		bool					IsDepth();
		const zVec2U16&			GetDim();
		zGfxTexture2d			GetTexture2D();
		void					Clear(const zVec4F& _vRGBA);
		void					Clear(float _fDepth=1, zU8 _uStencil=0, bool _bClearDepth=true, bool _bClearStencil=false);
		
		static zGfxRenderTarget	Create(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, bool _bSrgb=true);
	};

}} // namespace zen, zenRes

#endif

