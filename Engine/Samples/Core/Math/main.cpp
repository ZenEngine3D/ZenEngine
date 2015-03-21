#include "zenEngine.h"

namespace sample
{	
	void SampleVector();

	void RunSamples()
	{
		SampleVector();	
	}
}



int main (int argc, char * const argv[])
{	
	zenSys::zSampleEngineInstance SampleEngine( &sample::RunSamples );
	zenSys::LaunchEngine(&SampleEngine, argc,argv);
	return 0;
}