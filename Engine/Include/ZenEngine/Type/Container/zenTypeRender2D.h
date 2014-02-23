#pragma once
#ifndef __LibFramework_Api_Type_Render2D_h__
#define __LibFramework_Api_Type_Render2D_h__

namespace zen { namespace awtype {
	class AWRender2D
	{
		AWClassDeclareNoParent(AWRender2D)
	public:
		AWForceInline	AWRender2D(void);
		AWForceInline	~AWRender2D(void);

		static void		SStatup(void);
		static void		SShutdown(void);
		static awU32	SFillQuad(awVec2DSimple* pxVB, awS32 x1, awS32 y1, awS32 x2, awS32 y2, awU32 z, awU32 uColour, const awRegionS32& xClipArea);
		static awU32	SFillQuad(awVec2DFull*   pxVB, awS32 x1, awS32 y1, awS32 x2, awS32 y2, awS32 u1, awS32 v1, awS32 u2, awS32 v2, awU32 uColour, awU32 uMask, awU32 z, const awRegionS32& xClipArea);

		void			Startup(void);
		void			Shutdown(void);

		void			DrawQuads(const awVec2DSimple* pxQuads, awU32 uNumQuads, bool bSetStates = true);
		void			DrawQuads(const awVec2DFull*   pxQuads, awU32 uNumQuads, bool bSetStates = true);

//		void			SetTexture( Ares_ShaderResourceViewPtr& xTexture2D );
//		void			SetSamplerState( Ares_SamplerStatePtr& xSamplerState );
//		void			SetRasterizationState( Ares_RasterizerStatePtr& xRasterizationState );
//		void			SetDepthStencilState( Ares_DepthStencilStatePtr& xDepthStencilState );
//		void			SetBlendState( Ares_BlendStatePtr& xBlendState );

	protected:
		typedef enum AWShader2D_Combinations
		{
			AW_2DSimple		= 0,
			AW_2DFull		= 1,
			AW_2DMaxShaders	= 2,
		}AWShader2D_Combinations;

		bool						mbInit;
		awres::awGfxVertex			rCubeVertex;
		awres::awGfxIndex			rCubeIndex;
		awres::awGfxShaderVertex	rShaderVS;
		awres::awGfxShaderPixel		rShaderPS;
		awres::awGfxTexture2D		mxTexture;
		awres::awGfxSampler			mxSampler;
		awres::awGfxShaderBinding	mxShaderBindings[AW_2DMaxShaders];
		awres::awGfxMeshStrip		mxMeshStrips[AW_2DMaxShaders];
		awres::awGfxDepthStencil	mxDepthStencil[AW_2DMaxShaders];
		awres::awGfxBlend			mxBlend[AW_2DMaxShaders];
		awres::awGfxRasterizer		mxRasterizer;
	private:
		//copy constructor, = operator
		AWRender2D( const AWRender2D& );
		AWRender2D& operator = ( const AWRender2D& );
	};

}}  //namespace zen, Type 

#include "zenTypeRender2D.inl"

#endif
