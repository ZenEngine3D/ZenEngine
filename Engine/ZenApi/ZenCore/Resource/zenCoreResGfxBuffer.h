#pragma once
#ifndef __zenApi_Core_ResGfxBuffer_h__
#define __zenApi_Core_ResGfxBuffer_h__

namespace zen { namespace zenRes 
{	
	class zGfxIndex : public zcRes::GfxIndexRef
	{
	zenClassDeclare(zGfxIndex, zcRes::GfxIndexRef);
	public:
		using Super::Super;
		using Super::operator=;	

		zU8*					Lock();
		void					Unlock(const zenGfx::zContext& rContext);

		static zGfxIndex		Create(const zArrayBase<zU16>& _Indices, zenConst::ePrimitiveType _ePrimitiveType);
		static zGfxIndex		Create(const zArrayBase<zU32>& _Indices, zenConst::ePrimitiveType _ePrimitiveType);
	};

	class zGfxTexture2d : public zcRes::GfxTexture2dRef
	{
	zenClassDeclare(zGfxTexture2d, zcRes::GfxTexture2dRef);
	public:
		using Super::Super;
		using Super::operator=;
		
		const zVec2U16&			GetDim();
		static zGfxTexture2d	Create(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, zFlagResTexUse _CreationFlags=zFlagResTexUse());		
		static zGfxTexture2d	Create(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, const zArrayBase<zU8>& _aRawData, zFlagResTexUse _CreationFlags=zFlagResTexUse());
	};

	class zGfxTarget2D : public zcRes::GfxTarget2DRef
	{
	zenClassDeclare(zGfxTarget2D, zcRes::GfxTarget2DRef);
	public:		
		using Super::Super;
		using Super::operator=;

		bool						IsDepth()const;
		const zVec2U16&				GetDim()const;
		zGfxTexture2d				GetTexture2D();
		
		static zGfxTarget2D			Create(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, bool _bSrgb=true);
	};

	class zGfxBuffer : public zcRes::GfxBufferRef
	{
	zenClassDeclare(zGfxBuffer, zcRes::GfxBufferRef);
	public:		
		using Super::Super;
		using Super::operator=;
		void*					Lock();
		void					Unlock(const zenGfx::zContext& rContext);
	protected:
		static zGfxBuffer		Create(size_t _uElemSize, zU32 _uElemCount/*, zFlagResTexUse _UseFlags*/);
		static zGfxBuffer		Create(const zU8* _pData, size_t _uDataSize, size_t _uElemSize, zU32 _uElemCount/*, zFlagResTexUse _UseFlags*/);
	};

	template<class TStructData>
	class zGfxStructBuffer : public zGfxBuffer
	{
	zenClassDeclare(zGfxStructBuffer, zGfxBuffer);
	public:		
		using Super::Super;
		using Super::operator=;
		
		TStructData* Lock()
		{
			return reinterpret_cast<TStructData*>(Super::Lock());
		}

		static zGfxStructBuffer Create(zU32 _uElemCount/*, zFlagResTexUse _UseFlags*/)
		{
			return zGfxBuffer::Create( sizeof(TStructData), _uElemCount);
		}
		static zGfxStructBuffer Create(const zArrayBase<TStructData>& _aData, zU32 _uElemCount/*, zFlagResTexUse _UseFlags*/)
		{
			return zGfxBuffer::Create( reinterpret_cast<const zU8*>(_aData.First()), _aData.SizeMem(), sizeof(TStructData), _uElemCount);
		}
	};
}} // namespace zen, zenRes

#endif

