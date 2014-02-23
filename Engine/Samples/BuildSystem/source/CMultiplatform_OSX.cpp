#include <stdio.h>
#include "Multiplatform_base.h"

Multiplatform::Multiplatform(int aiVal)
: Multiplatform_base(aiVal)
{
}

void Multiplatform::print()
{
    printf("Multiplatform - OSX: Allo %i\n", miVal);
}