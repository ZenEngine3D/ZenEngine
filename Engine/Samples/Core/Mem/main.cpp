#include "zenEngine.h"

namespace sample
{	
	void SampleMalloc();
	void SampleFastPool();

	void RunSamples()
	{
		SampleMalloc();
		SampleFastPool();
	}
}



int main (int argc, char * const argv[])
{	
	zenSys::zSampleEngineInstance SampleEngine( &sample::RunSamples );
	zenSys::LaunchEngine(&SampleEngine, argc,argv);
	return 0;	
}