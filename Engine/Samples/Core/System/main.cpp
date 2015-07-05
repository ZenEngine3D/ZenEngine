#include "zenEngine.h"

namespace sample
{	

void SampleThread();
	
void RunSamples()
{
	SampleThread();
}

}

int main (int argc, char * const argv[])
{	
	zenSys::zSampleEngineInstance SampleEngine( &sample::RunSamples );
	zenSys::LaunchEngine(&SampleEngine, argc,argv);
	getchar();
	return 0;	
}