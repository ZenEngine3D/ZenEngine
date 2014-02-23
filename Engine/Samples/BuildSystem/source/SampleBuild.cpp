#include <iostream>
#include <AWEngine/Include/AWECore/AWCore.h>
#include "Multiplatform.h"
#include "subfolder/Subfolder.h"
#include "../libSource/SampleLib.h"

//=================================================================================================
//! @example SampleBuild.cpp
//! Test the functionalities of the build system setup. 
//! In particular, FileFilters for including per platform file/class implementation and libraries creation.
//=================================================================================================

int main (int argc, char * const argv[])
{
	Multiplatform	oTestMultiPlat(1);
	Subfolder		oTestSubdir(2);
	SampleLib		oTestLib(3);

	oTestMultiPlat.print();
	oTestSubdir.print();	
	oTestLib.Print();
	SampleLib::StaticPrint();
	std::cout << "Press enter when done...";
	getchar();

    return 0;
}
