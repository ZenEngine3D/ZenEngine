#include "zxExternal.h"

#define NK_IMPLEMENTATION
/*#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT*/
#include <Engine/ThirdParty/nuklear/nuklear.h>

namespace zxNuklear
{

const zArrayStatic<zenRes::zGfxVertex::Element> aUIVerticeInfos = {
		zenRes::zGfxVertex::Element(zenConst::keShaderElemType_Float, 2, zenConst::keShaderSemantic_Position,	offsetof(struct nk_draw_vertex,position)),
		zenRes::zGfxVertex::Element(zenConst::keShaderElemType_Float, 2, zenConst::keShaderSemantic_UV,			offsetof(struct nk_draw_vertex,uv)),
		zenRes::zGfxVertex::Element(zenConst::keShaderElemType_UByte, 4, zenConst::keShaderSemantic_Color,		offsetof(struct nk_draw_vertex,col))
	};

zxRenderData::zxRenderData()
{
	nk_init_default(&moContext, 0);
    //moContext.clip.copy		= nk_d3d11_clipbard_copy;
    //moContext.clip.paste	= nk_d3d11_clipbard_paste;
    moContext.clip.userdata = nk_handle_ptr(0);
    nk_buffer_init_default(&moCommands);
	nk_font_atlas_init_default(&moFontAtlas);
    nk_font_atlas_begin(&moFontAtlas);    	
		/*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
		/*struct nk_font *robot = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Robot-Regular.ttf", 14, 0);*/
		/*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
		/*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
		/*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
		/*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
	{
		const zU8* pGeneratedTexture; int width, height;
		pGeneratedTexture = reinterpret_cast<const zU8*>(nk_font_atlas_bake (&moFontAtlas, &width, &height, NK_FONT_ATLAS_RGBA32));
		zArrayStatic<zU8> aFontRGBA;
		aFontRGBA.Copy(pGeneratedTexture, width*height * 4);
		mrFontTextureAtlas = zenRes::zGfxTexture2d::Create(zenConst::keTexFormat_RGBA8, zVec2U16(width, height), aFontRGBA);
	}    
	zcRes::GfxTexture2dRef rTexture = mrFontTextureAtlas; 
	nk_font_atlas_end(&moFontAtlas, nk_handle_ptr(rTexture.Get()), &moDrawNullTexture);
	if (moFontAtlas.default_font)
        nk_style_set_font(&moContext, &moFontAtlas.default_font->handle);

	// fill converting configuration
    memset(&moConfig, 0, sizeof(moConfig));
    moConfig.global_alpha			= 1.0f;
    moConfig.shape_AA				= NK_ANTI_ALIASING_OFF;
    moConfig.line_AA				= NK_ANTI_ALIASING_OFF;
    moConfig.circle_segment_count	= 22;
    moConfig.curve_segment_count	= 22;
    moConfig.arc_segment_count		= 22;
    moConfig.null					= moDrawNullTexture;
}

zxRenderData::~zxRenderData()
{
	nk_font_atlas_clear(&moFontAtlas);
    nk_buffer_free(&moCommands);
    nk_free(&moContext);
}

zxNuklearHelper::zxNuklearHelper()
{
	// Shaders & State resources
	zenRes::zGfxStateRaster::Config	RasterConfig;
	RasterConfig.mbScissorEnable	= true;
	RasterConfig.mbShowBackface		= true;
	RasterConfig.mbShowFrontface	= true;
	mrShaderVertex					= zenRes::zGfxShaderVertex::Create("Shader/Nuklear.sl", "vs");
	mrShaderPixel					= zenRes::zGfxShaderPixel::Create("Shader/Nuklear.sl", "ps");
	mrShaderBinding					= zenRes::zGfxShaderBinding::Create(mrShaderVertex, mrShaderPixel);
	mrFontSampler					= zenRes::zGfxSampler::Create(zenConst::keTexFilter_Point, zenConst::keTexFilter_Point, zenConst::keTexWrap_Clamp, zenConst::keTexWrap_Clamp, 0);
	mrStateRaster					= zenRes::zGfxStateRaster::Create(RasterConfig);
	mrShaderBinding.CreateShaderParam(marShaderParams);
}

//=================================================================================================
//-------------------------------------------------------------------------------------------------
//=================================================================================================
void zxNuklearHelper::Render(const zEngineRef<zxRenderData>& _rNuklearData, WindowInputState* _pInputData)
{	
	zenPerf::zScopedEventCpu EmitEvent("UINuklear");
	return;
	//-------------------------------------------------------------------------
	// Input
	//-------------------------------------------------------------------------
    nk_input_begin(&_rNuklearData->moContext);
	if (_pInputData)
	{
		struct SpecialKey { zU8 muWindowKey; zU8 muKeyCombin; nk_keys muNuklearkey; };
		const SpecialKey ZenToNuklearKey[] = 
		{
			{'C',			VK_CONTROL,		NK_KEY_COPY},
			{'c',			VK_CONTROL,		NK_KEY_COPY},
			{'v',			VK_CONTROL,		NK_KEY_PASTE},
			{'V',			VK_CONTROL,		NK_KEY_PASTE},
			{'x',			VK_CONTROL,		NK_KEY_CUT},
			{'X',			VK_CONTROL,		NK_KEY_CUT},
			{'z',			VK_CONTROL,		NK_KEY_TEXT_UNDO},
			{'Z',			VK_CONTROL,		NK_KEY_TEXT_UNDO},
			{'y',			VK_CONTROL,		NK_KEY_TEXT_REDO},
			{'Y',			VK_CONTROL,		NK_KEY_TEXT_REDO},
			{VK_RIGHT,		VK_CONTROL,		NK_KEY_TEXT_WORD_RIGHT},
			{VK_LEFT,		VK_CONTROL,		NK_KEY_TEXT_WORD_LEFT},
			{VK_SHIFT,		0,				NK_KEY_SHIFT},
			{VK_CONTROL,	0,				NK_KEY_CTRL},
			{VK_RETURN,		0,				NK_KEY_ENTER},
			{VK_TAB,		0,				NK_KEY_TAB},
			{VK_BACK,		0,				NK_KEY_BACKSPACE},
			{VK_DELETE,		0,				NK_KEY_DEL},
			{VK_INSERT,		0,				NK_KEY_TEXT_INSERT_MODE},
			{VK_HOME,		0,				NK_KEY_TEXT_START},
			{VK_END,		0,				NK_KEY_TEXT_END},
			{VK_UP,			0,				NK_KEY_UP},
			{VK_LEFT,		0,				NK_KEY_LEFT},
			{VK_RIGHT,		0,				NK_KEY_RIGHT},
			{VK_DOWN,		0,				NK_KEY_DOWN},
		};
		
		const nk_buttons zenToNuklearMouse[WindowInputState::keMouseBtn__Count] = 
		{	
			NK_BUTTON_LEFT,		// keMouseBtn_Left
			NK_BUTTON_RIGHT,	// keMouseBtn_Right
			NK_BUTTON_MIDDLE	// keMouseBtn_Middle
		};

		std::bitset<255> KeyProcessed;
		KeyProcessed.reset();

		// Special keys
		for (zUInt idx(0); idx < zenArrayCount(ZenToNuklearKey); ++idx)
		{
			zU8 uWinKey		= ZenToNuklearKey[idx].muWindowKey;
			zU8 uSpecialKey	= ZenToNuklearKey[idx].muKeyCombin;
			if( !KeyProcessed[uWinKey] && (_pInputData->mbIsKeyPressed[uWinKey] || _pInputData->mbIsKeyReleased[uWinKey]) )
			{
				if( uSpecialKey == 0 || _pInputData->mbIsKeyDown[uSpecialKey] )
				{
					KeyProcessed[uWinKey] = true;
					nk_input_key(&_rNuklearData->moContext, ZenToNuklearKey[idx].muNuklearkey, _pInputData->mbIsKeyPressed[uWinKey]);
				}				
			}
		}

		// Standard keys
		for (zUInt idx(0), count(_pInputData->maCharacterPressed.Count()); idx < count; ++idx)
		{
			if( !KeyProcessed[idx] && _pInputData->maCharacterPressed[idx] >= 32 )
				nk_input_unicode(&_rNuklearData->moContext, (nk_rune)_pInputData->maCharacterPressed[idx]);
		}

		// Mouse
		nk_input_motion(&_rNuklearData->moContext, _pInputData->mvMousePos.x, _pInputData->mvMousePos.y);
		for (zUInt idx(0); idx < WindowInputState::keMouseBtn__Count; ++idx)
		{
			if(_pInputData->mbIsMousePressed[idx] || _pInputData->mbIsMouseReleased[idx])
				nk_input_button(&_rNuklearData->moContext, zenToNuklearMouse[idx], _pInputData->mvMousePos.x, _pInputData->mvMousePos.y, _pInputData->mbIsMousePressed[idx]);
		}
	}
    nk_input_end(&_rNuklearData->moContext);

    //-------------------------------------------------------------------------
	// Draw some test UI
	//-------------------------------------------------------------------------
	static nk_color background = nk_rgb(28,48,62);
    {
		const zVec2U16& vDim = _rNuklearData->mrRendertarget.GetDim();
		
		nk_panel layoutMainMenuWnd;
		nk_style SavedStyle = _rNuklearData->moContext.style;
		_rNuklearData->moContext.style.window.border				= 0;
		_rNuklearData->moContext.style.window.menu_border			= 0;
		_rNuklearData->moContext.style.window.padding				= nk_vec2(0,0);
		_rNuklearData->moContext.style.window.min_size				= nk_vec2(0,0);
		_rNuklearData->moContext.style.window.spacing				= nk_vec2(0,0);
		_rNuklearData->moContext.style.window.footer_padding		= nk_vec2(0,0);
		_rNuklearData->moContext.style.window.header.padding		= nk_vec2(0,0);
		_rNuklearData->moContext.style.window.header.label_padding	= nk_vec2(0,0);
		_rNuklearData->moContext.style.window.header.spacing		= nk_vec2(0,8);
		_rNuklearData->moContext.style.window.menu_border			= 0;
		if( nk_begin(&_rNuklearData->moContext, &layoutMainMenuWnd, "MenuBar", nk_rect(0, 200, vDim.x, 24), NK_WINDOW_NO_SCROLLBAR)	)
		{
			nk_panel menu;	
			//float saved = _rNuklearData->moContext.current->layout->bounds.h;
			//_rNuklearData->moContext.current->layout->bounds.h = 400;
			nk_menubar_begin(&_rNuklearData->moContext);

			float fRatio1(0.1f);
			nk_layout_row_begin(&_rNuklearData->moContext, NK_STATIC, 20, 1);
			//nk_layout_row(&_rNuklearData->moContext, NK_STATIC, 20, 1, &fRatio1);
			//nk_layout_row_push(&_rNuklearData->moContext, 45);
			if (nk_menu_begin_label(&_rNuklearData->moContext, &menu, "Menu", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_TOP, 120))
			{
				static size_t prog = 40;
				static int slider = 10;
				static int check = nk_true;
				nk_layout_row_dynamic(&_rNuklearData->moContext, 25, 1);
				if (nk_menu_item_label(&_rNuklearData->moContext, "Hide", NK_TEXT_LEFT))
					;//show_menu = nk_false;
				if (nk_menu_item_label(&_rNuklearData->moContext, "About", NK_TEXT_LEFT))
					;//show_app_about = nk_true;
				nk_progress(&_rNuklearData->moContext, &prog, 100, NK_MODIFIABLE);
				nk_slider_int(&_rNuklearData->moContext, 0, &slider, 16, 1);
				nk_checkbox_label(&_rNuklearData->moContext, "check", &check);
				nk_menu_end(&_rNuklearData->moContext);
			}
			//nk_layout_row_end(&_rNuklearData->moContext);

			float fRatio2(0.1);
		//	nk_layout_row_begin(&_rNuklearData->moContext, NK_STATIC, 20, 2);

			//nk_layout_row(&_rNuklearData->moContext, NK_STATIC, 20, 2, &fRatio2);
			//nk_layout_row_push(&_rNuklearData->moContext, 45);
			if (nk_menu_begin_label(&_rNuklearData->moContext, &menu, "Menu2", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_TOP, 120))
			{
				static size_t prog = 40;
				static int slider = 10;
				static int check = nk_true;
				nk_layout_row_dynamic(&_rNuklearData->moContext, 25, 1);
				if (nk_menu_item_label(&_rNuklearData->moContext, "Hide", NK_TEXT_LEFT))
					;//show_menu = nk_false;
				if (nk_menu_item_label(&_rNuklearData->moContext, "About", NK_TEXT_LEFT))
					;//show_app_about = nk_true;
				nk_menu_end(&_rNuklearData->moContext);
			}
			nk_layout_row_end(&_rNuklearData->moContext);
			

			nk_menubar_end(&_rNuklearData->moContext);

			

			//_rNuklearData->moContext.current->layout->bounds.h = saved;
			nk_end(&_rNuklearData->moContext);			
		}

		_rNuklearData->moContext.style = SavedStyle;
		nk_panel layout;
		if( nk_begin(&_rNuklearData->moContext, &layout, "Demo", nk_rect(0.f, 250.f, vDim.x/2.f, 400.f),
							NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)	)
		{

			nk_panel combo;
			const char* weapons[4] = { "Un","Deux","Trois","Quatre" };
			static int check_values[4] = { false, false, false, false };
			//nk_layout_row_static(&_rNuklearData->moContext, 30, 1, 1);
			nk_layout_row_dynamic(&_rNuklearData->moContext, 22, 2);
			if (nk_combo_begin_label(&_rNuklearData->moContext, &combo, "text", 400)) {
				nk_layout_row_dynamic(&_rNuklearData->moContext, 20, 1);
				nk_checkbox_label(&_rNuklearData->moContext, weapons[0], &check_values[0]);
				nk_checkbox_label(&_rNuklearData->moContext, weapons[1], &check_values[1]);
				nk_checkbox_label(&_rNuklearData->moContext, weapons[2], &check_values[2]);
				nk_checkbox_label(&_rNuklearData->moContext, weapons[3], &check_values[3]);
				nk_combo_end(&_rNuklearData->moContext);
			}

			enum {EASY, HARD};
			static int op = EASY;
			static int property = 20;

			nk_layout_row_static(&_rNuklearData->moContext, 30, 80, 1);
			if (nk_button_label(&_rNuklearData->moContext, "button", NK_BUTTON_DEFAULT))
				fprintf(stdout, "button pressed\n");
			nk_layout_row_dynamic(&_rNuklearData->moContext, 30, 2);
			if (nk_option_label(&_rNuklearData->moContext, "easy", op == EASY)) op = EASY;
			if (nk_option_label(&_rNuklearData->moContext, "hard", op == HARD)) op = HARD;
			nk_layout_row_dynamic(&_rNuklearData->moContext, 22, 1);
			nk_property_int(&_rNuklearData->moContext, "Compression:", 0, &property, 100, 10, 1);
			{
				struct nk_panel combo;
				nk_layout_row_dynamic(&_rNuklearData->moContext, 20, 1);
				nk_label(&_rNuklearData->moContext, "background:", NK_TEXT_LEFT);
				nk_layout_row_dynamic(&_rNuklearData->moContext, 25, 1);
				if (nk_combo_begin_color(&_rNuklearData->moContext, &combo, background, 400)) 
				{
					nk_layout_row_dynamic(&_rNuklearData->moContext, 120, 1);
					background = nk_color_picker(&_rNuklearData->moContext, background, NK_RGBA);
					nk_layout_row_dynamic(&_rNuklearData->moContext, 25, 1);
					background.r = (nk_byte)nk_propertyi(&_rNuklearData->moContext, "#R:", 0, background.r, 255, 1,1);
					background.g = (nk_byte)nk_propertyi(&_rNuklearData->moContext, "#G:", 0, background.g, 255, 1,1);
					background.b = (nk_byte)nk_propertyi(&_rNuklearData->moContext, "#B:", 0, background.b, 255, 1,1);
					background.a = (nk_byte)nk_propertyi(&_rNuklearData->moContext, "#A:", 0, background.a, 255, 1,1);
					nk_combo_end(&_rNuklearData->moContext);
				}
			}
			static char text[3][64];
			static int text_len[3];
			nk_edit_string(&_rNuklearData->moContext, NK_EDIT_FIELD, text[0], &text_len[0], 64, nk_filter_default);

			
			/*
            static int col_index = -1;
            static int line_index = -1;
			float step = (2*3.141592654f) / 128;
            int index = -1;
			float id = 0;
			struct nk_vec2 windowSize = nk_window_get_content_region_size(&_rNuklearData->moContext);
			nk_layout_row_dynamic(&_rNuklearData->moContext, windowSize.y, 1);
            struct nk_rect bounds = nk_widget_bounds(&_rNuklearData->moContext);
            if (nk_chart_begin(&_rNuklearData->moContext, NK_CHART_COLUMN, 128, 0.0f, 1.0f)) 
			{
                for (zUInt i = 0; i < 128; ++i) 
				{
                    //nk_flags res = nk_chart_push(&_rNuklearData->moContext, (float)fabs(sin(id)));
					nk_flags res = nk_chart_push(&_rNuklearData->moContext, float(i)/128.f);
                    if (res & NK_CHART_HOVERING)
                        index = (int)i;
                    if (res & NK_CHART_CLICKED)
                        col_index = (int)i;
                    id += step;
                }
                nk_chart_end(&_rNuklearData->moContext);
            }
            if (index != -1) 
			{
                char buffer[NK_MAX_NUMBER_BUFFER];
                sprintf(buffer, "Value: %.2f", (float)fabs(sin(step * (float)index)));
                nk_tooltip(&_rNuklearData->moContext, buffer);
            }
            if (col_index != -1) 
			{
                nk_layout_row_dynamic(&_rNuklearData->moContext, 20, 1);
                nk_labelf(&_rNuklearData->moContext, NK_TEXT_LEFT, "Selected value: %.2f", (float)fabs(sin(step * (float)col_index)));
            }*/
		}
	    nk_end(&_rNuklearData->moContext);
	}
    //if (nk_window_is_closed(_rNuklearData->moContext, "Demo")) break;

	//-------------------------------------------------------------------------
	// Render results
	//-------------------------------------------------------------------------
    {        
        // setup buffers to load vertices and elements
        struct nk_buffer vbuf, ibuf;
		zUInt TotalVtxCount = 16096;
		zUInt TotalIdxCount = 16096;
		if(_rNuklearData->muVertexCount < TotalVtxCount)
		{
			zArrayStatic<zenRes::zGfxVertex::Stream> aUIVerticeStreams(1);
			_rNuklearData->muVertexCount	= static_cast<zUInt>(TotalVtxCount*1.25);
			aUIVerticeStreams[0].muStride	= static_cast<zU32>(sizeof(struct nk_draw_vertex));
			aUIVerticeStreams[0].maElements = aUIVerticeInfos;
			aUIVerticeStreams[0].maData.SetCount(_rNuklearData->muVertexCount*sizeof(struct nk_draw_vertex));
			_rNuklearData->mrVertexBuffer = zenRes::zGfxVertex::Create(aUIVerticeStreams, zFlagResUse(zenConst::keResUse_DynamicDiscard));
		}

		if(_rNuklearData->muIndexCount < TotalIdxCount)
		{
			_rNuklearData->muIndexCount = static_cast<zUInt>(TotalIdxCount*1.25);			
			zArrayStatic<zU16> aIndices;
			aIndices.SetCount(_rNuklearData->muIndexCount);
			_rNuklearData->mrIndexBuffer = zenRes::zGfxIndex::Create(aIndices, zenConst::kePrimType_TriangleList);
		}

		//----------------------------------------------------------------------------
		// Adjust the rendering size to window size
		if( !_rNuklearData->mrRenderpass.IsValid() || _rNuklearData->mvScreenSize != _rNuklearData->mrRendertarget.GetDim())
		{
			zenRes::zGfxRenderPass::ConfigColorRT	UIColorRTConfig;
			zenRes::zGfxRenderPass::ConfigDepthRT	UIDepthRTConfig;
			_rNuklearData->mvScreenSize			= _rNuklearData->mrRendertarget.GetDim();
			UIColorRTConfig.mrTargetSurface		= _rNuklearData->mrRendertarget;
			UIColorRTConfig.mbBlendEnable		= true;
			UIColorRTConfig.meBlendColorSrc		= zenRes::zGfxRenderPass::ConfigColorRT::keBlendVal_SrcAlpha;
			UIColorRTConfig.meBlendColorDest	= zenRes::zGfxRenderPass::ConfigColorRT::keBlendVal_InvSrcAlpha;
			UIColorRTConfig.meBlendColorOp		= zenRes::zGfxRenderPass::ConfigColorRT::keBlendOp_Add;
			UIColorRTConfig.meBlendAlphaSrc		= zenRes::zGfxRenderPass::ConfigColorRT::keBlendVal_Zero;
			UIColorRTConfig.meBlendAlphaDest	= zenRes::zGfxRenderPass::ConfigColorRT::keBlendVal_One;
			UIColorRTConfig.meBlendAlphaOp		= zenRes::zGfxRenderPass::ConfigColorRT::keBlendOp_Add;		
			_rNuklearData->mrRenderpass			= zenRes::zGfxRenderPass::Create("RenderUI", 0, UIColorRTConfig, UIDepthRTConfig, mrStateRaster); //! @todo feature expose name
			zenMath::MatrixProjectionOrthoLH(_rNuklearData->matOrthographic, _rNuklearData->mvScreenSize.x, _rNuklearData->mvScreenSize.y, 0, 1);
			//nk_d3d11_get_projection_matrix(_rNuklearData->mvScreenSize.x, _rNuklearData->mvScreenSize.y, (float *)&_rNuklearData->matOrthographic);
		}

		if( _rNuklearData->muIndexCount && _rNuklearData->muVertexCount )
		{
			zenGfx::zContext rUIContext	= zenGfx::zContext::Create("UINuklear", _rNuklearData->mrRenderpass);

			//----------------------------------------------------------------------------
			// Update content of vertex/index
			struct nk_draw_vertex* pUIVertices	= reinterpret_cast<struct nk_draw_vertex*>(_rNuklearData->mrVertexBuffer.Lock());
			zU16* pUIIndices					= reinterpret_cast<zU16*>(_rNuklearData->mrIndexBuffer.Lock());
			nk_buffer_init_fixed(&vbuf, pUIVertices, (size_t)TotalVtxCount*sizeof(struct nk_draw_vertex));
			nk_buffer_init_fixed(&ibuf, pUIIndices, (size_t)TotalIdxCount*sizeof(zU16));
			nk_convert(&_rNuklearData->moContext, &_rNuklearData->moCommands, &vbuf, &ibuf, &_rNuklearData->moConfig);

			_rNuklearData->mrVertexBuffer.Unlock(rUIContext);
			_rNuklearData->mrIndexBuffer.Unlock(rUIContext);
		
			//----------------------------------------------------------------------------
			// Draw result
			const nk_draw_command* pCommand;
			UINT offset = 0;			
			UINT idxCount = 0;
			//! @todo perf only create strip when vertex buffer changes
			nk_draw_foreach(pCommand, &_rNuklearData->moContext, &_rNuklearData->moCommands)
			{
				idxCount += pCommand->elem_count;
			}			
			zenRes::zGfxMeshStrip rMeshStrip = zenRes::zGfxMeshStrip::Create(	_rNuklearData->mrVertexBuffer, 
																				_rNuklearData->mrIndexBuffer, 
																				mrShaderBinding, 
																				marShaderParams, 0, idxCount, 0);
			rMeshStrip.SetValue(zHash32("txFont"), _rNuklearData->mrFontTextureAtlas, mrFontSampler);
			rMeshStrip.SetValue(zHash32("ProjectionMatrix"), _rNuklearData->matOrthographic);

			nk_draw_foreach(pCommand, &_rNuklearData->moContext, &_rNuklearData->moCommands)
			{
				zVec4U16 vScreenScicor;
				vScreenScicor.x	= zenMath::Max<zI32>(0, pCommand->clip_rect.x);
				vScreenScicor.y	= zenMath::Max<zI32>(0, pCommand->clip_rect.y);
				vScreenScicor.z	= zenMath::Min<zI32>(_rNuklearData->mvScreenSize.x, pCommand->clip_rect.x+pCommand->clip_rect.w);
				vScreenScicor.w	= zenMath::Min<zI32>(_rNuklearData->mvScreenSize.y, pCommand->clip_rect.y+pCommand->clip_rect.h);
				zenGfx::zCommand::DrawMesh(rUIContext, 0.f, rMeshStrip, offset, pCommand->elem_count, vScreenScicor);
				offset			+= pCommand->elem_count;
			}
			rUIContext.Submit();
		}

		/*
        // iterate over and execute each draw command
        nk_draw_foreach(cmd, &d3d11.ctx, &d3d11.cmds)
        {
            ID3D11ShaderResourceView *texture_view = (ID3D11ShaderResourceView *)cmd->texture.ptr;
            D3D11_RECT scissor;
            if (!cmd->elem_count) continue;
            scissor.left = (LONG)cmd->clip_rect.x;
            scissor.right = (LONG)(cmd->clip_rect.x + cmd->clip_rect.w);
            scissor.top = (LONG)cmd->clip_rect.y;
            scissor.bottom = (LONG)(cmd->clip_rect.y + cmd->clip_rect.h);
            ID3D11DeviceContext_PSSetShaderResources(context, 0, 1, &texture_view);
            ID3D11DeviceContext_RSSetScissorRects(context, 1, &scissor);
            ID3D11DeviceContext_DrawIndexed(context, (UINT)cmd->elem_count, offset, 0);
            offset += cmd->elem_count;
        }
		*/
		
        nk_clear(&_rNuklearData->moContext);

    }
}

} // namespace zxImGui