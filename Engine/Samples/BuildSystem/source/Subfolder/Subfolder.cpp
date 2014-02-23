#include <AWEngine/Include/AWECore/AWCore.h>
#include "Subfolder.h"
#include <stdio.h>

ISubfolder::ISubfolder(int aival)
{
    miVal = aival;
}

void ISubfolder::print()
{
    printf( "Subfolder - Base: Allo %i\n", miVal);
}