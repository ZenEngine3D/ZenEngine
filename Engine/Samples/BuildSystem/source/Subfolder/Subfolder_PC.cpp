#include "AWEngine/Include/AWECore/AWCore.h"
#include "Subfolder.h"
#include <stdio.h>

Subfolder::Subfolder(int aiVal)
: ISubfolder(aiVal)
{
}

void Subfolder::print()
{
    printf( "Subfolder - PC: Allo %i\n", miVal);
}