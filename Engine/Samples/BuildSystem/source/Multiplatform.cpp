#include <AWEngine/Include/AWECore/AWCore.h>
#include "Multiplatform.h"
#include <stdio.h>

IMultiplatform::IMultiplatform(int aival)
{
    miVal = aival;
}

void IMultiplatform::print()
{
    printf("Multiplatform - Base: Allo %i\n", miVal);
}