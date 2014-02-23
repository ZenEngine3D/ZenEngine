#include "libZenEngine.h"

namespace sample
{	
	void SampleList1x();
	void SampleList2x();
	void SampleHash();
	void SampleHamt();
	void SampleArray();
	void SampleFlags();
	void SampleTypeMisc();
}

int main (int argc, char * const argv[])
{	
	if( FSys::EngineStart() )
	{
		sample::SampleFlags();
		sample::SampleList1x();
		sample::SampleList2x(); 
		sample::SampleHash();
		sample::SampleHamt();
		sample::SampleArray();
		sample::SampleTypeMisc();
		while( CSys::IsSystemActive() );
	}	
	return 0;	
}
