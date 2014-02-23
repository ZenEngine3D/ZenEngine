#include <AWEngine/Include/AWECore/AWCore.h>
#include "Multiplatform.h"
#include <stdio.h>

Multiplatform::Multiplatform(int aiVal)
: IMultiplatform(aiVal)
{
}

void Multiplatform::print()
{
    printf( "Multiplatform - PC: Allo %i\n", miVal);
}