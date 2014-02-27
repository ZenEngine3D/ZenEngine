#pragma once
#ifndef __zenCore_ResourceGfxBuffer_h__
#define __zenCore_ResourceGfxBuffer_h__

namespace zen { namespace zenRes {	
	
	AWClassResourceRefDeclare(GfxVertex, awconst::keResType_GfxVertex)
	public:
		struct Element
		{
		public:
			Element(){};
			Element(awconst::eShaderElementType _eType, zenU8 _ElementCount, awconst::eShaderSemantic _eSemantic, zenU8 _Offset )
			: meType(_eType)
			, meSemantic(_eSemantic)
			, muVectorSize(_ElementCount)
			, muOffset(_Offset)
			{}

			awconst::eShaderElementType	meType;
			awconst::eShaderSemantic	meSemantic;
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

	AWClassResourceRefDeclare(GfxIndex, awconst::keResType_GfxIndex)
	public:		
		zenU8*					Lock();
		void					Unlock();

		static GfxIndex			Create(const zenArrayBase<zenU16>& _Indices, awconst::ePrimitiveType _ePrimitiveType);
		static GfxIndex			Create(const zenArrayBase<zenU32>& _Indices, awconst::ePrimitiveType _ePrimitiveType);
	};

	AWClassResourceRefDeclare(GfxTexture2D, awconst::keResType_GfxTexture2D)
	public:		
		const zenVec2U16&		GetDim();
		static GfxTexture2D		Create(awconst::eTextureFormat _eFormat, zenVec2U16 _vDim, awFlagResTexCreate _CreationFlags=awFlagResTexCreate());		
		static GfxTexture2D		Create(awconst::eTextureFormat _eFormat, zenVec2U16 _vDim, const zenArrayBase<zenU8>& _aRawData, awFlagResTexCreate _CreationFlags=awFlagResTexCreate());
	};

	AWClassResourceRefDeclare(GfxRenderTarget, awconst::keResType_GfxRenderTarget)
	public:		
		bool					IsDepth();
		const zenVec2U16&		GetDim();
		GfxTexture2D			GetTexture2D();
		void					Clear(const zenVec4F& _vRGBA);
		void					Clear(float _fDepth=1, zenU8 _uStencil=0, bool _bClearDepth=true, bool _bClearStencil=false);
		

		static GfxRenderTarget	Create(awconst::eTextureFormat _eFormat, zenVec2U16 _vDim, bool _bSrgb=true);
	};

}} // namespace zen, zenRes

#endif

