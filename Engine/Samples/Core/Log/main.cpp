#include "zenEngine.h"

namespace sample
{	
	void SampleLog();
	void SampleAssert();

	void RunSamples()
	{
		SampleLog();
		SampleAssert();
	}
}

int main (int argc, char * const argv[])
{	
	zenSys::zSampleEngineInstance SampleEngine( &sample::RunSamples );
	zenSys::LaunchEngine(&SampleEngine, argc,argv);
	return 0;
}