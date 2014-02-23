#include <stdio.h>
#include "Subfolder_base.h"

Subfolder::Subfolder(int aiVal)
: Subfolder_base(aiVal)
{
}

void Subfolder::print()
{
    printf("Subfolder - OSX: Allo %i\n", miVal);
}