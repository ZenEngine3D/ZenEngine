#pragma once
#ifndef __zenCore_ResourceGfxBuffer_h__
#define __zenCore_ResourceGfxBuffer_h__

namespace zen { namespace awres {	
	
	AWClassResourceRefDeclare(awGfxVertex, awconst::keResType_GfxVertex)
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
		zenU8*						Lock();
		void						Unlock();
		
		static awGfxVertex			Create(const zenArrayBase<awGfxVertex::Stream>& _aStreams, zenU32 _uCreationFlags);
	};

	AWClassResourceRefDeclare(awGfxIndex, awconst::keResType_GfxIndex)
	public:		
		zenU8*						Lock();
		void						Unlock();

		static awGfxIndex			Create(const zenArrayBase<zenU16>& _Indices, awconst::ePrimitiveType _ePrimitiveType);
		static awGfxIndex			Create(const zenArrayBase<zenU32>& _Indices, awconst::ePrimitiveType _ePrimitiveType);
	};

	AWClassResourceRefDeclare(awGfxTexture2D, awconst::keResType_GfxTexture2D)
	public:		
		const zenVec2U16&			GetDim();
		static awGfxTexture2D		Create(awconst::eTextureFormat _eFormat, zenVec2U16 _vDim, awFlagResTexCreate _CreationFlags=awFlagResTexCreate());		
		static awGfxTexture2D		Create(awconst::eTextureFormat _eFormat, zenVec2U16 _vDim, const zenArrayBase<zenU8>& _aRawData, awFlagResTexCreate _CreationFlags=awFlagResTexCreate());
	};

	AWClassResourceRefDeclare(awGfxRenderTarget, awconst::keResType_GfxRenderTarget)
	public:		
		bool						IsDepth();
		const zenVec2U16&			GetDim();
		awGfxTexture2D				GetTexture2D();
		void						Clear(const zenVec4F& _vRGBA);
		void						Clear(float _fDepth=1, zenU8 _uStencil=0, bool _bClearDepth=true, bool _bClearStencil=false);
		

		static awGfxRenderTarget	Create(awconst::eTextureFormat _eFormat, zenVec2U16 _vDim, bool _bSrgb=true);
	};

}} // namespace zen, awres

#endif

