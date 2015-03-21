#include "zenEngine.h"

namespace sample
{	
	void SampleSignal();
	void RunSamples()
	{
		SampleSignal();	
	}
}

int main (int argc, char * const argv[])
{	
	zenSys::zSampleEngineInstance SampleEngine( &sample::RunSamples );
	zenSys::LaunchEngine(&SampleEngine, argc,argv);
	return 0;
}