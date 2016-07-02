#pragma once
#ifndef __zCore_Res_Gfx_All_h__
#define __zCore_Res_Gfx_All_h__

namespace zcGfx { class ManagerRender; } //Forward declaration //! @todo clean remove this and call function to do the stuff needed in manager instead?

#include ZENHeaderRenderer( zcResGfxVertex )
#include ZENHeaderRenderer( zcResGfxShaderParam )
#include ZENHeaderRenderer( zcResGfxBinding )
#include ZENHeaderRenderer( zcResGfxShader )
#include ZENHeaderRenderer( zcResGfxTexture )
#include ZENHeaderRenderer( zcResGfxRenderTarget )
#include ZENHeaderRenderer( zcResGfxView )
#include ZENHeaderRenderer( zcResGfxWindow )
#include ZENHeaderRenderer( zcResGfxState )
#include ZENHeaderRenderer( zcResGfxRenderpass )

#include "zcResGfxIndex.h"
#include "zcResGfxVertex.h"
#include "zcResGfxShader.h"
#include "zcResGfxShaderParam.h"
#include "zcResGfxState.h"
#include "zcResGfxTexture.h"
#include "zcResGfxRenderTarget.h"
#include "zcResGfxView.h"
#include "zcResGfxWindow.h"
#include "zcResGfxBinding.h"
#include "zcResGfxRenderpass.h"

#endif