#include "libZenEngine.h"

namespace sample
{	
	void SampleVector();
}

int main (int argc, char * const argv[])
{	
	if( FSys::EngineStart() )		
	{
		sample::SampleVector();	
		while( zbSys::IsSystemActive() );
	}
	return 0;
}