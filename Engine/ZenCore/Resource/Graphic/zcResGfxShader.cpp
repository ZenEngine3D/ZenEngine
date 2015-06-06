#include "zcCore.h"

namespace zcRes
{

bool GfxShaderVertex::Initialize()
{
	mrGfxInputSignature = mrExportData->mShaderInputSignatureID;
	return Super::Initialize();
}

}

