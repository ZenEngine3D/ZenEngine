#include "zcCore.h"

namespace zcRes
{

bool GfxShaderVertex::Initialize()
{
	mrGfxInputSignature = mrResData->mShaderInputSignatureID;
	return Super::Initialize();
}

}

