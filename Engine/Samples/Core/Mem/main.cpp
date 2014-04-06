#include "libZenEngine.h"

namespace sample
{	
	void SampleMalloc();
}

int main (int argc, char * const argv[])
{	
	if( FSys::EngineStart() )
	{
		sample::SampleMalloc();		
		while( zbSys::IsSystemActive() );
	}	
	return 0;	
}