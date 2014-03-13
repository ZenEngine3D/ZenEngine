#pragma once
#ifndef __zenCore_ResourceGfxBuffer_h__
#define __zenCore_ResourceGfxBuffer_h__

namespace zen { namespace zenRes {	
	
	AWClassResourceRefDeclare(zGfxVertex, zenConst::keResType_GfxVertex)
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
			zenConst::eShaderSemantic	meSemantic;
			zU8						muVectorSize;
			zU8						muOffset;		
		};

		struct Stream
		{
		public:
			zArrayStatic<zU8>		maData;
			zArrayStatic<Element>	maElements;
			zU8					muStride;
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
		
		static zGfxVertex		Create(const zArrayBase<zGfxVertex::Stream>& _aStreams, zU32 _uCreationFlags);
	};

	AWClassResourceRefDeclare(zGfxIndex, zenConst::keResType_GfxIndex)
	public:		
		zU8*					Lock();
		void					Unlock();

		static zGfxIndex		Create(const zArrayBase<zU16>& _Indices, zenConst::ePrimitiveType _ePrimitiveType);
		static zGfxIndex		Create(const zArrayBase<zU32>& _Indices, zenConst::ePrimitiveType _ePrimitiveType);
	};

	AWClassResourceRefDeclare(zGfxTexture2D, zenConst::keResType_GfxTexture2D)
	public:		
		const zVec2U16&			GetDim();
		static zGfxTexture2D	Create(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, zenFlagResTexCreate _CreationFlags=zenFlagResTexCreate());		
		static zGfxTexture2D	Create(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, const zArrayBase<zU8>& _aRawData, zenFlagResTexCreate _CreationFlags=zenFlagResTexCreate());
	};

	AWClassResourceRefDeclare(zGfxRenderTarget, zenConst::keResType_GfxRenderTarget)
	public:		
		bool					IsDepth();
		const zVec2U16&			GetDim();
		zGfxTexture2D			GetTexture2D();
		void					Clear(const zVec4F& _vRGBA);
		void					Clear(float _fDepth=1, zU8 _uStencil=0, bool _bClearDepth=true, bool _bClearStencil=false);
		
		static zGfxRenderTarget	Create(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, bool _bSrgb=true);
	};

}} // namespace zen, zenRes

#endif

