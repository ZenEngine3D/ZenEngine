#pragma once
#ifndef __LibFramework_Api_Type_Render2D_h__
#define __LibFramework_Api_Type_Render2D_h__

// @todo :Keep this? refactor?
namespace zen { namespace zenType {
	class zenRender2D
	{
		ZENClassDeclareNoParent(zenRender2D)
	public:
		AWForceInline	zenRender2D(void);
		AWForceInline	~zenRender2D(void);

		static void		SStatup(void);
		static void		SShutdown(void);
		static zenU32	SFillQuad(zenVec2DSimple* pxVB, zenS32 x1, zenS32 y1, zenS32 x2, zenS32 y2, zenU32 z, zenU32 uColour, const awRegionS32& xClipArea);
		static zenU32	SFillQuad(zenVec2DFull*   pxVB, zenS32 x1, zenS32 y1, zenS32 x2, zenS32 y2, zenS32 u1, zenS32 v1, zenS32 u2, zenS32 v2, zenU32 uColour, zenU32 uMask, zenU32 z, const awRegionS32& xClipArea);

		void			Startup(void);
		void			Shutdown(void);

		void			DrawQuads(const zenVec2DSimple* pxQuads, zenU32 uNumQuads, bool bSetStates = true);
		void			DrawQuads(const zenVec2DFull*   pxQuads, zenU32 uNumQuads, bool bSetStates = true);

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
		zenRes::GfxVertex			rCubeVertex;
		zenRes::GfxIndex			rCubeIndex;
		zenRes::GfxShaderVertex	rShaderVS;
		zenRes::GfxShaderPixel		rShaderPS;
		zenRes::GfxTexture2D		mxTexture;
		zenRes::GfxSampler			mxSampler;
		zenRes::GfxShaderBinding	mxShaderBindings[AW_2DMaxShaders];
		zenRes::GfxMeshStrip		mxMeshStrips[AW_2DMaxShaders];
		zenRes::GfxDepthStencil	mxDepthStencil[AW_2DMaxShaders];
		zenRes::GfxBlend			mxBlend[AW_2DMaxShaders];
		zenRes::GfxRasterizer		mxRasterizer;
	private:
		//copy constructor, = operator
		zenRender2D( const zenRender2D& );
		zenRender2D& operator = ( const zenRender2D& );
	};

}}  //namespace zen, Type 

#include "zenTypeRender2D.inl"

#endif
