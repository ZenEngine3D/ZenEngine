#pragma once
#ifndef __zenCore_ResourceGfxBuffer_h__
#define __zenCore_ResourceGfxBuffer_h__

namespace zen { namespace zenRes {	
	
	AWClassResourceRefDeclare(GfxVertex, zenConst::keResType_GfxVertex)
	public:
		struct Element
		{
		public:
			Element(){};
			Element(zenConst::eShaderElementType _eType, zenU8 _ElementCount, zenConst::eShaderSemantic _eSemantic, zenU8 _Offset )
			: meType(_eType)
			, meSemantic(_eSemantic)
			, muVectorSize(_ElementCount)
			, muOffset(_Offset)
			{}

			zenConst::eShaderElementType	meType;
			zenConst::eShaderSemantic	meSemantic;
			zenU8						muVectorSize;
			zenU8						muOffset;		
		};

		struct Stream
		{
		public:
			zenArrayStatic<zenU8>		maData;
			zenArrayStatic<Element>	maElements;
			zenU8					muStride;
			Stream& operator=(const Stream& _Copy)
			{
				maData		= _Copy.maData;
				maElements	= _Copy.maElements;
				muStride	= _Copy.muStride;
				return *this;
			}
		};
	public:		
		zenU8*					Lock();
		void					Unlock();
		
		static GfxVertex		Create(const zenArrayBase<GfxVertex::Stream>& _aStreams, zenU32 _uCreationFlags);
	};

	AWClassResourceRefDeclare(GfxIndex, zenConst::keResType_GfxIndex)
	public:		
		zenU8*					Lock();
		void					Unlock();

		static GfxIndex			Create(const zenArrayBase<zenU16>& _Indices, zenConst::ePrimitiveType _ePrimitiveType);
		static GfxIndex			Create(const zenArrayBase<zenU32>& _Indices, zenConst::ePrimitiveType _ePrimitiveType);
	};

	AWClassResourceRefDeclare(GfxTexture2D, zenConst::keResType_GfxTexture2D)
	public:		
		const zenVec2U16&		GetDim();
		static GfxTexture2D		Create(zenConst::eTextureFormat _eFormat, zenVec2U16 _vDim, zenFlagResTexCreate _CreationFlags=zenFlagResTexCreate());		
		static GfxTexture2D		Create(zenConst::eTextureFormat _eFormat, zenVec2U16 _vDim, const zenArrayBase<zenU8>& _aRawData, zenFlagResTexCreate _CreationFlags=zenFlagResTexCreate());
	};

	AWClassResourceRefDeclare(GfxRenderTarget, zenConst::keResType_GfxRenderTarget)
	public:		
		bool					IsDepth();
		const zenVec2U16&		GetDim();
		GfxTexture2D			GetTexture2D();
		void					Clear(const zenVec4F& _vRGBA);
		void					Clear(float _fDepth=1, zenU8 _uStencil=0, bool _bClearDepth=true, bool _bClearStencil=false);
		

		static GfxRenderTarget	Create(zenConst::eTextureFormat _eFormat, zenVec2U16 _vDim, bool _bSrgb=true);
	};

}} // namespace zen, zenRes

#endif

