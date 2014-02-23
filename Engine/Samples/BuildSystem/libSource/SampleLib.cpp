#include "SampleLib.h"
#include <stdio.h>

SampleLib::SampleLib(int aiValue )
:	mi_Value(aiValue)
{
}

void SampleLib::Print()
{
	printf("SampleLib::Print - %i\n", mi_Value );
}

void SampleLib::StaticPrint()
{
	printf("SampleLib::StaticPrint\n");
}
