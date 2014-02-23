#pragma once
#ifndef __LibEngine_Api_ResourceGfxBuffer_h__
#define __LibEngine_Api_ResourceGfxBuffer_h__

namespace zen { namespace awres {	
	
	AWClassResourceRefDeclare(awGfxVertex, awconst::keResType_GfxVertex)
	public:
		struct Element
		{
		public:
			Element(){};
			Element(awconst::eShaderElementType _eType, awU8 _ElementCount, awconst::eShaderSemantic _eSemantic, awU8 _Offset )
			: meType(_eType)
			, meSemantic(_eSemantic)
			, muVectorSize(_ElementCount)
			, muOffset(_Offset)
			{}

			awconst::eShaderElementType	meType;
			awconst::eShaderSemantic	meSemantic;
			awU8						muVectorSize;
			awU8						muOffset;		
		};

		struct Stream
		{
		public:
			awArrayStatic<awU8>		maData;
			awArrayStatic<Element>	maElements;
			awU8					muStride;
			Stream& operator=(const Stream& _Copy)
			{
				maData		= _Copy.maData;
				maElements	= _Copy.maElements;
				muStride	= _Copy.muStride;
				return *this;
			}
		};
	public:		
		awU8*						Lock();
		void						Unlock();
		
		static awGfxVertex			Create(const awArrayBase<awGfxVertex::Stream>& _aStreams, awU32 _uCreationFlags);
	};

	AWClassResourceRefDeclare(awGfxIndex, awconst::keResType_GfxIndex)
	public:		
		awU8*						Lock();
		void						Unlock();

		static awGfxIndex			Create(const awArrayBase<awU16>& _Indices, awconst::ePrimitiveType _ePrimitiveType);
		static awGfxIndex			Create(const awArrayBase<awU32>& _Indices, awconst::ePrimitiveType _ePrimitiveType);
	};

	AWClassResourceRefDeclare(awGfxTexture2D, awconst::keResType_GfxTexture2D)
	public:		
		const awVec2U16&			GetDim();
		static awGfxTexture2D		Create(awconst::eTextureFormat _eFormat, awVec2U16 _vDim, awFlagResTexCreate _CreationFlags=awFlagResTexCreate());		
		static awGfxTexture2D		Create(awconst::eTextureFormat _eFormat, awVec2U16 _vDim, const awArrayBase<awU8>& _aRawData, awFlagResTexCreate _CreationFlags=awFlagResTexCreate());
	};

	AWClassResourceRefDeclare(awGfxRenderTarget, awconst::keResType_GfxRenderTarget)
	public:		
		bool						IsDepth();
		const awVec2U16&			GetDim();
		awGfxTexture2D				GetTexture2D();
		void						Clear(const awVec4F& _vRGBA);
		void						Clear(float _fDepth=1, awU8 _uStencil=0, bool _bClearDepth=true, bool _bClearStencil=false);
		

		static awGfxRenderTarget	Create(awconst::eTextureFormat _eFormat, awVec2U16 _vDim, bool _bSrgb=true);
	};

}} // namespace zen, awres

#endif

