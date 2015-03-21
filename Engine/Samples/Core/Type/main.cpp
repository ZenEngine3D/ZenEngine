#include "zenEngine.h"

namespace sample
{	
	void SampleList1x();
	void SampleList2x();
	void SampleListIntrusive();
	void SampleHash();
	void SampleHamt();
	void SampleArray();
	void SampleFlags();
	void SampleTypeMisc();

	void RunSamples()
	{
		SampleFlags();
		SampleList1x();
		SampleList2x(); 
		SampleListIntrusive();
		SampleHash();
		SampleHamt();
		SampleArray();
		SampleTypeMisc();
	}
}



int main (int argc, char * const argv[])
{	
	zenSys::zSampleEngineInstance SampleEngine( &sample::RunSamples );
	zenSys::LaunchEngine(&SampleEngine, argc,argv);
	return 0;	
}


